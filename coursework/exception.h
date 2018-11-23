#pragma once

#include <windows.h>
#include <exception>
#include <string>

class Exception : public std::exception
{
public:
	Exception(std::string);
	const char *what() const throw ();

private:
	std::string message;
};
