#pragma once

#include <memory>

#include "Element.h"

#include "TextField.h"

class ElementTypes {
public:
    enum ElemntType {
        Button,
        TextField,
    };

    static std::shared_ptr<Element> GetElementPtr(ElemntType element){
        switch(element){
            case(TextField):
                return std::make_shared<Elements::TextField>();
        }

        return nullptr;
    }
};

