/*
 * systemInitializer.c
 *
 *  Created on: Apr 24, 2012
 *      Author: lutz
 */

#include <cranc/module/Module.h>
#include <cranc/stdtypes.h>

#include <algorithm>

static unsigned int callAllModules(unsigned int level) {
	auto* module = cranc::util::LinkedList<cranc::Module>::get().mFirst;
	unsigned int maxLevel = ~0U;
	while (module) {
		if (module->mInitLevel == level) {
			module->init(level);
		} else {
			if (module->mInitLevel > level) {
				maxLevel = std::min(maxLevel, module->mInitLevel);
			}
		}
		module = module->mNext;
	}
	return maxLevel;
}

void cranc::InitializeModules()
{
	unsigned int maxLevel = 0;
	do {
		maxLevel = callAllModules(maxLevel);
	} while (maxLevel != ~0U);
}


