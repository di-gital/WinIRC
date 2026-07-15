#include <Windows.h>
#include <winsock.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "winirc.h"
#include "app.h"
#include "parser.h"
#include "handlers.h"

extern Application App;

static char output[RECV_BUF_SIZE];

int ircError(IrcUser *user, int code) {
	char ascCode[3];
	ZeroMemory(output, RECV_BUF_SIZE);
	sprintf(ascCode, "%3d", code);
	
	strcat(output, ":");
	strcat(output, App.config.hostname);
	strcat(output, " ");
	strcat(output, ascCode);
	strcat(output, " \r\n");

	send(user->sock, output, strlen(output), 0);

	return TRUE;
}

/* argv[0] is the numeric code */

int ircRpl(IrcMsg *msg, int argc, char **argv) {
	char ascCode[3];
	ZeroMemory(output, RECV_BUF_SIZE);
	return TRUE;
}

int isRegistered(IrcUser *user) {
	if(!user) return FALSE;
	if(!strcmp(user->nick, "") || !strcmp(user->username, ""))
		return FALSE;
	return TRUE;
}

int registered(IrcUser *user) {
	static char userStr[16];
	char code[3];

	if(isRegistered(user)) return FALSE;
	user->registered = TRUE;
	sprintf(code, "%3d", RPL_WELCOME);

	ZeroMemory(output, RECV_BUF_SIZE);
	strcat(output, ":");
	strcat(output, App.config.hostname);
	strcat(output, " ");
	strcat(output, code);
	strcat(output, " ");
	strcat(output, user->username);
	strcat(output, " :Welcome to WinIRC! \r\n");

	send(user->sock, output, strlen(output), 0);

	/* Send messages to appropiate windows */
	ZeroMemory(userStr, 16*sizeof(char));
	SendMessage(App.layout.userList, LB_ADDSTRING, 
		(WPARAM) 0, (LPARAM) (LPCTSTR) &(user->username));
	sprintf(userStr, "Users (%d)", ++(App.nUsers));
	SendMessage(App.layout.userLabel, WM_SETTEXT, 
				(WPARAM) 0, (LPARAM) userStr); 	
	 
	return TRUE;
}	

int handlePass(IrcMsg *msg) {
	IrcUser *tmpUser = malloc(sizeof(IrcUser));

	if(tmpUser == NULL) return FALSE; /* Could not allocate memory */
	
	strncpy(msg->user->pass, msg->argv[0], MAX_PASS);
	msg->user->next = App.users;
	App.users = msg->user;
	App.nUsers++; 	

	return TRUE;
}

int handleNick(IrcMsg *msg) {
	int i;
	IrcUser *userPtr = App.users;
	char *nick;
	char code[3]; 
											  
	if(!msg) return FALSE;
	nick = msg->argv[0];
	
	/* Check if any user already has this nick */
	for(i = 0; i < App.nUsers; i++) {
		if(!strncmp(nick, userPtr->nick, MAX_NICK)) {
			ZeroMemory(output, RECV_BUF_SIZE);
			sprintf(code, "%3d", 433);
			strcat(output, code);
			strcat(output, " ");
			strcat(output, msg->user->nick);
			strcat(output, " :Nickname is already in use\r\n");
			return FALSE;
		}
		userPtr = userPtr->next; 
	}
;
	strncpy(msg->user->nick, msg->argv[0], MAX_NICK); /* Truncates name */
	registered(msg->user);
	return TRUE;
}

/* Assumes IrcUser struct already is allocated. */
int handleUser(IrcMsg *msg) {

	if(!msg || msg->cmd != USER) return FALSE;

	strncpy(msg->user->username, msg->argv[0], MAX_USERNAME);
	strncpy(msg->user->realname, msg->argv[3], MAX_USERNAME);
	
	registered(msg->user);
	return TRUE;
}

int sendPrivMsgs(IrcMsg *msg, IrcChannel *chan) {
	int i;

	ZeroMemory(output, RECV_BUF_SIZE);
	strcat(output, ":");
	strcat(output, msg->user->nick);
	strcat(output, "@");
	strcat(output, App.config.hostname);
	strcat(output, " ");
	strcat(output, CMD_PRIVMSG);
	strcat(output, " ");
	strcat(output, msg->argv[0]);
	strcat(output, " ");
	strcat(output, msg->argv[1]);
	strcat(output, "\r\n");

	for(i = 0; i < MAX_CHANNEL_USERS; i++) {
		if(chan->users[i] && strcmp(msg->user->nick, chan->users[i]->nick)) {
			send(chan->users[i]->sock, output, strlen(output), 0);
		}
	}

	return TRUE;
}

/* Send message to appropiate server-channel pair */
int handlePrivMsg(IrcMsg *msg) {
	int len = strlen(msg->argv[1]);
	char *text = msg->argv[1];
	IrcChannel *chan = getChannel(msg->argv[0]);

	if(msg->cmd != PRIVMSG) return FALSE;
	if(!chan) return FALSE;

	return sendPrivMsgs(msg, chan);
}

/* Insert a channel in the front of the channels list */
int insertChannel(char *name) {
	static char chanStr[16];
	IrcChannel *newChan = calloc(1, sizeof(IrcChannel));
	if(!newChan) return FALSE;

	strncpy(newChan->name, name, MAX_CHANNEL);
	newChan->next = App.channels;
	App.channels = newChan;
	App.nChannels++;
	SendMessage(App.layout.channelList, LB_ADDSTRING, 
		(WPARAM) 0, (LPARAM) name);

	ZeroMemory(chanStr, 16*sizeof(char));
	sprintf(chanStr, "Channels (%d)", App.nChannels);
	SendMessage(App.layout.channelLabel, WM_SETTEXT,
			    (WPARAM) 0, (LPARAM) chanStr);
	return TRUE;
}

int addToChannel(IrcUser *user, IrcChannel *chan) {
	int i;

	if(chan->nUsers >= MAX_CHANNEL_USERS) return FALSE;

	/* Find the first free slot */
	for(i = 0; i < MAX_CHANNEL_USERS; i++) {
		if(!(chan->users[i])) { 
			chan->users[i] = user;
			return TRUE;
		}
	}

	return FALSE;
}

int sendJoinMessages(IrcMsg *msg, IrcChannel *chan) {
	int i;
	char ascCode[3];
	IrcUser *user = msg->user;

	ZeroMemory(output, RECV_BUF_SIZE);

	/* Echo join message */
	strcat(output, ":");
	strcat(output, user->nick);
	strcat(output, "!");
	strcat(output, user->username);
	strcat(output, App.config.hostname);
	strcat(output, " JOIN ");
	strcat(output, msg->argv[0]);
	strcat(output, "\r\n");
	send(user->sock, output, strlen(output), 0);

	/* Send topic if it exists */
	ZeroMemory(output, RECV_BUF_SIZE);	
	if(strcmp(chan->topic, "")) {
		sprintf(ascCode, "%3d", 332);
		strcat(output, ascCode);
		strcat(output, " ");
		strcat(output, msg->argv[0]);
		strcat(output, " :");
		strcat(output, chan->topic);
		strcat(output, "\r\n");
	} else {
		ZeroMemory(output, RECV_BUF_SIZE);
		sprintf(ascCode, "%3d", 331);
		strcat(output, ascCode);
		strcat(output, " ");
		strcat(output, msg->user->nick);
		strcat(output, " ");
		strcat(output, msg->argv[0]);
		strcat(output, " :No topic set\r\n");
	}
	send(user->sock, output, strlen(output), 0); 

	if(1) {
		/* User list */
		ZeroMemory(output, RECV_BUF_SIZE);
		sprintf(ascCode, "%3d", 353);
		strcat(output, ascCode);
		strcat(output, " ");
		strcat(output, user->nick);
		strcat(output, " = ");
		strcat(output, msg->argv[0]);
		strcat(output, " :"); 

		for(i = 0; i < MAX_CHANNEL_USERS; i++) {
			if(chan->users[i]) { 
				strcat(output, chan->users[i]->nick);
				strcat(output, " ");
			}
		}
		strcat(output, "\r\n");	
		send(user->sock, output, strlen(output), 0);
		
		ZeroMemory(output, RECV_BUF_SIZE);
		sprintf(ascCode, "%3d", 366);
		strcat(output, ascCode);
		strcat(output, " ");
		strcat(output, user->nick);
		strcat(output, " ");
		strcat(output, msg->argv[0]);
		strcat(output, "\r\n");	
		send(user->sock, output, strlen(output), 0);
	}

	return TRUE;
	 	
}

/* Add user to channel */
int handleJoin(IrcMsg *msg) {
	char *channel = msg->argv[0]; /* Needs to be comma-delimited */
	IrcChannel *chanPtr = NULL;

	if(!msg->user->registered) return FALSE;
	
	if((chanPtr = getChannel(channel)) == NULL) {
		insertChannel(channel);
		chanPtr = App.channels;
	}
	
	if(addToChannel(msg->user, App.channels))
		sendJoinMessages(msg, App.channels);
	
	return TRUE;
}

int handleTopic(IrcMsg *msg) {
	IrcChannel *chan;
	
	if(!msg || !msg->user || msg->cmd != TOPIC) return FALSE;
	if((chan = getChannel(msg->argv[0])) == NULL)
		return FALSE;

	if(msg->argc == 1) {  /* Query topic */
		send(msg->user->sock, chan->topic, strlen(chan->topic), 0);
	} else if(msg->argc == 2) {	/* Set topic */
		strncpy(chan->topic, msg->argv[1], MAX_TOPIC);		
	} else {
		return FALSE;
	}

	return TRUE;
}

int removeFromChannel(IrcUser *user, IrcChannel *chan) {
	int i;

	for(i = 0; i < MAX_CHANNEL_USERS; i++) {
		if(chan->users[i] == user) {
			chan->users[i] == NULL;
			return TRUE;
		}
	}
	return FALSE;
}

int handleQuit(IrcMsg *msg) {
	int i;
	IrcChannel *chanPtr = App.channels;

	ZeroMemory(output, RECV_BUF_SIZE);
	strcat(output, ":");
	strcat(output, msg->user->nick);
	strcat(output, "!");
	strcat(output, msg->user->username);
	strcat(output, "@");
	strcat(output, msg->user->server);
	strcat(output, " QUIT :");
	strcat(output, msg->argv[0]);
	strcat(output, "\r\n");
	send(msg->user->sock, output, strlen(output), 0);
	
	closesocket(msg->user->sock);
	for(i = 0; i < App.nChannels; i++) {
		removeFromChannel(msg->user, chanPtr);
		chanPtr = chanPtr->next;
	} 
	destroyUser(msg->user);
	return TRUE;
}
