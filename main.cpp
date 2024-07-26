#include <iostream>
#include <windows.h>
#include <vector>
#include <thread>
#include <iostream>
#include <memory>
#include <functional>

#include "classes/dimensions.hpp"
#include "classes/frameObj.hpp"
#include "classes/frames/box.hpp"
#include "classes/frames/menuItem.hpp"
#include "classes/frames/menu.hpp"

HMODULE WDll = LoadLibraryExW(L"whiteavocado64.dll", nullptr, 0);

using DR = void (__cdecl*)(int, int, int, int, int, int, int, int, bool);
using DP = void (__cdecl*)(int, int, int, int, int, bool);

DR const drawLine = reinterpret_cast<DR>(GetProcAddress(WDll, "drawLine"));
DP const drawPixel = reinterpret_cast<DP>(GetProcAddress(WDll, "drawPixel"));

void drawScreen() {
    drawLine(100, 100, 200, 100, 1, 255, 0, 0, false);
}

void free() {
    FreeLibrary(WDll);
}

int main() {
    while (true) {
        drawScreen();
    }
    free();
    return 0;
}
