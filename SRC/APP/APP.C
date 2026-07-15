/* WinSock 1.1 code. Its use is NOT recommended for new projects. 
 * Those looking to use this code should update it to WinSock 2. */

#include <Windows.h>
#include <winsock.h>
#include <stdlib.h>
#include <stdio.h>

#include "winirc.h"
#include "app.h"
#include "parser.h"
#include "handlers.h"
#include "log.h"
#include "ui.h"

extern Application App;

/* Return the user corresponding to a string */
IrcUser *findUserStr(char *name) {
	int i;
	IrcUser *ptr = App.users;

	for(i = 0; i < App.nUsers; i++) {
		if(!strcmp(ptr->username, name)) return ptr;
		ptr = ptr->next;		
	}

	return NULL;
}

/* Find user corresponding to socket. Returns NULL if no such
 * relation is found. */
IrcUser *findUserSock(SOCKET s) {
	int i;																																															
	IrcUser *ptr = App.users;
	BOOL foundSock = FALSE;

	if(!ptr) return NULL;

	for(i = 0; i < App.nUsers; i++) {
		if(ptr->sock == s) return ptr;
		ptr = ptr->next;
	}

	return NULL;
}

int queueMsg(IrcMsg *msg) {
	if(App.q.length == 0) {
	 	App.q.front = App.q.back = msg;
	} else {
		msg->next = App.q.front;
		App.q.front = msg;
	}
	App.q.length++;
	return 1;	
}

IrcMsg *dequeueMsg() {
	int i;
	IrcMsg *ptr = App.q.front;
	IrcMsg *back = App.q.back; /* Last element */

	if(!(App.q.front) || !(App.q.back) || App.q.length == 0)
		return NULL;

	if(App.q.length == 1) { 
		App.q.front = App.q.back = NULL;
		App.q.length = 0;
		return ptr;
	}

	for(i = 0; i < (App.q.length - 1); i++) {
		ptr = ptr->next;	
	}

	App.q.back = ptr;
	(App.q.back)->next = NULL;
	(App.q.length)--;
	return back;
}

void freeMsg(IrcMsg *msg) {
	free(msg->argv);
	free(msg->text);
	free(msg);
}

void destroyUser(IrcUser *user) {
	int i;
	IrcUser *ptr = App.users;

	if(!ptr || App.nUsers == 0) return;

	if(ptr == user) {
		App.users = ptr->next;
		free(ptr);
		return;
	}

	for(i = 0; i < (App.nUsers - 1); i++) {
		if(ptr->next == user) {
			ptr->next = ptr->next->next;
			free(user);
			break;
		}
		ptr = ptr->next;
	}

	App.nUsers--;
}

/* Procedure to handle accept() calls asynchronously. */
/* If successful, add a new connection to the connection list. */
LRESULT ircAccept() {
	int sockaddr_len;
	struct sockaddr_in clientAddr;
	SOCKET clientSock;

	sockaddr_len = sizeof(struct sockaddr_in);
	clientSock = accept(App.sock, (struct sockaddr *) &clientAddr,
						&sockaddr_len);
	if(clientSock == INVALID_SOCKET) return FALSE;

	WSAAsyncSelect(clientSock, App.hWnd, WM_CLIENTALERT, FD_READ | FD_CLOSE);
	return TRUE;
}

/* Queue a new message received from a user */
LRESULT ircRead(SOCKET sock) {
	char *text; 
	IrcUser *tmpUser;
	IrcMsg *tmpMsg;

	if(sock == INVALID_SOCKET) return FALSE;

	tmpMsg = calloc(1, sizeof(IrcMsg));
	if(tmpMsg == NULL) return FALSE;
	
	text = calloc(RECV_BUF_SIZE, sizeof(char));
	if(text == NULL) return FALSE;
	
	tmpMsg->text = text;
	recv(sock, tmpMsg->text, RECV_BUF_SIZE, 0);

	tmpUser = findUserSock(sock);
	/* In the case that this user is not registered, create a new
	 * IrcUser. */
	if(!tmpUser) {
		tmpUser = calloc(1, sizeof(IrcUser));
		if(!tmpUser) return FALSE;

		tmpUser->sock = sock;
		tmpUser->next = App.users;
		App.users = tmpUser;
		App.nUsers++;
	}


	tmpMsg->user = tmpUser;

	/* Populate user */
	if(ircParseMsg(tmpMsg) < 0) {
		freeMsg(tmpMsg);
		return FALSE;
	}

	queueMsg(tmpMsg);

	return TRUE;
}

IrcChannel *getChannel(char *name) {
	int i;
	IrcChannel *ptr = App.channels;

	for(i = 0; i < App.nChannels; i++) {
		if(!strcmp(ptr->name, name)) return ptr;
		ptr = ptr->next;
	}
	return NULL;
}

/* Close a connection to a user and rebuild all databases */
LRESULT ircClose(SOCKET s) {
	return closesocket(s);
}

/* Procedure for asynchronous processing of messages relating to the 
 * server's socket. */
LRESULT CALLBACK servProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(lParam)) {
		case(FD_ACCEPT):
			return ircAccept();
		case(FD_CLOSE):
			closesocket((SOCKET) wParam);
			return TRUE;
		default: return 0;
	}
}

/* Procedure for receiving information from users. Information is sent 
 * to the appropiate users from this function. */
LRESULT CALLBACK clientProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(lParam)) {
		case(FD_READ): return ircRead((SOCKET) wParam);
		case(FD_CLOSE): return ircClose((SOCKET) wParam);
		default: return 0;
	}
}

/* Run the subroutine assigned to the message. */
int ircDispatch(IrcMsg *msg) {
	int retVal;
	if(!msg) return FALSE;
	if(!(msg->handler)) return FALSE;
	
	retVal = msg->handler(msg);	
	if(retVal) writeToLog(msg);
	return retVal;
}

int initFail() {
	SendMessage(App.layout.controlStatus, WM_SETTEXT, 
			   (WPARAM) 0, (LPARAM) "disconnected");
	EnableWindow(App.layout.controlConnect, TRUE);
	return FALSE; 
}

/* Free all data structures */
int stopServer() {
	closesocket(App.sock);
	WSACleanup();
	initFail();
	return TRUE;
}

int initSuccess() {
	SendMessage(App.layout.controlStatus, WM_SETTEXT, 
			   (WPARAM) 0, (LPARAM) "connected");
	EnableWindow(App.layout.controlConnect, FALSE);
	return TRUE;	
}

/* Initialize the server. Return zero iff failure. */
int initServer() {
	static char host[128];
	static char portlabel[32];
	SOCKET ircSock;
	SOCKADDR_IN sin;
	WSADATA wsaData;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY; /* 0.0.0.0 */
	sin.sin_port = htons(App.config.port);

	if(WSAStartup(MAKEWORD(1, 2), &wsaData) != 0) return FALSE; 

	ircSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(ircSock == INVALID_SOCKET) return FALSE;

	if(bind(ircSock, (struct sockaddr *) &sin, sizeof(sin))
		== SOCKET_ERROR) {
		closesocket(ircSock);
		return initFail();
	}

	if(listen(ircSock, SOMAXCONN) == SOCKET_ERROR) {
		closesocket(ircSock);
		return initFail();
	}
						 
	WSAAsyncSelect(ircSock, App.hWnd, WM_SOCKALERT, FD_ACCEPT | FD_CLOSE);
	App.sock = ircSock;

	App.q.length = 0;
	App.q.front = NULL;
	App.q.back = App.q.front;

	ZeroMemory(App.log.text, LOG_SIZE);
	ZeroMemory(host, 128*sizeof(char));
	ZeroMemory(portlabel, 32*sizeof(char));
	sprintf(host, "Hostname: %s", App.config.hostname);
	sprintf(portlabel, "Port: %d", App.config.port);
	SendMessage(App.layout.hostnameLabel, WM_SETTEXT, 
				(WPARAM) 0, (LPARAM) host);
	SendMessage(App.layout.portLabel, WM_SETTEXT, 
				(WPARAM) 0, (LPARAM) portlabel);
	 
	return initSuccess();
}

