#include <Windows.h>
#include <stdio.h>
#include <wchar.h>

#include "config.h"

extern AppConfig Config;

BOOL loadConf(LPSTR file) {
	strcpy(Config.file, file);

	GetPrivateProfileString(TEXT("winirc"), TEXT("hostname"),
	  						TEXT("localhost"), (LPTSTR) &(Config.hostname),
							64, file);
	Config.port = (unsigned short) 
				  GetPrivateProfileInt(TEXT("winirc"), TEXT("port"),
									   6667, file);
	Config.maxChannels = GetPrivateProfileInt(TEXT("winirc"), 
											  TEXT("max_channels"), 5, file); 

	return TRUE;
}

BOOL saveConf() {
	TCHAR buf[64];

	WritePrivateProfileString(TEXT("winirc"), TEXT("hostname"),
	  						 (LPCTSTR) &(Config.hostname), Config.file);

	sprintf(buf, "%u", Config.port);
	WritePrivateProfileString(TEXT("winirc"), TEXT("port"),
								buf, Config.file);

	sprintf(buf, "%d", Config.maxChannels);
	WritePrivateProfileString(TEXT("winirc"), TEXT("max_channels"),
								buf, Config.file);
	return TRUE;
}

void loadDefaultConf() {
	strcpy(Config.hostname, "localhost");
	Config.port = 6667;
	Config.maxChannels = 5;
	Config.logFile = "";
	Config.profileDb = "";
}


