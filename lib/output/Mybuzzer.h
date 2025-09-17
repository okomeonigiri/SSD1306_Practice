#ifndef MYBUZZER_H
#define MYBUZZER_H
#include <Arduino.h>
class MyBUZZER {
    public:
        void setup();
        void start(int BUZZERnote, int BUZZERduration); //音程、持続時間を指定して音を鳴らす　持続時間を999にすると無制限
        void stop();
        void preset(int BUZZERpresetNUM); //既定の音を鳴らす

    private:
        const bool useBUZZER = 1;
        const byte BUZZER_PIN = 47;
};

#endif // MYBUZZER_H