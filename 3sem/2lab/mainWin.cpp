// mainWin.cpp
#include <windows.h>
#include "guiHistogram.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow) {
    return WinMainGUI(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}