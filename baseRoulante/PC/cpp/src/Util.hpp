#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>
#define abs(x) ((x) > 0 ? (x) : -(x))

template<typename T>
std::string numToString(T smthg)
{
	std::ostringstream oss;
	oss << smthg ;
	return oss.str();
}

#endif
