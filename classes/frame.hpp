#pragma once

class frame {
    private:
        RECT box;
        std::string title;
        std::vector<button> buttons;

    public:
        frame() {}
        frame(int bX, int bY, int eX, int eY, std::string title) : title(title) {
            this->box.left = bX;
            this->box.top = bY;
            this->box.right = eX;
            this->box.bottom = eY;
        }

        int getBX() { return this->box.left; }
        int getBY() { return this->box.top; }
        int getEX() { return this->box.right; }
        int getEY() { return this->box.bottom; }
        std::string& getTitle() { return this->title; }
        RECT& getBox() { return this->box; }
        std::vector<button>& getButtons() { return this->buttons; }

        void setBX(int in) { box.left = in; }
        void setBY(int in) { box.top = in; }
        void setEX(int in) { box.right = in; }
        void setEY(int in) { box.bottom = in; }
        void setTitle(std::string in) { this->title = in; }
};
