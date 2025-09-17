#ifndef MYSWITCH_H
#define MYSWITCH_H

#include <Arduino.h>

class MySwitch {
public:
    void setup();
    int checkTactSwitch();
    bool checkToggleSwitch();
private:
};
#endif // MYSWITCH_H