#include "../include/TextInputHandler.h"

#include <raylib.h>
#include <string>

#include <iostream>

std::string *TextInputHandler::inputPointer = nullptr;

void TextInputHandler::Update() {
    int key = GetCharPressed();

    while (key > 0)
    {
        if ((key >= 32) && (key <= 125))
        {
            if (inputPointer != nullptr) {
                (*inputPointer) += (char)key;
            }
        }

        key = GetCharPressed();
    }
    if (inputPointer != nullptr) {
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            if (!(*inputPointer).empty())
                (*inputPointer).pop_back();
        }
        if (IsKeyPressed(KEY_ENTER))
        {
            (*inputPointer).push_back('\n');
        }
    }
}