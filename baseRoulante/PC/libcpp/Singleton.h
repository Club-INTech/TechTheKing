#ifndef SINGLETON_H
#define SINGLETON_H

template <class T> class Singleton
{

public:
	static T& Instance();
protected:
	static T m_i;

private:
        T& operator=(const T&){}
};

#endif
