#ifndef GENERAL_H
#define GENERAL_H
#include <Arduino.h>
#include "Timer.h"
#include "input.h"
#include "output.h"
class General
{
public:
    void setup();
    void startUp();
    int getMode();

private:
    int tact=0;
    bool toggle=0;
    int mode=0;
    int phase=0;
    Timer generalTimer;
    struct BUTTON_LIST{
        const char* first;
        const char* second;
        const char* third;
        const char* fourth;
        const char* fifth;
        const char* sixth;
    };
    BUTTON_LIST buttonList;
    bool pushedNext=false;
    int lastBuzzer=0;
};

#endif // GENERAL_H