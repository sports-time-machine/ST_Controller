#include "ofMain.h"
#include "testApp.h"


#include <Windows.h>

HANDLE _mutex;

static void ReleaseMutex()
{
    ReleaseMutex(_mutex);
    CloseHandle(_mutex);
}

static bool CheckAlreadyRunning(const char* ident_string)
{
    auto mutex = CreateMutexA(nullptr, TRUE, ident_string);
    atexit(ReleaseMutex);
    return (GetLastError() == ERROR_ALREADY_EXISTS);
}


//========================================================================
int main()
{
    if (CheckAlreadyRunning("stController"))
    {
        MessageBoxA(nullptr, "すでに起動しています", "スポーツタイムマシン コントローラー", MB_OK);
        return EXIT_FAILURE;
    }

    ofGLWindowSettings settings;
    settings.setSize(800, 400);
    settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

    auto window = ofCreateWindow(settings);

    ofRunApp(window, make_shared<testApp>());
    ofRunMainLoop();
}
