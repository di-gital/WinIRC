#include <Windows.h>

#include "winirc.h"
#include "config.h"
#include "ui.h"
#include "events.h"

extern Application App;

/* Set fonts for all child windows. */
void createFont() {
	HFONT msSans8, msSans8B, arial28, courier10;
	arial28 = CreateFont(-28, 0, 0, 0,
						  FW_BOLD,
						  TRUE, FALSE, FALSE,
						  ANSI_CHARSET,
						  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						  DEFAULT_QUALITY,
						  DEFAULT_PITCH | FF_SWISS,
						  "Arial");
	msSans8 =  CreateFont(-8, 0, 0, 0,
						  FW_NORMAL,
						  FALSE, FALSE, FALSE,
					      ANSI_CHARSET,
						  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						  DEFAULT_QUALITY,
						  DEFAULT_PITCH | FF_SWISS,
						  "MS Sans Serif");
	msSans8B =  CreateFont(-8, 0, 0, 0,
						  FW_BOLD,
						  FALSE, FALSE, FALSE,
					      ANSI_CHARSET,
						  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						  DEFAULT_QUALITY,
						  DEFAULT_PITCH | FF_SWISS,
						  "MS Sans Serif");
	courier10 = CreateFont(-10, 0, 0, 0,
						   FW_NORMAL,
						   FALSE, FALSE, FALSE,
						   ANSI_CHARSET,
						   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						   DEFAULT_QUALITY,
						   FIXED_PITCH | FF_DONTCARE,
						   "Courier");
						   

	App.fonts.msSans8 = msSans8;
	App.fonts.msSans8B = msSans8B;
	App.fonts.courier10 = courier10;
	App.fonts.arial28Bi = arial28;
}

/* Create all child controls for the main window. This function is
 * intended to be called only from WndProc(), message WM_CREATE. */
LRESULT CALLBACK initUiLayout(HWND hWnd) {
	if(hWnd == NULL) return 0;

	/* Branding */
	App.layout.winIrcLogo = CreateWindow("STATIC", NAMESTR, 
								WS_CHILD | WS_VISIBLE | SS_CENTER,
								0, 0, 0, 0,
								hWnd, WINIRCLOGO, App.hInstance, NULL);
								
	/* Server panel */ 
	App.layout.serverBox = CreateWindow("BUTTON", "Server",
									  WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
									  0, 0, 0, 0,
									  hWnd, SERVERBOX, App.hInstance, NULL);

	App.layout.hostnameLabel = CreateWindow("STATIC", "Hostname:",
									  WS_CHILD | WS_VISIBLE,
									  0, 0, 0, 0,
									  hWnd, HOSTNAMELABEL, App.hInstance, NULL);
	App.layout.portLabel = CreateWindow("STATIC", "Port: 6667",
									  WS_CHILD | WS_VISIBLE,
									  0, 0, 0, 0,
									  hWnd, PORTLABEL, App.hInstance, NULL);
	App.layout.uptimeLabel = CreateWindow("STATIC", "Uptime: 0 hr, 0 min, 0 sec",
									  WS_CHILD | WS_VISIBLE,
									  0, 0, 0, 0,
									  hWnd, UPTIMELABEL, App.hInstance, NULL);


	/* Channel panel */
	App.layout.channelLabel = CreateWindow("BUTTON", "Channels (0)",
									  WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
									  0, 0, 0, 0,
									  hWnd, CHANNELLABEL, App.hInstance, NULL);
	App.layout.channelList = CreateWindow("LISTBOX", NULL,
							    WS_CHILD | WS_VISIBLE 
							    | WS_VSCROLL | WS_HSCROLL
							    | LBS_DISABLENOSCROLL | LBS_STANDARD
							    | LBS_MULTICOLUMN,
								0, 0, 0, 0,
								hWnd, CHANNELLIST, App.hInstance, NULL);
 	App.layout.channelView = CreateWindow("BUTTON", "View...",
									  WS_CHILD | WS_VISIBLE,
									  0, 0, 0, 0,
									  hWnd, CHANNELVIEW, App.hInstance, NULL);
	App.layout.channelEdit = CreateWindow("BUTTON", "Edit...",
									  WS_CHILD | WS_VISIBLE,
									  0, 0, 0, 0,
									  hWnd, CHANNELEDIT, App.hInstance, NULL);
	App.layout.channelDelete = CreateWindow("BUTTON", "Delete",
									  WS_CHILD | WS_VISIBLE,
									  0, 0, 0, 0,
									  hWnd, CHANNELDELETE, App.hInstance, NULL);

	/* User panel */
	App.layout.userLabel = CreateWindow("BUTTON", "Users (0)",
									  WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
									  0, 0, 0, 0,
									  hWnd, USERLABEL, App.hInstance, NULL);
	App.layout.userList = CreateWindow("LISTBOX", NULL,
								WS_CHILD | WS_VISIBLE 
								| WS_VSCROLL | WS_HSCROLL
								| LBS_DISABLENOSCROLL | LBS_STANDARD
								| LBS_MULTICOLUMN,
								0, 0, 0, 0,
								hWnd, USERLIST, App.hInstance, NULL);
 	App.layout.userAdd = CreateWindow("BUTTON", "Add..",
									  WS_CHILD | WS_VISIBLE,
									  0, 0, 0, 0,
									  hWnd, USERINFO, App.hInstance, NULL);
 	App.layout.userEdit = CreateWindow("BUTTON", "Edit...",
									  WS_CHILD | WS_VISIBLE,
									  0, 0, 0, 0,
									  hWnd, USEREDIT, App.hInstance, NULL);
 	App.layout.userDelete = CreateWindow("BUTTON", "Delete",
									  WS_CHILD | WS_VISIBLE,
									  0, 0, 0, 0,
									  hWnd, USERDELETE, App.hInstance, NULL);

	/* Control panel */
	App.layout.controlBox =  CreateWindow("BUTTON", "Control",
									  WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
									  0, 0, 0, 0,
									  hWnd, CONTROLBOX, App.hInstance, NULL);
	App.layout.controlConnect = CreateWindow("BUTTON", "Connect",
									  WS_CHILD | WS_VISIBLE, 
									  0, 0, 0, 0,
									  hWnd, CONTROLCONNECT, App.hInstance, NULL);
	App.layout.controlDisconnect = CreateWindow("BUTTON", "Disconnect",
									  WS_CHILD | WS_VISIBLE, 
									  0, 0, 0, 0,
									  hWnd, CONTROLDISCONNECT, App.hInstance, NULL);
	App.layout.controlRestart = CreateWindow("BUTTON", "Restart",
									  WS_CHILD | WS_VISIBLE, 
									  0, 0, 0, 0,
									  hWnd, CONTROLRESTART, App.hInstance, NULL);
	App.layout.controlConfig = CreateWindow("BUTTON", "Configure",
									  WS_CHILD | WS_VISIBLE, 
									  0, 0, 0, 0,
									  hWnd, CONTROLCONFIG, App.hInstance, NULL);
	App.layout.controlLabel = CreateWindow("STATIC", "Server status: ",
									  WS_CHILD | WS_VISIBLE,
									  0, 0, 0, 0,
									  hWnd, CONTROLLABEL, App.hInstance, NULL);
	App.layout.controlStatus = CreateWindow("STATIC", "disconnected",
									  WS_CHILD | WS_VISIBLE,
									  0, 0, 0, 0,
									  hWnd, CONTROLSTATUS, App.hInstance, NULL);

	/* Log panel */
	App.layout.logBox = CreateWindow("BUTTON", "Log",
									  WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
									  0, 0, 0, 0,
									  hWnd, CONTROLSTATUS, App.hInstance, NULL);
	App.layout.logText = CreateWindow("LISTBOX", NULL,
									  WS_CHILD | WS_VISIBLE
									| WS_VSCROLL | WS_HSCROLL
									| LBS_DISABLENOSCROLL | LBS_STANDARD,
									  0, 0, 0, 0,
									  hWnd, LOGTEXT, App.hInstance, NULL);

	createFont(); /* Set fonts on all created child windows. */										  
	return 0;
}

