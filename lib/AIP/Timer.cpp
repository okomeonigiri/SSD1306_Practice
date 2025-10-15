#include "Timer.h"
#include <Arduino.h>


void Timer::reset(){
	milliseconds = millis();
	microseconds = micros();
}

unsigned long Timer::read_milli(){
	return (millis() - milliseconds);
}

unsigned long Timer::set_milli(unsigned long ms){
	milliseconds = millis() - ms;
	return milliseconds;
}

unsigned long Timer::read_micro(){
	return (micros() - microseconds);
}