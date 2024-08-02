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
#include "classes/frame.hpp"

using json = nlohmann::json;

//Globals
std::vector<std::thread> threads;
frame windowFrame;
bool update = true;
bool active = true;
HMODULE WDll = LoadLibraryExW(L"whiteavocado64.dll", nullptr, 0);
//

auto dpcn(std::string fName) {//DLL function process address from function name
    return GetProcAddress(WDll, fName.c_str());
}

void free() {
    FreeLibrary(WDll);
}

using DR = void (__cdecl*)(int, int, int, int, int, int, int, int, bool);
using DP = void (__cdecl*)(int, int, int, int, int, bool);
using MB = void (__cdecl*)(const char*, const char*, const char*, const char*, std::string&);
using CS = void (__cdecl*)();
using GR = void (__cdecl*)(int&, int&);
using FR = void (__cdecl*)(int, int, int, int, int, int, int);
using DT = void (__cdecl*)(const char*, int, int, int, int, int, int, int, int, int, int, int);
using GSN = const char* (__cdecl*)();

DR const drawLine = reinterpret_cast<DR>(dpcn("drawLine"));
DP const drawPixel = reinterpret_cast<DP>(dpcn("drawPixel"));
MB const msgBox = reinterpret_cast<MB>(dpcn("msgBox"));
CS const cls = reinterpret_cast<CS>(dpcn("cls"));
GR const getScreenResolution = reinterpret_cast<GR>(dpcn("getScreenResolution"));
FR const fillRect = reinterpret_cast<FR>(dpcn("fillRect"));
DT const drawTxt = reinterpret_cast<DT>(dpcn("drawTxt"));
GSN const getSelfName = reinterpret_cast<GSN>(dpcn("getSelfName"));

void drawBox(RECT box, point3 rgb) {
    drawLine(box.left, box.top, box.right, box.top, 1, rgb.x_i, rgb.y_i, rgb.z_i, false);
    drawLine(box.left, box.top, box.left, box.bottom, 1, rgb.x_i, rgb.y_i, rgb.z_i, false);
    drawLine(box.right, box.top, box.right, box.bottom, 1, rgb.x_i, rgb.y_i, rgb.z_i, false);
    drawLine(box.left, box.bottom, box.right, box.bottom, 1, rgb.x_i, rgb.y_i, rgb.z_i, false);
}

void drawWindowFrame() {
    drawBox(windowFrame.getBox(), point3(0, 255, 0));
    drawTxt(getSelfName(), windowFrame.getBX() + 2, windowFrame.getBY(), windowFrame.getEX(), windowFrame.getBY() + 30, 255, 0, 0, 0, 255, 0, 14);
    drawLine(windowFrame.getBX(), windowFrame.getBY() + 22, windowFrame.getEX(), windowFrame.getBY() + 22, 1, 0, 255, 0, false);
}

void drawScreen() {
    drawWindowFrame();
}

void tick() {
    while (active) {
        if (update) {
            //update = false;
            drawScreen();
        }
    }
}

int main() {
    windowFrame = frame(10, 10, 710, 510, "Tool");
    while (active) {
        tick();
    }
    free();
    return 0;
}
