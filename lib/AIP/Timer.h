#pragma once

#include <Arduino.h>

class Timer {
    public:
        void reset();
        unsigned long read_milli(); //時間を取得(1/1000s)
        unsigned long set_milli(unsigned long ms);
        unsigned long read_micro(); //時間を取得(1/1000000s)
        void pause();
        void unpause();

    private:
        unsigned long milliseconds;
        unsigned long microseconds;
        bool is_paused;
        unsigned long pause_start;
};