#include "MyDisplay.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void MyDisplay::setup() {
	// I2C開始。必要なら 100kHz に落とす: Wire.setClock(100000);
    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
	Wire.begin();
	Wire.setClock(400000);

	// SSD1306 初期化（内部チャージポンプ使用）
	if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
		// 初期化失敗時は停止（必要ならここでエラー表示や点滅処理を）
		while (true) {}
	}

	// 画面を真っ黒にして反映（ここまでが“初期設定”）
	display.clearDisplay();
	display.display();
}

void MyDisplay::drawPixel(int x, int y, bool color) {
    display.drawPixel(x, y, color);
}