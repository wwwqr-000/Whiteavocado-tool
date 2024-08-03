#pragma once

class button {
    private:
        RECT box;
        int BX, BY, EX, EY;
        std::function<void()> clickCallback;

    public:
        button(RECT box, std::function<void()> &func) : clickCallback(func), box(box) {}
        void click() { clickCallback(); }
};
