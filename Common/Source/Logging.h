#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>

/**
	Made by Yan Quan (Handle: Crashaholic)\n
	Logger header file, used for user-defined errors\n
	\n
	Thanks James for fixing!\n
	\n
	MACROS:\n
	\n
	- LOG_NONE (const char* text_here) //No priority logging\n
	- LOG_WARN (const char* text_here) //Low priority logging\n
	- LOG_TRACE(const char* text_here) //Medium priority logging\n
	- LOG_ERROR(const char* text_here) //HIGHEST priority logging\n

*/

/**
	\author Yan Quan (Crashaholic)\n
	\brief  Made for outputing user-defined messages into the console\n
*/
struct Log
{
	static HANDLE hConsole; /// The WINAPI Handle

	Log() {};
	~Log() {};

	/**
		@brief Prints out the location of the function call
	*/
	void pLocation(const char* functionName, const char* fileName)
	{
#ifdef SHOW_LOC
		std::cout << " at " << functionName << " in " << fileName << "\n";
		SetConsoleTextAttribute(hConsole, 0x0f);
#else
		std::cout << '\n';
#endif
	}

	/**
		@brief Prints out the input without 
	*/
	void None(const char* val)
	{
		SetConsoleTextAttribute(hConsole, 0x0f);
		std::cout << val;
	}

	/**
		@brief Prints out the location of the function call
	*/
	void Warn(const char* val)
	{
		SetConsoleTextAttribute(hConsole, 0x0e);
		std::cout << val;
	}

	/**
		@brief Prints out the location of the function call
	*/
	void Trace(const char* val)
	{
		SetConsoleTextAttribute(hConsole, 0x0a);
		std::cout << val;
	}

	/**
		@brief Prints out the location of the function call
	*/
	void Error(const char* val)
	{
		SetConsoleTextAttribute(hConsole, 0x0c);
		std::cout << val;
	}

	void PrintTime(WORD attrib)
	{
		SetConsoleTextAttribute(Log::hConsole, attrib);
		std::cout << __TIME__ << " | ";
	}

	/**
		@brief Prints out the location of the function call
	*/
	template<typename T, typename... Targs>
	void None(const char* format, T value, Targs... Fargs)
	{
		SetConsoleTextAttribute(Log::hConsole, 0x0f);
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

	/**
		@brief Prints out the location of the function call
	*/
	template<typename T, typename... Targs>
	void Warn(const char* format, T value, Targs... Fargs)
	{
		SetConsoleTextAttribute(Log::hConsole, 0x0e);
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

	/**
		@brief Prints out the location of the function call
	*/
	template<typename T, typename... Targs>
	void Trace(const char* format, T value, Targs... Fargs)
	{
		SetConsoleTextAttribute(Log::hConsole, 0x0a);
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

	/**
		@brief Prints out the location of the function call
	*/
	template<typename T, typename... Targs>
	void Error(const char* format, T value, Targs... Fargs)
	{
		SetConsoleTextAttribute(Log::hConsole, 0x0c);
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
#define LOG_NONE(...)		logger.PrintTime(0x0f) ;logger.None (__VA_ARGS__); logger.pLocation(__FUNCTION__, __FILENAME__)
#define LOG_WARN(...)		logger.PrintTime(0x0e) ;logger.Warn (__VA_ARGS__); logger.pLocation(__FUNCTION__, __FILENAME__)
#define LOG_TRACE(...)		logger.PrintTime(0x0a) ;logger.Trace(__VA_ARGS__); logger.pLocation(__FUNCTION__, __FILENAME__)
#define LOG_ERROR(...)		logger.PrintTime(0x0c) ;logger.Error(__VA_ARGS__); logger.pLocation(__FUNCTION__, __FILENAME__)

#else
#define LOG_NONE
#define LOG_WARN
#define LOG_TRACE
#define LOG_ERROR

#endif

#endif