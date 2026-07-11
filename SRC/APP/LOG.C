/* Log-writing code */

#include <Windows.h>
#include <string.h>
#include <time.h>

#include "winirc.h"
#include "app.h"

extern Application App;

/* Write a string to the temporary buffer. Returns a positive number of
 * bytes written. Nonpositive if there is a failure. */
 
/* If the buffer is overflowed, the entire log is written to the file
 * specified in Config. Otherwise, the buffer wraps around. */  
int logWriter(char *str) {
	int length = strlen(str);
	
	if(!str) return FALSE;
	strcat(&(App.log.text[App.log.ptr]), str);
	App.log.ptr += length;
	return TRUE;
}

/* Generates the correct string to send to logWriter() */
int writeToLog(IrcMsg *msg) {
	time_t t;
	struct tm *timeinfo;
	static char tmpBuf[0x1000];
	char *timeAsc;
	ZeroMemory(tmpBuf, 0x1000);

	time(&t);
	timeinfo = localtime(&t);
	timeAsc = asctime(timeinfo);
	timeAsc[24] = '\0';

	strcat(tmpBuf, timeAsc);
	strcat(tmpBuf, ": ");
	strcat(tmpBuf, msg->user->username);

	switch(msg->cmd) {
		case JOIN:
			strcat(tmpBuf, " has joined channel ");
			strcat(tmpBuf, msg->argv[0]);
			break;
		case USER:
			strcat(tmpBuf, " has connected ");
			break; 
		case NICK:
			strcat(tmpBuf, " has changed nick to ");
			strcat(tmpBuf, msg->user->nick);
			break;
		case TOPIC:
			if(msg->argc == 1) return 0;
			strcat(tmpBuf, " has changed the topic of ");
			strcat(tmpBuf, msg->argv[0]);
			strcat(tmpBuf, " to: ");
			strcat(tmpBuf, msg->argv[1]);
			break;
		case PASS:
			strcat(tmpBuf, " had a password (re)set");
			break;
		case QUIT:
			strcat(tmpBuf, " has left.");
			break;
		default: 
			return 0; /* Not a command that is logged */
	}
	SendMessage(App.layout.logText, LB_ADDSTRING, (WPARAM) 0, (LPARAM) tmpBuf);
	strcat(tmpBuf, "\r\n");
	return logWriter(tmpBuf);
}
