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

using json = nlohmann::json;

std::vector<std::thread> threads;
bool update = true;
bool active = true;
int beginX = 100;
int beginY = 100;

HMODULE WDll = LoadLibraryExW(L"whiteavocado64.dll", nullptr, 0);

auto dpcn(std::string fName) {
    return GetProcAddress(WDll, fName.c_str());
}

using DR = void (__cdecl*)(int, int, int, int, int, int, int, int, bool);
using DP = void (__cdecl*)(int, int, int, int, int, bool);
using MB = void (__cdecl*)(const char*, const char*, const char*, const char*, std::string&);
using CS = void (__cdecl*)();

DR const drawLine = reinterpret_cast<DR>(dpcn("drawLine"));
DP const drawPixel = reinterpret_cast<DP>(dpcn("drawPixel"));
MB const msgBox = reinterpret_cast<MB>(dpcn("msgBox"));
CS const cls = reinterpret_cast<CS>(dpcn("cls"));

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
