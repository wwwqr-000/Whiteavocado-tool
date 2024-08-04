#pragma once

class text {
    private:
        std::string txt;
        RECT box;
        point3 tRGB, bRGB;//Text and Background RGB
        int fontSize;

    public:
        text() {}
        text(std::string txt, RECT box, point3 tRGB, point3 bRGB, int fontSize) : txt(txt), box(box), tRGB(tRGB), bRGB(bRGB), fontSize(fontSize) {}
        std::string& getTxt() { return this->txt; }
        RECT& getBox() { return this->box; }
        point3& getTextColor() { return this->tRGB; }
        point3& getBackgroundColor() { return this->bRGB; }
        int& getFontSize() { return this->fontSize; }

};
