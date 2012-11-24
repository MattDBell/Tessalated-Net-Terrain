#ifndef DEBUGGING_H
#define DEBUGGING_H

#define LOG(file, line)		Debugging::Log(file, line, __LINE__, __FILE__)
#define LOGDEFAULT(line)	Debugging::Log("Main", line, __LINE__, __FILE__)
#define MAXBUFFERLENGTH		1000

class Log;

class Debugging
{
	static Debugging* instance;
	static Debugging* Get();
	Log* logs;
	int numLogs;
	
public:
	static void Log(char * file, char * line, int lineNumber, char* fileName);
	static void Flush();
};

#endif