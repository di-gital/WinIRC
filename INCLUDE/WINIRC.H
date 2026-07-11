#ifndef _WINIRC
#define _WINIRC

#include <Windows.h>
#include <winsock.h>

#include "config.h"
#include "app.h"
#include "parser.h"
#include "ui.h"
#include "log.h"

#define NUSOCKS 0x200

typedef struct application {
	/* Presentation layer */
	HINSTANCE hInstance;
	HWND hWnd;
	UiLayout layout;
	Fonts fonts;

	/* Persistence layer */
	AppConfig config;
	HANDLE userDb;	     /* User data storage; INI file */
	int nUsers;
	int nConnectedUsers;
	IrcUser *users;
	int nChannels;
	IrcChannel *channels;

	/* Application layer */
	IrcLog log;
	MsgQ q;
	SOCKET sock;
} Application;

#endif

