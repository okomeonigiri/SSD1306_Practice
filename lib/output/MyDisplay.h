#ifndef MYDISPLAY_H
#define MYDISPLAY_H
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Arduino.h>


#define SDA_PIN 25
#define SCL_PIN 24
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET   -1      // リセット未接続なら -1
#define OLED_ADDR    0x3C

class MyDisplay {
public:
    static constexpr byte mode_y=1;

    struct MYUI{
        const char* mode_names[8] = {
            "Attack",
            "Defense",
            "Test",
            "Motor",
            "Line",
            "Ball",
            "Gyro",
            "ATctrl"
        };

        const char* Title;

        struct BUTTON_LIST{
            const char* first;
            const char* second;
            const char* third;
            const char* fourth;// ctrl廃止で使わないーー
            const char* fifth;//  |
            const char* sixth;//  |
        } buttonList;
    } UI;

    MyDisplay();  // コンストラクタ
    void setup();
    int get_radius() { return radius; }

    // 基本描画機能（全てdrawPixelベース）
    void drawPixel(int x, int y, bool color);
    void updateDisplay();
    void clearDisplay();
    void drawText(int x, int y, const char* text, int size = 1);
    void drawTextB(int x, int y, const char* text, int size = 1);
    void drawTextCenter(int centerX, int y, const char* text, int size = 1);
    void drawCircle(int centerX, int centerY, int radius, bool filled = false);
    void drawCircleFromCenter(int centerX, int centerY, int radius, bool filled = false); // drawCircleのエイリアス
    void drawRectangle(int x, int y, int width, int height, bool filled = false);
    void drawRoundRectangle(int x, int y, int width, int height, int radius, bool filled = false);
    void drawRectangleFromCenter(int centerX, int centerY, int width, int height, bool filled = false);
    void drawRoundRectangleFromCenter(int centerX, int centerY, int width, int height, int radius, bool filled = false);
    void drawLine(int x0, int y0, int x1, int y1);
    void preset(int mode,int param =0); // 画面プリセット
    void setGeneralDisplayMode(int mode);
    void ButtonSilent(int tact,bool toggle);

    // 自作機能用のヘルパー関数（public）
    void drawHorizontalLine(int x1, int x2, int y);

    // シリアルモニター出力機能
    void printDisplayToSerial(); // 画面内容をシリアルモニターに出力

    // 動的ビットマップリサイズ機能
    void drawResizedBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t original_w, int16_t original_h, int16_t new_w, int16_t new_h, uint16_t color);

private:
    const int radius=5;
    const int select_frame_num=7;
    int generalDisplayMode=0;

    //ButtonSilent
    bool layer;
    bool left;
    bool center;
    bool right;
    bool toggle;
    Adafruit_SSD1306 display;
};
#endif // MYDISPLAY_H