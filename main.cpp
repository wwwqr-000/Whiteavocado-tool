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

std::vector<std::thread> threads;
bool update = true;
bool active = true;
int beginX = 100;
int beginY = 100;

HMODULE WDll = LoadLibraryExW(L"whiteavocado64.dll", nullptr, 0);

using DR = void (__cdecl*)(int, int, int, int, int, int, int, int, bool);
using DP = void (__cdecl*)(int, int, int, int, int, bool);
using MB = void (__cdecl*)(const char*, const char*, const char*, const char*, std::string&);
using CS = void (__cdecl*)();

DR const drawLine = reinterpret_cast<DR>(GetProcAddress(WDll, "drawLine"));
DP const drawPixel = reinterpret_cast<DP>(GetProcAddress(WDll, "drawPixel"));
MB const msgBox = reinterpret_cast<MB>(GetProcAddress(WDll, "msgBox"));
CS const cls = reinterpret_cast<CS>(GetProcAddress(WDll, "cls"));

void drawScreen() {
    drawLine(10, 100, 10 + 100, 100, 1, 255, 0, 0, false);
}

void tick() {
    while (active) {
        if (update) {
            update = false;
            drawScreen();
        }
    }
}

void free() {
    FreeLibrary(WDll);
}

int main() {
    while (active) {
        tick();
    }
    free();
    return 0;
}
