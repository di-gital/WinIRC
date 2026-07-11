#ifndef _HANDLERS
#define HANDLERS

#include <Windows.h>

#include "parser.h"

#define INVALID_USER TEXT("INVALID_USER")

int handleNick(IrcMsg *);
int handlePrivMsg(IrcMsg *);
int handlePass(IrcMsg *);
int handleJoin(IrcMsg *);
int handleUser(IrcMsg *);
int handleTopic(IrcMsg *);
int handleQuit(IrcMsg *);

/* RFC 2813-defined errors */

#define ERR_ALREADYREGISTRED 462
#define ERR_NICKNAMEINUSE 433
#define ERR_BADCHANMASK
#define ERR_BANNEDFROMCHAN
#define ERR_CHANNELISFULL
#define ERR_ERRONEUSNICKNAME
#define ERR_INVITEONLYCHAN
#define ERR_NEEDMOREPARAMS
#define ERR_NOSUCHCHANNEL
#define ERR_TOOMANYCHANNELS
#define ERR_TOOMANYTOPICS
#define ERR_UNAVAILRESOURCE

/* RFC 2813-defined replies */
#define RPL_WELCOME 1
#define RPL_YOURHOST 2
#define RPL_MYINFO
#define RPL_TOPIC
#define RPL_YOURSERVICE


#endif
