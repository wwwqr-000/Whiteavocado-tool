#pragma once

class frame {
    private:
        RECT box;
        std::string title;

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
        RECT getBox() { return this->box; }

        void setBX(int in) { box.left = in; }
        void setBY(int in) { box.top = in; }
        void setEX(int in) { box.right = in; }
        void setEY(int in) { box.bottom = in; }
};
