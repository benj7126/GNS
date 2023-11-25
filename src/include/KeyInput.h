#pragma once

#include <vector>

#include <raylib.h>

class KeyInput{
private:
    static int curKey;
    static float updateCooldown;

public:
    static void UpdateKeyInput(){
        if (curKey != -1 && IsKeyReleased(curKey))
            curKey = -1;

        updateCooldown -= GetFrameTime();
    }

    static bool KeyActive(int keyIn){
        if (IsKeyPressed(keyIn)){
            updateCooldown = 0.4;
            curKey = keyIn;
            return true;
        }

        if (updateCooldown <= 0 && curKey == keyIn){
            updateCooldown = 0.04;
            return true;
        }

        return false;
    }
};