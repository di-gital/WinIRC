#ifndef _PARSER
#define _PARSER

#include <Windows.h>
#include <winsock.h>

#include "app.h"

#define MESSAGE_LENGTH 0x2000

/* Registration commands; RFC 2813 section 4.1 */
#define CMD_JOIN "JOIN"
#define CMD_MODE "MODE"
#define CMD_NICK "NICK"
#define CMD_NJOIN "NJOIN"
#define CMD_PASS "PASS"
#define CMD_SERVER "SERVER"
#define CMD_SERVICE "SERVICE"
#define CMD_QUIT "QUIT"
#define CMD_SQUIT "SQUIT"
#define CMD_USER "USER"
#define CMD_PRIVMSG "PRIVMSG"
#define CMD_TOPIC "TOPIC"

/* Implementation-defined messages. */
#define MSG_QUIT "Connection closed."

/* Functions */
int ircParseMsg(IrcMsg *); /* Mutate arg2, return code */

#endif
