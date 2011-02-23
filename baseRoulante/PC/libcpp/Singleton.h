#ifndef SINGLETON_H
#define SINGLETON_H

template <class T> class Singleton{

public:
	static T& Instance();
	template <class M> static T& Instance();
	template <class M, class C> static T& Instance();
protected:
	static T m_instance;
	Singleton(){};
	Singleton(const T&);
    T& operator=(const T&){};
};

#endif
