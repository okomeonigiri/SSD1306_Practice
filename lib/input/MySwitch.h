#ifndef MYSWITCH_H
#define MYSWITCH_H

#include <Arduino.h>

class MySwitch {
public:
    void setup();
    void checkTactSwitch();
    void checkToggleSwitch();
private:
};
#endif // MYSWITCH_H