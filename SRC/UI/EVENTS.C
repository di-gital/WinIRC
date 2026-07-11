#include <Windows.h>

#include "winirc.h"
#include "app.h"
#include "config.h"
#include "resource.h"
#include "ui.h"

extern Application App;
extern AppConfig Config;

BOOL CALLBACK SetFontProc(HWND hWnd, LPARAM lParam) {
	SendMessage(hWnd, WM_SETFONT, (WPARAM) App.fonts.msSans8, TRUE);
	return TRUE;
}

BOOL CALLBACK aboutProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_INITDIALOG:
			EnumChildWindows(hWnd, SetFontProc, (WPARAM) App.fonts.msSans8);
			UpdateWindow(hWnd);

			return FALSE;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
				case IDCANCEL:
					EndDialog(hWnd, LOWORD(wParam));
					return TRUE;
			}
			return FALSE;
		case WM_CLOSE:
			EndDialog(hWnd, LOWORD(wParam));
			return TRUE;
		default:
			return FALSE;
	}
}

BOOL CALLBACK configProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_INITDIALOG:
			/* Fill in with values of AppConfig Config */
			EnumChildWindows(hWnd, SetFontProc, (LPARAM) App.fonts.msSans8);
			SetDlgItemText(hWnd, ID_HOSTNAME, Config.hostname);
			SetDlgItemInt(hWnd, ID_PORT, Config.port, FALSE);
			SetDlgItemInt(hWnd, ID_CHANNELS, Config.maxChannels, TRUE);
			SetDlgItemText(hWnd, ID_LOG_FILE, Config.logFile);
			SetDlgItemText(hWnd, ID_USERDB_SELECT, Config.profileDb);
								   
			return 0;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case ID_CONFIG_SAVE:
					GetDlgItemText(hWnd, ID_HOSTNAME, 
								  (LPTSTR) Config.hostname, 64);
					Config.port = GetDlgItemInt(hWnd, ID_PORT, NULL, FALSE);
					SendMessage(App.layout.hostnameLabel, WM_SETTEXT, 
								0, (LPARAM) Config.hostname);
					saveConf();
					MessageBox(hWnd, "Configuration saved. New settings "
									 "will take effect upon restart.",
									 "WinIRC", MB_OK);
				case ID_CONFIG_CANCEL:
					EndDialog(hWnd, LOWORD(wParam));
					return 0;
				default: return 0;
			}
		case WM_CLOSE:
			EndDialog(hWnd, LOWORD(wParam));
			return 0;
		default:
			return 0;
	}
}

BOOL CALLBACK logProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	RECT rect;
	PAINTSTRUCT ps;
	POINT pt;
	HWND child;

	switch(msg) {
		case WM_INITDIALOG:
			pt.x = pt.y = 0;
			child = ChildWindowFromPoint(hWnd, pt); 
			SendMessage(child, WM_SETFONT, (WPARAM) App.fonts.courier10, TRUE); 
			return TRUE;
		case WM_CLOSE:
			EndDialog(hWnd, LOWORD(wParam));
			return TRUE;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rect);
			SetDlgItemText(hWnd, ID_WINIRC_LOG_TEXT, App.log.text);
			EndPaint(hWnd, &ps); 
			return 0;
				
		default: 
			return 0;
	}
}

BOOL CALLBACK chanEditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch(LOWORD(wParam)) {
		case ID_USEREDIT_SAVE:
		case ID_USEREDIT_CANCEL:
			EndDialog(hWnd, LOWORD(wParam));
			return 0;	
	}
	
	switch(msg) {
		case WM_INITDIALOG: 
			EnumChildWindows(hWnd, SetFontProc, (WPARAM) App.fonts.msSans8);
			return 0;
		case WM_CLOSE: EndDialog(hWnd, LOWORD(wParam));
		default: return 0;
	}
}

BOOL CALLBACK userEditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(wParam)) {
		case ID_USEREDIT_SAVE:
		case ID_USEREDIT_CANCEL:
			EndDialog(hWnd, LOWORD(wParam));
			return 0;	
	}
	
	switch(msg) {
		case WM_INITDIALOG: 
			EnumChildWindows(hWnd, SetFontProc, (WPARAM) App.fonts.msSans8);
			return 0;
		case WM_CLOSE: EndDialog(hWnd, LOWORD(wParam));
		default: return 0;
	}
}

BOOL CALLBACK userListProc(HWND hWnd, UINT msg,
						   WPARAM wParam, LPARAM lParam) {
	int i;
	IrcUser *ptr = App.users;

	if(!ptr || App.nUsers == 0) return FALSE;
	
	for(i = 0; i < App.nUsers; i++) {
		SendMessage(hWnd, LB_ADDSTRING, (WPARAM) NULL, 
										(LPARAM) ptr->username);
		ptr = ptr->next;
	}	

	return TRUE;
}

/* WM_CREATE, WM_SIZE */								  
BOOL CALLBACK wndResize(HWND hWnd, UINT msg,
						WPARAM wParam, LPARAM lParam) {
	RECT windowRect;
	int height, rightColW;

	if(msg != WM_SIZE) {
		rightColW = 576 - 3*HMARGIN - LEFTCOLW;
		height = 384;
	} else {
		rightColW = LOWORD(lParam) - 3*HMARGIN - LEFTCOLW;
		height = LOWORD(lParam);
	}

	/* Divide the window into two tabs */
	GetWindowRect(App.hWnd, &windowRect);

	SetWindowPos(App.layout.serverBox, NULL, HMARGIN, VMARGIN,
				 LEFTCOLW, SERVPANELH,
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	
	/* Control panel */
	SetWindowPos(App.layout.controlBox, NULL, CONTROLPANELX, CONTROLPANELY, 
				 LEFTCOLW, CONTROLPANELH, 
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	SetWindowPos(App.layout.controlLabel, NULL, 
				 HMARGIN + PANELMARGIN, 
				 CONTROLPANELY + HMARGIN + PANELMARGIN,
				 78, 12,
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
   	SetWindowPos(App.layout.controlStatus, NULL, 
				 HMARGIN + PANELMARGIN + 68, 
				 CONTROLPANELY + HMARGIN + PANELMARGIN,
				 80, 12,
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	SetWindowPos(App.layout.controlConnect, NULL, 
			     HMARGIN + (LEFTCOLW - 2*72)/2, 
				 CONTROLPANELY + 3*HMARGIN,
				 72, 24, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	SetWindowPos(App.layout.controlDisconnect, NULL, 
			     HMARGIN + LEFTCOLW - (LEFTCOLW - 2*72)/2 - 72, 
				 CONTROLPANELY + 3*HMARGIN,
				 72, 24, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	SetWindowPos(App.layout.controlRestart, NULL, 
			     HMARGIN + (LEFTCOLW - 2*72)/2, 
				 CONTROLPANELY + 3*HMARGIN + PANELMARGIN + BUTTONH,
				 72, 24, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	SetWindowPos(App.layout.controlConfig, NULL, 
			     HMARGIN + LEFTCOLW - (LEFTCOLW - 2*72)/2 - 72, 
				 CONTROLPANELY + 3*HMARGIN + PANELMARGIN + BUTTONH,
				 72, 24, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);				 					  
	/* Place WinIRC logo */
	SetWindowPos(App.layout.winIrcLogo, NULL,
				 HMARGIN, 3*VMARGIN + CONTROLPANELH + SERVPANELH,
				 LEFTCOLW,
				 height - 4*VMARGIN + CONTROLPANELH + SERVPANELH,
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW); 

	/* Channels panel */
	SetWindowPos(App.layout.channelLabel, NULL,
				 RIGHTCOLX, VMARGIN,
				 rightColW, CHANNELH,
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	SetWindowPos(App.layout.channelList, NULL,
				 RIGHTCOLX + HMARGIN,
				 2*VMARGIN + PANELMARGIN,
				 rightColW - 2*HMARGIN - 72, 
				 CHANNELH - 2*VMARGIN, 
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	SetWindowPos(App.layout.channelView, NULL,
				 RIGHTCOLX + HMARGIN + (rightColW - 2*HMARGIN - 72),
				 2*VMARGIN + PANELMARGIN,
				 BUTTONW, (CHANNELH - 2*VMARGIN)/3, 
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	SetWindowPos(App.layout.channelEdit, NULL,
				 RIGHTCOLX + HMARGIN + (rightColW - 2*HMARGIN - 72),
				 2*VMARGIN + PANELMARGIN + (CHANNELH - 2*VMARGIN)/3,
				 BUTTONW, (CHANNELH - 2*VMARGIN)/3, 
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	SetWindowPos(App.layout.channelDelete, NULL,
				 RIGHTCOLX + HMARGIN + (rightColW - 2*HMARGIN - 72),
				 2*VMARGIN + PANELMARGIN + 2*(CHANNELH - 2*VMARGIN)/3,
				 BUTTONW, (CHANNELH - 2*VMARGIN)/3, 
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	/* Users panel */
	SetWindowPos(App.layout.userLabel, NULL,
				 RIGHTCOLX, VMARGIN + VMARGIN + CHANNELH,
				 rightColW, CHANNELH,
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	SetWindowPos(App.layout.userList, NULL,
				 RIGHTCOLX + HMARGIN,
				 3*VMARGIN + CHANNELH + PANELMARGIN,
				 rightColW - 2*HMARGIN - 72, 
				 CHANNELH - 2*VMARGIN, 
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	SetWindowPos(App.layout.userAdd, NULL,
				 RIGHTCOLX + HMARGIN + (rightColW - 2*HMARGIN - 72),
				 3*VMARGIN + CHANNELH + PANELMARGIN,
				 BUTTONW, (CHANNELH - 2*VMARGIN)/3, 
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	SetWindowPos(App.layout.userEdit, NULL,
				 RIGHTCOLX + HMARGIN + (rightColW - 2*HMARGIN - 72),
				 3*VMARGIN + CHANNELH + PANELMARGIN + (CHANNELH - 2*VMARGIN)/3,
				 BUTTONW, (CHANNELH - 2*VMARGIN)/3, 
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	SetWindowPos(App.layout.userDelete, NULL,
				 RIGHTCOLX + HMARGIN + (rightColW - 2*HMARGIN - 72),
				 3*VMARGIN + CHANNELH + PANELMARGIN + 2*(CHANNELH - 2*VMARGIN)/3,
				 BUTTONW, (CHANNELH - 2*VMARGIN)/3, 
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
				 	
	/* Log panel */
	SetWindowPos(App.layout.logBox, NULL,
				 RIGHTCOLX, 3*VMARGIN + 2*CHANNELH,
				 rightColW, CHANNELH,
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	SetWindowPos(App.layout.logText, NULL,
				 RIGHTCOLX + HMARGIN,
				 4*VMARGIN + 2*CHANNELH + PANELMARGIN,
				 rightColW - 2*HMARGIN, 
				 CHANNELH - 2*VMARGIN, 
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);

	InvalidateRect(App.hWnd, NULL, TRUE);
	UpdateWindow(App.hWnd);

	return FALSE;						
}								  								  

/* Function to dispatch tasks to the application layer. */																  
BOOL CALLBACK buttonProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(wParam)) {
			case (int) CONTROLCONNECT:
				initServer();
				return TRUE;
			case (int) CONTROLDISCONNECT:
				stopServer();
				return TRUE;
			case (int) CONTROLRESTART:
				stopServer();
				initServer();
				return TRUE;
			case (int) CONTROLCONFIG:
				DialogBox(App.hInstance, MAKEINTRESOURCE(IDD_CONFIG),
						  App.hWnd, configProc);
				return TRUE;	
			case (int) USERINFO:
			case (int) USEREDIT:
			case (int) USERDELETE:
			case (int) CHANNELVIEW:
			case (int) CHANNELEDIT:
			case (int) CHANNELDELETE:
				MessageBox(App.hWnd, "Unimplemented", 
						   "WinIRC", MB_ICONEXCLAMATION); 
		default: return 0;
	}
}
