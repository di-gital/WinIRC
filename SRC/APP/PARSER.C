#include <Windows.h>
#include <stdlib.h>

#include "parser.h"
#include "handlers.h"

IrcMsg tmpMsg;

/* Inputs:
 * char *str: pointer to string containing command
 * IrcMsg *msgStruct: pointer to entire message */
int getCmd(char *str, IrcMsg *msgStruct) {
	if(!strcmp(str, CMD_PRIVMSG)) {
		msgStruct->cmd = PRIVMSG;
		msgStruct->argc = 2;
		msgStruct->handler = handlePrivMsg;	
	} else if(!strcmp(str, CMD_JOIN)) {
		msgStruct->cmd = JOIN; 
		msgStruct->argc = 1;
		msgStruct->handler = handleJoin;
	} else if(!strcmp(str, CMD_MODE)) {
		msgStruct->cmd = MODE;
		msgStruct->argc = 0; /* TODO */	
	} else if(!strcmp(str, CMD_NICK)) {
		msgStruct->cmd = NICK;
		msgStruct->argc = 1;
		msgStruct->handler = handleNick;
	} else if(!strcmp(str, CMD_TOPIC)) {
		msgStruct->cmd = TOPIC;
		msgStruct->handler = handleTopic;
	} else if(!strcmp(str, CMD_NJOIN)) {
		msgStruct->cmd = NJOIN;
		msgStruct->argc = 1;
	} else if(!strcmp(str, CMD_PASS)) {
		msgStruct->cmd = PASS;
		msgStruct->argc = 4;
	} else if(!strcmp(str, CMD_SERVER)) {
		msgStruct->cmd = SERVER;
		msgStruct->argc = 4;
	} else if(!strcmp(str, CMD_SERVICE)) { 
		msgStruct->cmd = SERVICE;
		msgStruct->argc = 6;
	} else if(!strcmp(str, CMD_QUIT)) {
		msgStruct->cmd = QUIT;
		msgStruct->argc = 1;
		msgStruct->handler = handleQuit;
	} else if(!strcmp(str, CMD_SQUIT)) {
		msgStruct->cmd = SQUIT;
		msgStruct->argc = 2;
	} else if(!strcmp(str, CMD_USER)) {
		msgStruct->cmd = USER;
		msgStruct->argc = 3;
		msgStruct->handler = handleUser;
	} else { /* Command not recognized */
		msgStruct->cmd = ERROR;
		msgStruct->argc = 0;
  		return 0;
	}

	return 1;
}

int getArgV(IrcMsg *msg) {
	int i;
	int len;
	char *tmpStr;

	switch(msg->cmd) {
		case TOPIC:
			msg->argv[0] = strtok(NULL, " ");
			tmpStr = strtok(NULL, " ");
			msg->argc = (!tmpStr) ? 1 : 2;
			if(msg->argc == 2) { /* Topic change */
				msg->argv[1] = tmpStr + 1;
				len = strlen(tmpStr);
				/* Use entire string, not just first word */
				tmpStr[len] = ' '; 
			} 
			break;
		case PRIVMSG:
			msg->argv[0] = strtok(NULL, " ");
			msg->argv[1] = strtok(NULL, " ");
			len = strlen(msg->argv[1]);
			msg->argv[1][len] = ' ';
			break;
		case QUIT:
			msg->argv[0] = strtok(NULL, " ");
			len = strlen(msg->argv[0]);
			msg->argv[0][len] = ' ';
		default:
			for(i = 0; i < msg->argc; i++) {
				msg->argv[i] = strtok(NULL, " ");
			} 
			 
	}
	return TRUE;
}

/* Populate an IrcMsg struct */
/* Expects user and text fields to already be populated. */
int ircParseMsg(IrcMsg *msg) {
	int len;	
	char *command;
	IrcUser *user = msg->user;

	if(!msg || !(msg->user)) return FALSE;

	len = strlen(msg->text);
	msg->text[len - 2] = '\0'; /* Sanitize line ending */

	command = strtok(msg->text, " ");
	getCmd(command, msg);
	
	msg->argv = malloc(msg->argc*sizeof(char *));
	if(!msg->argv) return FALSE; 

	return getArgV(msg);
}  
 
