#include <Arduino.h>

#ifndef DEFENSE_H
#define DEFENSE_H
class Defense{
    public:
    String keys[4]{"speed","test","temp1","value"};
    float value[4]{1,2,3,4};
    void defense_();
    private:
};
#endif //DEFENSE_H