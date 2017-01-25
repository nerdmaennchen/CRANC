
TARGET = cranc-example

# compiler
CROSS_COMPILE_PREFIX = 
CC      = $(CROSS_COMPILE_PREFIX)gcc
CPP     = $(CROSS_COMPILE_PREFIX)g++
LD      = $(CROSS_COMPILE_PREFIX)g++
SIZE    = $(CROSS_COMPILE_PREFIX)size
GDB     = $(CROSS_COMPILE_PREFIX)gdb
OBJ_CPY = $(CROSS_COMPILE_PREFIX)objcopy

SRC_FOLDER = src/
LIBS =
LIB_PATHS =
INCLUDES = src/ \
	src/include

LINKER_SCRIPT_FILE =
MAP_FILE = $(TARGET).map


INCLUDE_CMD = $(addprefix -I, $(INCLUDES))
LIB_CMD = $(addprefix -l, $(LIBS))
LIB_PATH_CMD = $(addprefix -L, $(LIB_PATHS))

# Flags

DEFINES		+=

FP_FLAGS       ?=
COMMON_FLAGS	+= $(DEFINES) $(FP_FLAGS)
COMMON_FLAGS	+= -Os -g3
COMMON_FLAGS	+= $(INCLUDE_CMD)
COMMON_FLAGS	+= -fvisibility=hidden

# Warnings
W_FLAGS      += -Wextra -Wshadow -Wredundant-decls
W_FLAGS      += -Wall -Wundef

###############################################################################
# C flags

CFLAGS		+= $(COMMON_FLAGS)
CFLAGS		+= $(W_FLAGS)
CFLAGS      += -Wimplicit-function-declaration -Wmissing-prototypes -Wstrict-prototypes

###############################################################################
# C++ flags

CPPFLAGS	+= $(COMMON_FLAGS)
CPPFLAGS	+= $(W_FLAGS)
# add this for link-time template instanciation
#CPPFLAGS    += -frepo
CPPFLAGS    += 
CPPFLAGS    += -fno-rtti -fno-exceptions -fno-threadsafe-statics
CPPFLAGS	+= -std=c++11
CPPFLAGS	+= -MD
CPPFLAGS	+= -I$(INCLUDE_DIR)


###############################################################################
# Linker flags

LINKERFLAGS +=  $(COMMON_FLAGS) -frepo
LINKERFLAGS += -Wl,--gc-sections
LINKERFLAGS += -Wl,-Map=$(TARGET).map
#LINKERFLAGS += -s

CPP_SUFFIX = .cpp
C_SUFFIX   = .c
OBJ_SUFFIX = .o
DEP_SUFFIX = .d
OBJ_DIR    = obj/

# files that should be listed first when linking
IMPORTANT_ORDER_FILES =
IGNORE_STRINGS = */archive/*


IGNORE_STRINGS       += $(IMPORTANT_ORDER_FILES)
CPP_FILES            += $(sort $(filter-out $(IGNORE_STRINGS), $(shell find $(SRC_FOLDER) -name "*$(CPP_SUFFIX)" | grep -v $(addprefix -e, $(IGNORE_STRINGS)))))
IMPORTANT_CPP_FILES  += $(sort $(filter-out $(IGNORE_STRINGS), $(shell find $(SRC_FOLDER) -name "*$(CPP_SUFFIX)" | grep $(addprefix -e, $(IGNORE_STRINGS)))))
ALL_CPP_FILES         = $(CPP_FILES) $(IMPORTANT_CPP_FILES)

C_FILES              += $(sort $(filter-out $(IGNORE_STRINGS), $(shell find $(SRC_FOLDER) -name "*$(C_SUFFIX)" | grep -v $(addprefix -e, $(IGNORE_STRINGS)))))
IMPORTANT_C_FILES    += $(sort $(filter-out $(IGNORE_STRINGS), $(shell find $(SRC_FOLDER) -name "*$(C_SUFFIX)" | grep $(addprefix -e, $(IGNORE_STRINGS)))))
ALL_C_FILES           =  $(C_FILES) $(IMPORTANT_C_FILES)

CPP_OBJ_FILES        += $(addsuffix $(OBJ_SUFFIX), $(addprefix $(OBJ_DIR), $(CPP_FILES) $(IMPORTANT_CPP_FILES)))
C_OBJ_FILES          += $(addsuffix $(OBJ_SUFFIX), $(addprefix $(OBJ_DIR), $(C_FILES) $(IMPORTANT_C_FILES)))


DEP_FILES            += $(addprefix $(OBJ_DIR), $(addsuffix $(DEP_SUFFIX), $(ALL_CPP_FILES) $(ALL_C_FILES)))

ifndef VERBOSE
SILENT = @
endif


.phony: all clean flash

all: $(TARGET).elf

clean:
	$(SILENT) rm -rf $(OBJ_DIR) $(TARGET).elf $(TARGET).map $(TARGET).bin


$(TARGET).elf: $(CPP_OBJ_FILES) $(C_OBJ_FILES)
	$(SILENT) echo linking $(target)
	$(SILENT) $(LD) -o $@ $^ $(LINKERFLAGS) $(LIB_PATH_CMD) $(LIB_CMD)
	$(SILENT) $(SIZE) $@
	@ echo done

$(OBJ_DIR)%$(C_SUFFIX)$(OBJ_SUFFIX): %$(C_SUFFIX) $(LINKER_SCRIPT_FILE)
	@echo building $<
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) $(INCLUDE_CMD) -MM -MF $(OBJ_DIR)$<.d -c $<
	@ mv -f $(OBJ_DIR)$<.d $(OBJ_DIR)$<.d.tmp
	@ sed -e 's|.*:|$@:|' < $(OBJ_DIR)$<.d.tmp > $(OBJ_DIR)$<.d
	@ rm -f $(OBJ_DIR)$<.d.tmp
	
	$(SILENT) $(CC) $(CFLAGS) $(INCLUDE_CMD) -o $@ -c $<
	
	
$(OBJ_DIR)%$(CPP_SUFFIX)$(OBJ_SUFFIX): %$(CPP_SUFFIX)
	@echo building $<
	@ mkdir -p $(dir $@)
	@ $(CPP) $(CPPFLAGS) $(INCLUDE_CMD) -MM -MF $(OBJ_DIR)$<.d -c $<
	@ mv -f $(OBJ_DIR)$<.d $(OBJ_DIR)$<.d.tmp
	@ sed -e 's|.*:|$@:|' < $(OBJ_DIR)$<.d.tmp > $(OBJ_DIR)$<.d
	@ rm -f $(OBJ_DIR)$<.d.tmp
	
	$(SILENT) $(CPP) $(CPPFLAGS) $(INCLUDE_CMD) -o $@ -c $<

-include $(DEP_FILES)
