#ifndef SINGLETON_H
#define SINGLETON_H

#include <iostream>

template<typename T>
class Singleton{
public:
	static T* Instance();
	template <class C> static Singleton* Instance(C a);
	template <class C, class M> static Singleton* Instance(C a,M b);
    ~Singleton(){};
protected:
	static T* m_instance;
	Singleton(){};
	Singleton(const Singleton&);
    T* operator=(const Singleton&){};
};

#endif
