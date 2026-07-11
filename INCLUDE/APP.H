#ifndef _APP
#define _APP

#include <Windows.h>
#include <winsock.h>

#define RECV_BUF_SIZE 0x1000
#define SEND_BUF_SIZE RECV_BUF_SIZE
#define MSG_MAX_LEN 0x200
#define LOG_BUF_SIZE 0x1000

#define MAX_NICK     10
#define MAX_USERNAME 63
#define MAX_HOSTNAME 63
#define MAX_PASS	 63
#define MAX_CHANNEL  63
#define MAX_CHANNEL_USERS 64
#define MAX_TOPIC 64

typedef struct ircUser {
	SOCKET sock;
	BOOL registered;	/* Both NICK and USER complete successfully */
	char nick[MAX_NICK];
	char username[MAX_USERNAME];
	char realname[MAX_USERNAME];
	char server[MAX_HOSTNAME];
	char pass[MAX_PASS];
	struct ircUser *next;
} IrcUser;

typedef struct ircChannel {
	char name[MAX_CHANNEL];
	char topic[MAX_CHANNEL];
	int nUsers; 
	IrcUser *users[MAX_CHANNEL_USERS];
	struct ircChannel *next;
} IrcChannel;

char LogBuf[LOG_BUF_SIZE]; /* Buffer for log messages */

typedef struct logBuffer {
	int buf_size;
	char *buf;
	int bufPosition; /* mod LOG_BUF_SIZE */ 
	HANDLE logFile;
} LogBuffer;

typedef unsigned int IrcCmd;

#define ERROR 0
#define JOIN 1
#define MODE 2
#define NICK 3
#define NJOIN 4
#define PASS 5
#define SERVER 6
#define SERVICE 7
#define	QUIT 8
#define SQUIT 9
#define PRIVMSG 10
#define USER 11
#define TOPIC 12

/* Data structure to handle message, incoming or outgoing. */
typedef struct ircMsg {
	IrcUser *user;	  /* From whom */
	unsigned short port;
	IrcCmd cmd;
	int argc;		 /* Argument count */
	char **argv;	 /* List of arguments */
	char **flags;	
	char *text;      /* Pointer for the entire string sent by client */
	int (*handler)(struct ircMsg *);	/* Handler function */
	struct ircMsg *next;
} IrcMsg;

/* Message queue */
typedef struct msgQ {
	int length;
	IrcMsg *front;
	IrcMsg *back;		
} MsgQ;

/* Functions */
int initServer(void);
int stopServer(void);
int queueMsg();
IrcMsg *dequeueMsg(void);
int ircMessageLoop();
void freeMsg(IrcMsg *);
void destroyUser(IrcUser *);
LRESULT CALLBACK servProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK clientProc(HWND, UINT, WPARAM, LPARAM);
IrcChannel *getChannel(char *);
int ircDispatch(IrcMsg *);

/* Networking-related WndProc messages */
#define WM_SOCKALERT (WM_USER + 1)
#define WM_CLIENTALERT (WM_SOCKALERT + 1)

#endif
