#include <iostream>
#include <windows.h>
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <memory>
#include <functional>

#include "classes/json.hpp"
#include "classes/dimensions.hpp"
#include "classes/frameObj.hpp"
#include "classes/frames/box.hpp"
#include "classes/frames/menuItem.hpp"
#include "classes/frames/menu.hpp"

using json = nlohmann::json;

HMODULE WDll = LoadLibraryExW(L"whiteavocado64.dll", nullptr, 0);

using DR = void (__cdecl*)(int, int, int, int, int, int, int, int, bool);
using DP = void (__cdecl*)(int, int, int, int, int, bool);
using MB = void (__cdecl*)(const char*, const char*, const char*, const char*, std::string&);
using CS = void (__cdecl*)();

DR const drawLine = reinterpret_cast<DR>(GetProcAddress(WDll, "drawLine"));
DP const drawPixel = reinterpret_cast<DP>(GetProcAddress(WDll, "drawPixel"));
MB const msgBox = reinterpret_cast<MB>(GetProcAddress(WDll, "msgBox"));
CS const cls = reinterpret_cast<CS>(GetProcAddress(WDll, "cls"));

std::string msgRes;

int x;

void drawScreen() {
    drawLine(x, 100, x + 100, 100, 1, 255, 0, 0, false);
    x += 10;
}

void free() {
    FreeLibrary(WDll);
}

int main() {
    while (true) {
        Sleep(50);
        drawScreen();
        Sleep(50);
        cls();
    }
    free();
    return 0;
}
