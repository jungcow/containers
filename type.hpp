#ifndef TYPE_HPP
#define TYPE_HPP

#include <string>
#include <typeinfo>

#ifdef __GNUG__
#include <cxxabi.h>

#include <cstdlib>
#include <memory>

std::string demangle(const char* name);

template <class T>
std::string type(const T& t)
{
	return demangle(typeid(t).name());
}

#if __cplusplus == 201103L

std::string demangle(const char* name)
{
	int status = -4;  // some arbitrary value to eliminate the compiler warning

	// enable c++11 by passing the flag -std=c++11 to g++
	std::unique_ptr<char, void (*)(void*)> res{
		abi::__cxa_demangle(name, NULL, NULL, &status),
		std::free};

	return (status == 0) ? res.get() : name;
}

#elif __cplusplus == 199711L

struct handle
{
	char* p;
	handle(char* ptr) : p(ptr) {}
	~handle() { std::free(p); }
};

std::string demangle(const char* name)
{
	int status = -4;  // some arbitrary value to eliminate the compiler warning

	handle result(abi::__cxa_demangle(name, NULL, NULL, &status));

	return (status == 0) ? result.p : name;
}

#endif

#else
#if __cplusplus == 201103L

// does nothing if not g++
std::string demangle(const char* name)
{
	return name;
}

#elif __cplusplus == 199711L

// does nothing if not g++
std::string demangle(const char* name)
{
	return name;
}
#endif

#endif

#endif
