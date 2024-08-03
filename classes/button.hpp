#pragma once

class button {
    private:
        RECT box;
        std::function<void()> clickCallback;

    public:
        button() {}
        button(RECT box, std::function<void()> func) : clickCallback(func), box(box) {}
        void click() { clickCallback(); }
        RECT& getBox() { return this->box; }
};
