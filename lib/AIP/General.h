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

    Timer generalTimer;
    bool pushedNext=false;
};

#endif // GENERAL_H