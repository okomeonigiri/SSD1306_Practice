#ifndef GENERAL_H
#define GENERAL_H
#include <Arduino.h>
#include "Timer.h"
#include "input.h"
#include "output.h"
/*
設定の配列テンプレート
valuesにそのまま格納しますので配列から参照してくださいね
キャッシュしておけば配列呼び出しのコストを減らせますよ

String keys[]{
}

float values[]{
}

*/
class General{
public:
    void setup();
    void startUp();
    int getMode();
        inline void setMode(int m) { mode = m; }
        inline void setPhase(int p) { phase = p; }
private:
    void ModeSelect();
    void StandBy();
    void Setting();
        void Edit(char* key, float* value);
    inline void control_guide(bool brack = false);
    inline void WaitReleaseTact();
    int mode=0;
    int phase=0;
};

#endif // GENERAL_H