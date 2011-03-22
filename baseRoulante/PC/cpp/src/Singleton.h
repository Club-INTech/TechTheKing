#ifndef SINGLETON_H
#define SINGLETON_H

#include <iostream>

class Singleton{
public:
	static Singleton* Instance();
	template <class C> static Singleton* Instance(C a);
	template <class C, class M> static Singleton* Instance(C a,M b);
    ~Singleton(){};
protected:
	static Singleton* m_instance;
	Singleton(){};
	Singleton(const Singleton&);
    Singleton* operator=(const Singleton&){};
};

#endif
