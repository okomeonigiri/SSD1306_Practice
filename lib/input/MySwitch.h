#ifndef MYSWITCH_H
#define MYSWITCH_H

#include <Arduino.h>

class MySwitch {
public:
    void setup();
    int checkTactSwitch();
    bool checkToggleSwitch();
private:
    // #define TACT_LEFT 32
    // #define TACT_CENTER 31
    // #define TACT_RIGHT 34
    // #define TOGGLE_SWITCH 33
    // #define LAYER_SWITCH 30

    #define TACT_LEFT 24
    #define TACT_CENTER 26
    #define TACT_RIGHT 28
    #define TOGGLE_SWITCH 30
    #define LAYER_SWITCH 22
};
#endif // MYSWITCH_H