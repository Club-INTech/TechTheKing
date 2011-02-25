#ifndef SINGLETON_H
#define SINGLETON_H

#include <iostream>

template <class T> class Singleton{

public:
	static T& Instance();
	template <class C> static T& Instance(C a);
	template <class C, class M> static T& Instance(C a,M b);
protected:
	static T m_instance;
	Singleton(){};
	Singleton(const T&);
    T& operator=(const T&){};
};

#endif
