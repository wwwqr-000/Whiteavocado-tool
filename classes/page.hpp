#pragma once

class page {
    private:
        std::string name;
        frame frame_;

    public:
        page() {}
        page (std::string name, frame& fIn) : name(name), frame_(fIn) {}
        page (std::string name, frame fIn) : name(name), frame_(fIn) {}
        std::string& getName() { return this->name; }
        frame& getFrame() { return this->frame_; }
};
