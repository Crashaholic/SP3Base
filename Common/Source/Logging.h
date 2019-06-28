#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>

/*
|
|	Made by Yan Quan (Handle: Crashaholic)
|	Logger header file, used for user-defined errors
|
|	Thanks James for fixing!
|
|	MACROS:
|
|	- LOG_NONE (const char* text_here) //No priority logging
|	- LOG_WARN (const char* text_here) //Low priority logging
|	- LOG_TRACE(const char* text_here) //Medium priority logging
|	- LOG_ERROR(const char* text_here) //HIGHEST priority logging
|
*/
struct Log
{
	static HANDLE hConsole;

	Log() {};
	~Log() {};

	void pLocation(const char* functionName, const char* fileName)
	{
		std::cout << " at " << functionName << " in " << fileName << "\n";
		SetConsoleTextAttribute(hConsole, 0x0f);
	}

	void None(const char* val)
	{
		SetConsoleTextAttribute(hConsole, 0x0f);
		std::cout << val;
	}

	void Warn(const char* val)
	{
		SetConsoleTextAttribute(hConsole, 0x0e);
		std::cout << val;
	}

	void Trace(const char* val)
	{
		SetConsoleTextAttribute(hConsole, 0x0a);
		std::cout << val;
	}

	void Error(const char* val)
	{
		SetConsoleTextAttribute(hConsole, 0x0c);
		std::cout << val;
	}

	template<typename T, typename... Targs>
	void None(const char* format, T value, Targs... Fargs)
	{
		SetConsoleTextAttribute(Log::hConsole, 0x0f);
		std::cout << __TIME__ << " | ";
		for (; *format != '\0'; ++format)
		{
			if (*format == '%')
			{
				std::cout << value;
				Log::None(format + 1, Fargs...);
				return;
			}
			std::cout << *format;
		}
	}


	template<typename T, typename... Targs>
	void Warn(const char* format, T value, Targs... Fargs)
	{
		SetConsoleTextAttribute(Log::hConsole, 0x0e);
		std::cout << __TIME__ << " | ";
		for (; *format != '\0'; ++format)
		{
			if (*format == '%')
			{
				std::cout << value;
				Log::Warn(format + 1, Fargs...);
				return;
			}
			std::cout << *format;
		}
	}

	template<typename T, typename... Targs>
	void Trace(const char* format, T value, Targs... Fargs)
	{
		SetConsoleTextAttribute(Log::hConsole, 0x0a);
		std::cout << __TIME__ << " | ";
		for (; *format != '\0'; ++format)
		{
			if (*format == '%')
			{
				std::cout << value;
				Log::Trace(format + 1, Fargs...);
				return;
			}
			std::cout << *format;
		}
	}

	template<typename T, typename... Targs>
	void Error(const char* format, T value, Targs... Fargs)
	{
		SetConsoleTextAttribute(Log::hConsole, 0x0c);
		std::cout << __TIME__ << " | ";
		for (; *format != '\0'; ++format)
		{
			if (*format == '%')
			{
				std::cout << value;
				Log::Error(format + 1, Fargs...);
				return;
			}
			std::cout << *format;
		}
	}
};

static Log logger;

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#ifdef _DEBUG
#define LOG_NONE(...)		logger.None (__VA_ARGS__); logger.pLocation(__FUNCTION__, __FILENAME__)
#define LOG_WARN(...)		logger.Warn (__VA_ARGS__); logger.pLocation(__FUNCTION__, __FILENAME__)
#define LOG_TRACE(...)		logger.Trace(__VA_ARGS__); logger.pLocation(__FUNCTION__, __FILENAME__)
#define LOG_ERROR(...)		logger.Error(__VA_ARGS__); logger.pLocation(__FUNCTION__, __FILENAME__)
#endif


#endif