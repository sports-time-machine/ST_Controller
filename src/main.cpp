#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"


#include <Windows.h>

HANDLE _mutex;

static void ReleaseMutex()
{
	ReleaseMutex(_mutex);
	CloseHandle(_mutex);
}

static bool CheckAlreadyRunning(const char* ident_string)
{
	auto mutex = CreateMutexA(NULL, TRUE, ident_string);
	atexit(ReleaseMutex);
	return (GetLastError()==ERROR_ALREADY_EXISTS);
}



//========================================================================
int main( ){
	if (CheckAlreadyRunning("stController"))
	{
		MessageBoxA(NULL, "すでに起動しています", "スポーツタイムマシン コントローラー", MB_OK);
		return EXIT_FAILURE;
	}


    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 800,400, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
