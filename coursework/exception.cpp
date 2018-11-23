#include "exception.h"

Exception::Exception(std::string message)
{
	DWORD errorMessageID = GetLastError();

	if (errorMessageID != 0)
	{
		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
		this->message = message + ": " + std::string(messageBuffer, size);
		LocalFree(messageBuffer);
	}
	else
		this->message = message;
}

const char *Exception::what() const throw ()
{
	return this->message.c_str();
}
