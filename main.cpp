#include <iostream>
#include <windows.h>
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <memory>
#include <functional>
#include <fstream>

#include "classes/json.hpp"
#include "classes/dimensions.hpp"
#include "classes/button.hpp"
#include "classes/text.hpp"
#include "classes/line.hpp"
#include "classes/colorBox.hpp"
#include "classes/frame.hpp"
#include "classes/page.hpp"

using json = nlohmann::json;

//Globals
std::vector<std::thread> threads;
frame windowFrame;//Active window frame value
bool update = true;
bool active = true;
bool validDragPos = false;//If the mouse drag is in the title bar of the window.
HMODULE WDll = LoadLibraryExW(L"whiteavocado64.dll", nullptr, 0);

//frame pages
std::vector<frame> pages;
frame homeFramePage;
frame infoFramePage;
//

//

std::string scc(std::string i) { return std::string(i); }//Convert 'as is' string to clean string

auto dpcn(std::string fName) {//DLL function process address from function name
    return GetProcAddress(WDll, fName.c_str());
}

void free() {
    FreeLibrary(WDll);
}

using DR = void (__cdecl*)(int, int, int, int, int, int, int, int, bool);
using MB = void (__cdecl*)(const char*, const char*, const char*, const char*, std::string&);
using CS = void (__cdecl*)();
using GCP = void (__cdecl*)(int&, int&);
using FR = void (__cdecl*)(int, int, int, int, int, int, int);
using DT = void (__cdecl*)(const char*, int, int, int, int, int, int, int, int, int, int, int);
using GSN = const char* (__cdecl*)();
using BL = const int (__cdecl*)(std::string&);
using BP = const void (__cdecl*)(const char*);

DR const drawLine = reinterpret_cast<DR>(dpcn("drawLine"));
MB const msgBox = reinterpret_cast<MB>(dpcn("msgBox"));
CS const cls = reinterpret_cast<CS>(dpcn("cls"));
FR const fillRect = reinterpret_cast<FR>(dpcn("fillRect"));
DT const drawTxt = reinterpret_cast<DT>(dpcn("drawTxt"));
GSN const getSelfName = reinterpret_cast<GSN>(dpcn("getSelfName"));
BL const buttonListener = reinterpret_cast<BL>(dpcn("buttonListener"));
GCP const getCursorPos = reinterpret_cast<GCP>(dpcn("getCursorPos"));
BP const beep = reinterpret_cast<BP>(dpcn("beep"));

void drawBox(RECT box, point3 rgb) {
    drawLine(box.left, box.top, box.right, box.top, 1, rgb.x_i, rgb.y_i, rgb.z_i, false);
    drawLine(box.left, box.top, box.left, box.bottom, 1, rgb.x_i, rgb.y_i, rgb.z_i, false);
    drawLine(box.right, box.top, box.right, box.bottom, 1, rgb.x_i, rgb.y_i, rgb.z_i, false);
    drawLine(box.left, box.bottom, box.right, box.bottom, 1, rgb.x_i, rgb.y_i, rgb.z_i, false);
}

void drawWindowFrame() {
    drawBox(windowFrame.getBox(), point3(0, 255, 0));
    drawTxt(windowFrame.getTitle().c_str(), windowFrame.getBX() + 2, windowFrame.getBY(), windowFrame.getEX(), windowFrame.getBY() + 30, 255, 0, 0, 0, 255, 0, 14);
    drawLine(windowFrame.getBX(), windowFrame.getBY() + 22, windowFrame.getEX(), windowFrame.getBY() + 22, 1, 0, 255, 0, false);
    //Closing icon
    drawLine(windowFrame.getEX() - 20, windowFrame.getBY(), windowFrame.getEX(), windowFrame.getBY() + 20, 1, 255, 0, 0, false);
    drawLine(windowFrame.getEX() - 20, windowFrame.getBY() + 20, windowFrame.getEX(), windowFrame.getBY(), 1, 255, 0, 0, false);
    //

    for (auto& txt : windowFrame.getTextElements()) {//Draw text elements of windowFrame
        point3 tRGB = txt.getTextColor();
        point3 bRGB = txt.getBackgroundColor();
        drawTxt(txt.getTxt().c_str(), txt.getBox().left, txt.getBox().top, txt.getBox().right, txt.getBox().bottom, tRGB.x_i, tRGB.y_i, tRGB.z_i, bRGB.x_i, bRGB.y_i, bRGB.z_i, txt.getFontSize());
    }
    for (auto& lineBox : windowFrame.getLineBoxes()) {//Draw a box frame
        drawBox(lineBox.getBox(), lineBox.getRGB());
    }
    for (auto& filledBox : windowFrame.getFilledBoxes()) {//Fill a box

    }
    for (auto& lineElement : windowFrame.getLineElements()) {//Draw a line
        drawLine(lineElement.getBegin().x_i, lineElement.getBegin().y_i, lineElement.getEnd().x_i, lineElement.getEnd().y_i, 1, lineElement.getRGB().x_i, lineElement.getRGB().y_i, lineElement.getRGB().z_i, false);
    }
}

void clearFramePageVectors(frame& in) {
    in.getButtons().clear();
    in.getTextElements().clear();
    in.getLineBoxes().clear();
    in.getFilledBoxes().clear();
    in.getLineElements().clear();
}

void infoPageSetup() {
    infoFramePage = windowFrame;
    clearFramePageVectors(infoFramePage);
    infoFramePage.setTitle("Info");

    //Buttons

    //

    //Text elements
    RECT testBox;
    testBox.left = infoFramePage.getBX() + 2;
    testBox.top = infoFramePage.getBY() + 50;
    testBox.right = infoFramePage.getBX() + 50;
    testBox.bottom = infoFramePage.getBY() + 70;
    text test("What is this tool?", testBox, point3(0, 0, 0), point3(0, 255, 0), 12);
    infoFramePage.getTextElements().emplace_back(test);
    //
}

void selectInfoFrame() {
    cls();
    infoPageSetup();
    windowFrame = infoFramePage;
    cls();
}

void homePageSetup() {
    homeFramePage = windowFrame;
    clearFramePageVectors(homeFramePage);
    homeFramePage.setTitle("Home");

    //Txt
    RECT wBox;
    wBox.left = homeFramePage.getBX() + 2;
    wBox.top = homeFramePage.getBY() + 50;
    wBox.right = homeFramePage.getEX();
    wBox.bottom = homeFramePage.getBY() + 100;
    text wTxt("Welcome to the whiteavocado tool!", wBox, point3(0, 0, 0), point3(0, 255, 0), 12);
    homeFramePage.getTextElements().emplace_back(wTxt);
    //

    //Line test
    int yBegin = 92;
    RECT lilBox;
    lilBox.left = homeFramePage.getBX() + 2;
    lilBox.top = homeFramePage.getBY() + yBegin;
    lilBox.right = homeFramePage.getEX() - 2;
    lilBox.bottom = homeFramePage.getBY() + yBegin + 50;
    colorBox testBox(point3(255, 255, 255), lilBox);
    homeFramePage.getLineBoxes().emplace_back(testBox);

    RECT redirInfoBox;
    redirInfoBox.left = homeFramePage.getBX() + 4;
    redirInfoBox.top = homeFramePage.getBY() + yBegin + 2;
    redirInfoBox.right = homeFramePage.getEX() - 4;
    redirInfoBox.bottom = homeFramePage.getBY() + yBegin + 48;
    button redirInfoPage(redirInfoBox, []() { selectInfoFrame(); });
    homeFramePage.getButtons().emplace_back(redirInfoPage);
    homeFramePage.getLineBoxes().emplace_back(colorBox(point3(0, 0, 0), redirInfoBox));
    //
}

void pageSetupLoader() {
    infoPageSetup();
    homePageSetup();
}

void drawScreen() {
    drawWindowFrame();
}

void buttonTrigger(int &x, int &y) {
    for (auto& button : windowFrame.getButtons()) {
        if (x >= button.getBox().left && x <= button.getBox().right && y >= button.getBox().top && y <= button.getBox().bottom) {
            button.click();
        }
    }
}

void close() {//The close callback for the whole application
    std::string msgResult;
    beep("normal");
    msgBox("Whiteavocado tool", "Are you sure you want to close whiteavocado tool?", "yn", "q", msgResult);
    if (msgResult != "yes") { return; }
    active = false;
    update = false;
    cls();
    Sleep(100);
    cls();
    Sleep(500);
    cls();
}

void createDefaultButtons() {
    RECT closeBox;
    closeBox.left = windowFrame.getEX() - 20;
    closeBox.right = windowFrame.getEX();
    closeBox.top = windowFrame.getBY();
    closeBox.bottom = windowFrame.getBY() + 20;
    button closeButton(closeBox, []() { close(); });
    windowFrame.getButtons().emplace_back(closeButton);
}

void windowFrameSetup() {
    createDefaultButtons();
}

void updatePositions(int &x, int &y) {
    windowFrame.setEX((windowFrame.getEX() - windowFrame.getBX()) + x);
    windowFrame.setEY((windowFrame.getEY() - windowFrame.getBY()) + y);
    windowFrame.setBX(x);
    windowFrame.setBY(y);

    clearFramePageVectors(windowFrame);

    if (windowFrame.getTitle() == "Info") {
        infoPageSetup();
        windowFrame = infoFramePage;
    }
    else if (windowFrame.getTitle() == "Home") {
        homePageSetup();
        windowFrame = homeFramePage;
    }

    createDefaultButtons();
}

void mouseBLThread() {//Mousebutton listener callback func
    while (active) {
        std::string stat;
        int index = buttonListener(stat);
        int x, y;
        getCursorPos(x, y);
        if (index == 0) {//Left button down
            buttonTrigger(x, y);
            if (x >= windowFrame.getBX() && x <= windowFrame.getEX() && y >= windowFrame.getBY() && y <= (windowFrame.getBY() + 22)) {
                validDragPos = true;
            }
        }
        else if (index == 3) {//Left button up
            if (validDragPos) {
                updatePositions(x, y);
                Sleep(20);
                cls();
            }
            validDragPos = false;
        }
    }
}

void tick() {
    while (active && update) {
        drawScreen();
    }
}

int gjpi(std::string i) {//scc + to int
    std::string out(i);
    return std::stoi(out);
}

bool loadPages() {
    std::ifstream active("pages/active.txt");
    if (!active.is_open()) { return false; }
    std::string line;

    while (std::getline(active, line)) {
        std::ifstream page("pages/" + line + ".json");
        if (!page.is_open()) {
            active.close();
            page.close();
            return false;
        }

        std::string content, pageLine;
        while (std::getline(page, pageLine)) {
            content += pageLine;
            content += '\n';
        }

        auto jp = json::parse(content);

        //Initialize page
        frame currentPage = windowFrame;

        currentPage.setTitle(std::string(jp["title"]));
        if (jp["usingLines"]) {
            for (int i = 0; i < jp["lines"].size(); i++) {
                //W.I.P
                //line sLine(point2(gjpi(jp["lines"][i]["begin"]["x"]), gjpi(jp["lines"][i]["begin"]["y"])), point2(gjpi(jp["lines"][i]["end"]["x"]), gjpi(jp["lines"][i]["end"]["y"])), point3(gjpi(jp["lines"][i]["rgb"]["r"]), gjpi(jp["lines"][i]["rgb"]["g"]), gjpi(jp["lines"][i]["rgb"]["b"])));
            }
        }

        if (jp["mainPage"]) {
            currentPage.getBox().left = std::stoi(std::string(jp["frame"]["x"]));
            currentPage.getBox().top = std::stoi(std::string(jp["frame"]["y"]));
            currentPage.getBox().right = std::stoi(std::string(jp["frame"]["width"]));
            currentPage.getBox().bottom = std::stoi(std::string(jp["frame"]["height"]));
            windowFrame = currentPage;
        }


        pages.emplace_back(currentPage);
        //

        page.close();
    }

    active.close();
    return true;
}

int main() {
    windowFrame = frame(10, 10, 710, 510, "Starting");
    windowFrameSetup();
    pageSetupLoader();
    //windowFrame = homeFramePage;
    if (!loadPages()) {
        std::cout << "Could not load pages\n";
    }
    threads.emplace_back([]{ mouseBLThread(); });
    while (active) {
        tick();
    }
    for (auto& thread : threads) {
        thread.join();
    }
    free();
    return 0;
}
