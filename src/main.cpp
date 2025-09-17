#include <Arduino.h>
#include "output.h"
#include "input.h"
#include "AIP.h"

void setup() {
	general.setup();
}

void loop() {
	myDisplay.clearDisplay();
	general.startUp(); // 起動時の処理
	
	myDisplay.updateDisplay();
}

