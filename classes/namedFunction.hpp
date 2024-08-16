#pragma once

class namedFunction {
    private:
        std::function<void()> callback;
        std::string name;

    public:
        namedFunction(std::function<void()> func, std::string name) : callback(func), name(name) {}
        void click() { callback(); }
        std::string& getName() { return this->name; }
        std::function<void()>& getCallback() { return this->callback; }
};
