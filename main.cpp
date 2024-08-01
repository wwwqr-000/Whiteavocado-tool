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

HMODULE WDll = LoadLibraryExW(L"whiteavocado64.dll", nullptr, 0);

struct windowFrame {
    int beginX, beginY, endX, endY;
    std::string title;
    bool refresh;
};

windowFrame frame;

auto dpcn(std::string fName) {//DLL function process addres from function name
    return GetProcAddress(WDll, fName.c_str());
}

void free() {
    FreeLibrary(WDll);
}

using DR = void (__cdecl*)(int, int, int, int, int, int, int, int, bool);
using DP = void (__cdecl*)(int, int, int, int, int, bool);
using MB = void (__cdecl*)(const char*, const char*, const char*, const char*, std::string&);
using CS = void (__cdecl*)();

DR const drawLine = reinterpret_cast<DR>(dpcn("drawLine"));
DP const drawPixel = reinterpret_cast<DP>(dpcn("drawPixel"));
MB const msgBox = reinterpret_cast<MB>(dpcn("msgBox"));
CS const cls = reinterpret_cast<CS>(dpcn("cls"));

void drawWindowFrame() {
    drawLine(10, 100, 10 + 100, 100, 1, 255, 0, 0, false);
}

void drawScreen() {
    drawWindowFrame();
}

void tick() {
    while (active) {
        if (update) {
            update = false;
            drawScreen();
        }
    }
}

int main() {
    while (active) {
        tick();
    }
    free();
    return 0;
}
