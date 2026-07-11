#ifndef _UI
#define _UI

#include <Windows.h>

/* Child window identifiers */
#define WINIRCLOGO       (HMENU)0x1000
#define HOSTNAMELABEL    (HMENU)0x1001
#define CHANNELLABEL     (HMENU)0x1002
#define	CHANNELLIST      (HMENU)0x1003
#define USERLABEL        (HMENU)0x1004
#define	USERLIST         (HMENU)0x1005
#define	STATUSBAR        (HMENU)0x1006
#define CONNECTIONSTATUS (HMENU)0x1007
#define VERSIONSTR		 (HMENU)0x1008
#define	SERVERBOX		 (HMENU)0x1009
#define UPTIMELABEL		 (HMENU)0x100A
#define CHANNELVIEW		 (HMENU)0x100B
#define CHANNELDELETE	 (HMENU)0x100C
#define CHANNELEDIT		 (HMENU)0x100D
#define USERINFO		 (HMENU)0x100E
#define USERDELETE		 (HMENU)0x100F
#define USEREDIT		 (HMENU)0x1010
#define CONTROLBOX		 (HMENU)0x1011
#define CONTROLCONNECT	 (HMENU)0x1012
#define CONTROLDISCONNECT (HMENU)0x1013
#define LOGBOX			 (HMENU)0x1014
#define LOGTEXT 		 (HMENU)0x1015
#define CONTROLLABEL	 (HMENU)0x1016
#define CONTROLSTATUS	 (HMENU)0x1017
#define CONTROLRESTART	 (HMENU)0x1018
#define CONTROLCONFIG	 (HMENU)0x1019

#define NAMESTR 	    TEXT("WinIRC v0.1")
#define CHANNELSTR 	    TEXT("Channels")
#define USERSTR 	    TEXT("Users")
#define CONNECTEDSTR    TEXT("Connected")
#define DISCONNECTEDSTR TEXT("Disconnected")

typedef struct uiLayout {
	HWND mainWnd;

	/* Branding */
	HWND winIrcLogo;

	/* Server section */
	HWND serverBox;
	HWND hostnameLabel;
	HWND uptimeLabel;
	
	/* Channel section */
	HWND channelLabel;
	HWND channelList;
	HWND channelView;
	HWND channelEdit;
	HWND channelDelete;
	
	/* Users section */
	HWND userLabel;
	HWND userList;
	HWND userAdd;
	HWND userEdit;
	HWND userDelete;

	/* Server controls */
	HWND controlBox;
	HWND controlConnect;
	HWND controlDisconnect;
	HWND controlLabel;
	HWND controlStatus;
	HWND controlRestart;
	HWND controlConfig;

	/* Log */
	HWND logBox;
	HWND logText;
} UiLayout;

typedef struct fonts {
	HFONT msSans8;   /* General GUI */
	HFONT msSans8B;	 /* Labels */
	HFONT arial28Bi; /* Logo */
	HFONT courier10; /* Log */
} Fonts;

/* Macros relating to positioning */
#define HMARGIN 16
#define VMARGIN 16
#define LEFTCOLW 192
#define PANELMARGIN 8
#define SERVPANELH	   120
#define CONTROLPANELH  120
#define CONTROLPANELX (HMARGIN)
#define CONTROLPANELY (HMARGIN + SERVPANELH + HMARGIN)
#define BUTTONW 64
#define BUTTONH 24

#define RIGHTCOLX (HMARGIN + LEFTCOLW + HMARGIN) 
#define CHANNELH 144

HWND initMainWnd(HINSTANCE, int);
LRESULT CALLBACK initUiLayout(HWND);
BOOL CALLBACK wndResize(HWND, UINT, WPARAM, LPARAM); /* WM_SIZE callback */

#endif
