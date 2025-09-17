#include <Arduino.h>
#include "output.h"
#include "input.h"

void setup() {
	myDisplay.setup();
}

void loop() {
	myDisplay.preset(2); // 画面プリセット
	delay(200);
}

