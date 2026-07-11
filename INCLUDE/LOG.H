#ifndef _LOG
#define _LOG

#include <Windows.h>

#include "app.h"

#define LOG_SIZE 0x2000

typedef struct ircLog {
	int ptr;
	char text[LOG_SIZE];
} IrcLog;

int writeToLog(IrcMsg *);

#endif
