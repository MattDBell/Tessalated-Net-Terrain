#include "Debugging.h"
#include <cmath>

#pragma warning (push)
#pragma warning (disable: 4996)
Debugging* Debugging::instance = 0;
char * LineNumberToString(int lineNumber)
{
	const char * prefix = " ( "; //3
	const char * postfix = " ) : "; //5 + null 
	int numDigits = int(log10((double)lineNumber)) + 1;
	char * ret = new char[numDigits + 9]; //3 + 5 + null
	memcpy(ret, prefix, 3);
	for(int i = 0; i < numDigits; ++i)
	{
		ret[3 + (numDigits - i - 1)] = (char)((lineNumber % 10) + '0');
		lineNumber /= 10;
	}
	memcpy(&ret[3 + numDigits], postfix, 6);
	return ret;
}

void Debugging::Log::LogLine(char * file, char* toLog)
{
	if(m_file && !strcmp(file, file))
	{
		int length = strlen(toLog);
		if(logSize - cursor > length)
		{
			memcpy(&actualLog[cursor], toLog, length +1);
			cursor += length;
			return;
		} else
		{
			Flush();
			LogLine(file, toLog);
		}
	} else {
		if(m_file)
		{
			Flush();
			LogLine(file, toLog);
		} else {
			m_file = file;
			LogLine(file, toLog);
		}
	}
}
void Debugging::Log::Flush()
{
	if(!m_file)
		return;
	FILE* f = fopen(m_file, "a");
	fwrite(actualLog, sizeof(char), cursor + 1, f);
	fclose(f);
	m_file = 0;
}

void Debugging::Log(char * file, char * toLog, int lineNumber, char* fileName)
{
	if(!toLog || !file || !fileName) return;
	int flength = strlen(fileName);
	char * number = LineNumberToString(lineNumber);
	int nlength = strlen(number);
	int llength = strlen(toLog);
	char * actLog = new char[flength + nlength + llength + 2];
	actLog[flength + nlength + llength] = '\n';
	actLog[flength + nlength + llength + 1] = '\0';
	memcpy(&actLog[0], fileName, flength);
	memcpy(&actLog[flength], number, nlength);
	memcpy(&actLog[flength + nlength], toLog, llength);
	delete[] number;

	for(int i = 0; i < numConcurrentLogs; ++i)
	{
		if(Get()->logs[i].m_file && !strcmp(Get()->logs[i].m_file, file)){
			Get()->logs[i].LogLine(file, actLog);
			delete[] actLog;
			return;
		}
	}
	for(int i = 0; i < numConcurrentLogs; ++i)
	{
		if(!Get()->logs[i].m_file)
		{
			Get()->logs[i].LogLine(file, actLog);
			delete[] actLog;
			return;
		}
	}
	int maxI = 0;
	int max = Get()->logs[0].cursor;
	for(int i = 1; i < numConcurrentLogs; ++i)
	{
		if(Get()->logs[i].cursor > max)
		{
			max = Get()->logs[i].cursor;
			maxI = i;
		}
	}
	Get()->logs[maxI].LogLine(file, actLog);
	delete[] actLog;
}

void Debugging::Flush()
{
	for(int i = 0; i < numConcurrentLogs; ++i)
		Get()->logs[i].Flush();
}
#pragma warning (pop)