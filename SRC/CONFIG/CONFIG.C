#include <Windows.h>
#include <stdio.h>
#include <wchar.h>

#include "winirc.h"
#include "config.h"

extern Application App;

BOOL loadConf(LPSTR file) {
	strcpy(App.config.file, file);

	GetPrivateProfileString(TEXT("winirc"), TEXT("hostname"),
	  						TEXT("localhost"), App.config.hostname,
							64, file);
	App.config.port = (unsigned short) 
				  GetPrivateProfileInt(TEXT("winirc"), TEXT("port"),
									   6667, file);
	App.config.maxChannels = GetPrivateProfileInt(TEXT("winirc"), 
											  TEXT("max_channels"), 5, file); 

	return TRUE;
}

BOOL saveConf() {
	TCHAR buf[64];

	WritePrivateProfileString(TEXT("winirc"), TEXT("hostname"),
	  						 (LPCTSTR) App.config.hostname, App.config.file);

	sprintf(buf, "%u", App.config.port);
	WritePrivateProfileString(TEXT("winirc"), TEXT("port"),
								buf, App.config.file);

	sprintf(buf, "%d", App.config.maxChannels);
	WritePrivateProfileString(TEXT("winirc"), TEXT("max_channels"),
								buf, App.config.file);
	return TRUE;
}

void loadDefaultConf() {
	strcpy(App.config.hostname, "localhost");
	App.config.port = 6667;
	App.config.maxChannels = 5;
	App.config.logFile = "";
	App.config.profileDb = "";
}


