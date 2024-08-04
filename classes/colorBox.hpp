#pragma once

class colorBox {
    private:
        point3 rgb;
        RECT box;

    public:
        colorBox() {}
        colorBox(point3 rgb, RECT box) : rgb(rgb), box(box) {}

        point3& getRGB() { return this->rgb; }
        RECT& getBox() { return this->box; }

};
