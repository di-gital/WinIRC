#include <Windows.h>

#include "winirc.h"
#include "ui.h"
#include "events.h"
#include "resource.h"

extern Application App;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
		case WM_CREATE:
			 initUiLayout(hWnd);
			 EnumChildWindows(hWnd, SetFontProc, (LPARAM) App.fonts.msSans8);
			 SendMessage(App.layout.winIrcLogo, WM_SETFONT, (WPARAM) App.fonts.arial28Bi, TRUE);
			 SendMessage(App.layout.channelLabel, WM_SETFONT, (WPARAM) App.fonts.msSans8B, TRUE);
			 SendMessage(App.layout.userLabel, WM_SETFONT, (WPARAM) App.fonts.msSans8B, TRUE);
			 SendMessage(App.layout.serverBox, WM_SETFONT, (WPARAM) App.fonts.msSans8B, TRUE);
			 SendMessage(App.layout.controlBox, WM_SETFONT, (WPARAM) App.fonts.msSans8B, TRUE);
			 SendMessage(App.layout.logBox, WM_SETFONT, (WPARAM) App.fonts.msSans8B, TRUE);
			 SendMessage(App.layout.controlStatus, WM_SETFONT, (WPARAM) App.fonts.msSans8B, TRUE);
		case WM_SIZE:
			return wndResize(hWnd, uMsg, wParam, lParam);
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_CTLCOLORSTATIC:
			 SetTextColor((HDC) wParam, GetSysColor(COLOR_WINDOWTEXT));
			 SetBkColor((HDC) wParam, GetSysColor(COLOR_BTNFACE));
			 return (LRESULT) CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
		case WM_SOCKALERT: 
			return servProc(hWnd, uMsg, wParam, lParam);
		case WM_CLIENTALERT:
			return clientProc(hWnd, uMsg, wParam, lParam);
		case WM_COMMAND:
			switch (LOWORD(wParam)) {								
				case IDR_FILE_EXIT:
					DestroyWindow(hWnd);
					return 0;
				case ID_HELP_ABOUT:
					DialogBox(App.hInstance, MAKEINTRESOURCE(IDD_ABOUT),
						  App.hWnd, aboutProc);
					return 0;
				case ID_SETTINGS_CONFIG:
					DialogBox(App.hInstance, MAKEINTRESOURCE(IDD_CONFIG),
						  App.hWnd, configProc);
					return 0;
				case IDR_WINIRC_LOG:
					DialogBox(App.hInstance, MAKEINTRESOURCE(IDD_WINIRC_LOG),
						  	  App.hWnd, logProc);
					return 0;
			}

			switch(HIWORD(wParam)) {
				case BN_CLICKED:
					buttonProc(hWnd, uMsg, wParam, lParam);
					return 0;
				case LBN_DBLCLK:
					if(LOWORD(wParam) == CHANNELLIST) {
						DialogBox(App.hInstance, 
								  MAKEINTRESOURCE(IDD_CHANEDIT),
								  App.hWnd, chanEditProc);
					} else if(LOWORD(wParam) == USERLIST) {
						DialogBox(App.hInstance,
								  MAKEINTRESOURCE(IDD_USEREDIT),
								  App.hWnd, userEditProc);
					}
					return 0;	
			}
			return 0;			
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
                                                                                    
ATOM registerWndClass(HINSTANCE hInstance) {
	WNDCLASS wc;

	ZeroMemory(&wc, sizeof(WNDCLASS));

	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "winirc";
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MAINMENU);

	return RegisterClass(&wc); 
}

HWND initMainWnd(HINSTANCE hInstance, int nShowCmd) {
	HWND hWnd;
	
	if(!registerWndClass(hInstance)) return 0;

	hWnd = CreateWindow("winirc", "WinIRC server",
				 WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
				 CW_USEDEFAULT, CW_USEDEFAULT,
				 CW_USEDEFAULT, CW_USEDEFAULT,
				 NULL, NULL,
				 hInstance, NULL);

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	return hWnd;
}
