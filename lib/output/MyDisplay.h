#ifndef MYDISPLAY_H
#define MYDISPLAY_H
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Arduino.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET   -1      // リセット未接続なら -1
#define OLED_ADDR    0x3C    // i2c_scanner で確認したアドレスに合わせる

class MyDisplay {
public:
    MyDisplay();  // コンストラクタ
    void setup();
    
    // 基本描画機能（全てdrawPixelベース）
    void drawPixel(int x, int y, bool color);
    void updateDisplay();
    void clearDisplay();
    void drawText(int x, int y, const char* text, int size = 1);
    void drawCircle(int centerX, int centerY, int radius, bool filled = false);
    void drawRectangle(int x, int y, int width, int height, bool filled = false);
    void drawLine(int x0, int y0, int x1, int y1);
    void preset(int mode); // 画面プリセット
    
    // 自作機能用のヘルパー関数（public）
    void drawHorizontalLine(int x1, int x2, int y);
    
private:
    Adafruit_SSD1306 display;
};
#endif // MYDISPLAY_H