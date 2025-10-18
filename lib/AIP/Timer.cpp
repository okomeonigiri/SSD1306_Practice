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

void Timer::pause(){
	if (!is_paused) {
		pause_start = millis();
		is_paused = true;
	}
}

unsigned long Timer::read_micro(){
	return (micros() - microseconds);
}

void Timer::unpause(){
	if (is_paused) {
		unsigned long paused_duration = millis() - pause_start;
		milliseconds += paused_duration;
		microseconds += paused_duration * 1000;
		is_paused = false;
	}
}