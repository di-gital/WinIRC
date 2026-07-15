#include <Windows.h>

#include "winirc.h"
#include "app.h"
#include "ui.h"

Application App;

static int messageLoop() {
	MSG msg;

	while(TRUE) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if(msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);	
		} 
		if(App.q.length > 0) { 
			ircDispatch(dequeueMsg());
		} else {
			WaitMessage();										 
		}
	}

	return (int) msg.wParam;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR nCmdLine, int nShowCmd) {
	HWND mainWnd;

	ZeroMemory(&App, sizeof(App));

	if((mainWnd = initMainWnd(hInstance, nShowCmd)) == 0) {
		MessageBox(NULL, "WinIRC failed to start.", "Error", MB_ICONSTOP);
		return 1;
	}

	/* Set global application information. */
	App.hInstance = hInstance;
	App.hWnd = mainWnd;

	if(!loadConf(TEXT(".\\winirc.ini"))) {
		loadDefaultConf();
		MessageBox(App.hWnd, "WinIRC failed to load configuration file." 	
	 						 " Using defaults.", "WinIRC", MB_ICONINFORMATION);
	}

	initServer();
	return messageLoop();
}
