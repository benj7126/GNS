#pragma once

#include <string>

class TextInputHandler {
private:
    static std::string* inputPointer;

public:
    static void LinkForInput(std::string* element) {
        inputPointer = element;
    };

    static void Update();
};