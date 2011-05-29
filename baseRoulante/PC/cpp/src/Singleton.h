#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <boost/utility.hpp>
#include <boost/thread/once.hpp>
#include <boost/scoped_ptr.hpp>

template<class T>
class Singleton : private boost::noncopyable
{
public:
    static T& Instance();
    static void init();
protected:
    ~Singleton() {}
     Singleton() {}
private:
     static boost::scoped_ptr<T> t;
     static boost::once_flag flag;
};

#endif
