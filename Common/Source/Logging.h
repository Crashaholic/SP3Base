#ifndef LOGGING_HPP
#define LOGGING_HPP

/*
|
|	Made by Yan Quan
|
*/

#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>

#define LOG_NONE(sLog)			Log::None(sLog, __FUNCTION__, __FILE__, __LINE__)
#define LOG_WARN(sLog)			Log::Warn(sLog, __FUNCTION__, __FILE__, __LINE__)
#define LOG_TRACE(sLog)			Log::Trace(sLog, __FUNCTION__, __FILE__, __LINE__)
#define LOG_ERROR(sLog)			Log::Error(sLog, __FUNCTION__, __FILE__, __LINE__)

namespace Log
{
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	void None(std::string text, const char* functionName, const char* fileName, unsigned int lineNumber)
	{
#ifdef _DEBUG
		SetConsoleTextAttribute(hConsole, 0x0f);
		std::cout
#ifdef LOG_WITH_DATE
			<< __DATE__ << " "
#endif
			<< __TIME__;
		std::cout << " [LOG]  : ";
		std::cout << text;
		std::cout << " at " << functionName << " in " << fileName << "(" << lineNumber << ")\n";
		SetConsoleTextAttribute(hConsole, 0x0f);
#else
		SetConsoleTextAttribute(hConsole, 0x0e);
		std::cout << "Logging: Debug is disabled\n";
#endif
	}

	void Warn(std::string text, const char* functionName, const char* fileName, unsigned int lineNumber)
	{
#ifdef _DEBUG
		SetConsoleTextAttribute(hConsole, 0x0e);
		std::cout
#ifdef LOG_WITH_DATE
		 << __DATE__ << " "
#endif
		<< __TIME__;
		SetConsoleTextAttribute(hConsole, 0x0f);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, 0x0e);
		std::cout << "WARN";
		SetConsoleTextAttribute(hConsole, 0x0f);
		std::cout << "] : ";
		std::cout << text;
		SetConsoleTextAttribute(hConsole, 0x0e);
		std::cout << " at " << functionName << " in " << fileName << "(" << lineNumber << ")\n";
		SetConsoleTextAttribute(hConsole, 0x0f);
#else
		SetConsoleTextAttribute(hConsole, 0x0e);
		std::cout << "Logging: Debug is disabled\n";
#endif
	}

	void Trace(std::string text, const char* functionName, const char* fileName, unsigned int lineNumber)
	{
#ifdef _DEBUG
		SetConsoleTextAttribute(hConsole, 0x0a);
		std::cout
#ifdef LOG_WITH_DATE
			<< __DATE__ << " "
#endif
			<< __TIME__;
		SetConsoleTextAttribute(hConsole, 0x0f);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, 0x0a);
		std::cout << "TRACE";
		SetConsoleTextAttribute(hConsole, 0x0f);
		std::cout << "]: ";
		std::cout << text;
		SetConsoleTextAttribute(hConsole, 0x0a);
		std::cout << " at " << functionName << " in " << fileName << "(" << lineNumber << ")\n";
		SetConsoleTextAttribute(hConsole, 0x0f);
#else
		SetConsoleTextAttribute(hConsole, 0x0a);
		std::cout << "Logging: Debug is disabled\n";
#endif
	}

	void Error(std::string text, const char* functionName, const char* fileName, unsigned int lineNumber)
	{
#ifdef _DEBUG
		SetConsoleTextAttribute(hConsole, 0x0c);
		std::cout
#ifdef LOG_WITH_DATE
			<< __DATE__ << " "
#endif
			<< __TIME__;
		SetConsoleTextAttribute(hConsole, 0x0f);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, 0x0c);
		std::cout << "ERROR";
		SetConsoleTextAttribute(hConsole, 0x0f);
		std::cout << "]: ";
		std::cout << text;
		SetConsoleTextAttribute(hConsole, 0x0c);
		std::cout << " at " << functionName << " in " << fileName << "(" << lineNumber << ")\n";
		SetConsoleTextAttribute(hConsole, 0x0f);
#else
		SetConsoleTextAttribute(hConsole, 0x0c);
		std::cout << "Logging: Debug is disabled\n";
#endif
	}

};

#endif