#include <Windows.h>

#ifndef _CONF
#define _CONF

typedef struct appConfig {
	TCHAR file[255];
	TCHAR hostname[64];
	unsigned short port;
	int maxChannels;
	LPSTR logFile;
	LPSTR profileDb;
} AppConfig;

BOOL loadConf(LPSTR);
BOOL saveConf(void);
void loadDefaultConf(void);

#endif
