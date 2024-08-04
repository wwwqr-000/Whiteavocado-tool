#pragma once

class text {
    private:
        std::string txt;
        RECT box;

    public:
        text() {}
        text(std::string txt, RECT box) : txt(txt), box(box) {}
        std::string& getTxt() { return this->txt; }
        RECT& getBox() { return this->box; }

};
