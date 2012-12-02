#ifndef DEBUGGING_H
#define DEBUGGING_H

#include <stdio.h>
#include <cstring>

#define LOG(file, toLog)		Debugging::Log(file, toLog, __LINE__, __FILE__)
#define LOGDEFAULT(toLog)	Debugging::Log("Main", toLog, __LINE__, __FILE__)
#define MAXBUFFERLENGTH		1000

#define numConcurrentLogs (10)
#define logSize (1000)


class Debugging
{
	struct Log
	{
		char * m_file;
		char actualLog[logSize];
		int cursor;
		Log() 
			: m_file(0), cursor(0)
		{ actualLog[0] = '\0';}
		void LogLine(char * file, char* toLog);
		void Flush();
	};
	static Debugging* instance;
	static Debugging* Get() 
	{ 
		if(!instance)
			instance = new Debugging();
		return instance;
	}
	Log logs[numConcurrentLogs];
	Debugging(){}
public:
	static void Log(char * file, char * toLog, int lineNumber, char* fileName);
	static void Flush();
};

#endif