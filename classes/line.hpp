#pragma once

class line {
    private:
        point2 b, e;//Begin and End coordinates
        point3 rgb;

    public:
        line() {}
        line(point2 b, point2 e, point3 rgb) : b(b), e(e), rgb(rgb) {}
        point2& getBegin() { return this->b; }
        point2& getEnd() { return this->e; }
        point3& getRGB() { return this->rgb; }

};
