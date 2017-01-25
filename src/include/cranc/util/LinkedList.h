#pragma once

#include "Singleton.h"

namespace cranc
{
namespace util
{

template<typename T>
class LinkedListNode {
public:
	LinkedListNode();
	T* mNext {nullptr};
protected:
	virtual ~LinkedListNode();
};

template<typename T>
class LinkedList final : public Singleton<LinkedList<T>>{
public:
	void add(T* element) {
		element->mNext = this->mFirst;
		this->mFirst = element;
	}

	void remove(T* element) {
		T **iter = &(this->mFirst);
		while (*iter) {
			T **next = &((*iter)->mNext);
			if (*next == element) {
				(*iter)->mNext = element->mNext;
				return;
			}
			iter = next;
		}
	}
	virtual ~LinkedList() {}

	T* mFirst {nullptr};
};


template<typename T>
LinkedListNode<T>::LinkedListNode() {
	LinkedList<T>::get().add((T*)this);
}

template<typename T>
LinkedListNode<T>::~LinkedListNode() {
	LinkedList<T>::get().remove((T*)this);
}

}
}
