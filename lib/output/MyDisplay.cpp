#include "MyDisplay.h"
#include <input.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// コンストラクタでdisplayオブジェクトを初期化
MyDisplay::MyDisplay() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
}

//Teensy 4.1
void MyDisplay::setup() {
	// I2C開始。SDA=D25, SCL=D24を指定
	Wire.begin();
	Wire.setSDA(SDA_PIN);
	Wire.setSCL(SCL_PIN);
	Wire.setClock(800000);

	// SSD1306 初期化（内部チャージポンプ使用）
	if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
		// 初期化失敗時は停止（必要ならここでエラー表示や点滅処理を）
		while (true) {//高速Lチカ
            digitalWrite(LED_BUILTIN, HIGH);
            delay(10);
            digitalWrite(LED_BUILTIN, LOW);
            delay(10);
        }
	}

	// 画面を真っ黒にして反映（ここまでが"初期設定"）
	display.clearDisplay();
	display.display();
}

void MyDisplay::drawPixel(int x, int y, bool color) {
    // 画面範囲チェック
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
    display.drawPixel(x, y, color ? SSD1306_WHITE : SSD1306_BLACK);
}

void MyDisplay::updateDisplay() {
    display.display();
}

void MyDisplay::clearDisplay() {
    display.clearDisplay();
}

void MyDisplay::drawText(int x, int y, const char* text, int size) {
    display.setTextSize(size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(x, y);
    display.print(text);
}

void MyDisplay::drawTextB(int x, int y, const char* text, int size) {
    display.setTextSize(size);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(x, y);
    display.print(text);
}

// ヘルパー関数：水平線描画（塗りつぶし円用）
void MyDisplay::drawHorizontalLine(int x1, int x2, int y) {
    if (x1 > x2) {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    for (int x = x1; x <= x2; x++) {
        drawPixel(x, y, true);
    }
}

void MyDisplay::drawCircle(int centerX, int centerY, int radius, bool filled) {
    // ブレゼンハムの円描画アルゴリズム
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    
    while (y >= x) {
        if (filled) {
            // 塗りつぶし円：各yレベルで水平線を描画
            drawHorizontalLine(centerX - x, centerX + x, centerY + y);
            drawHorizontalLine(centerX - x, centerX + x, centerY - y);
            drawHorizontalLine(centerX - y, centerX + y, centerY + x);
            drawHorizontalLine(centerX - y, centerX + y, centerY - x);
        } else {
            // 円周のみ：8方向の対称点を描画
            drawPixel(centerX + x, centerY + y, true);
            drawPixel(centerX - x, centerY + y, true);
            drawPixel(centerX + x, centerY - y, true);
            drawPixel(centerX - x, centerY - y, true);
            drawPixel(centerX + y, centerY + x, true);
            drawPixel(centerX - y, centerY + x, true);
            drawPixel(centerX + y, centerY - x, true);
            drawPixel(centerX - y, centerY - x, true);
        }

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        x++;
    }
}

void MyDisplay::drawRectangle(int x, int y, int width, int height, bool filled) {
    if (filled) {
        // 塗りつぶし四角形：各行で水平線を描画
        for (int j = y; j < y + height; j++) {
            drawHorizontalLine(x, x + width - 1, j);
        }
    } else {
        // 四角形の枠線：4辺を描画
        // 上辺
        drawHorizontalLine(x, x + width - 1, y);
        // 下辺
        drawHorizontalLine(x, x + width - 1, y + height - 1);
        // 左辺
        for (int j = y; j < y + height; j++) {
            drawPixel(x, j, true);
        }
        // 右辺
        for (int j = y; j < y + height; j++) {
            drawPixel(x + width - 1, j, true);
        }
    }
}

void MyDisplay::drawLine(int x0, int y0, int x1, int y1) {
    // ブレゼンハムの直線描画アルゴリズム
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    int x = x0;
    int y = y0;
    
    while (true) {
        drawPixel(x, y, true);
        
        if (x == x1 && y == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void MyDisplay::drawRectangleFromCenter(int centerX, int centerY, int width, int height, bool filled) {
    // 中心点から左上角の座標を計算
    int x = centerX - width / 2;
    int y = centerY - height / 2;
    
    // 既存のdrawRectangle関数を呼び出し
    drawRectangle(x, y, width, height, filled);
}

void MyDisplay::preset(int mode) {
    
    switch (mode) {
        case 1: //VEGA ロゴ　V.1
        clearDisplay();
            drawRectangle(32, 9, 64, 2, true);
            drawRectangle(32, 11, 63, 2, true);

            drawRectangle(31, 12, 5, 6, true);
            drawRectangle(30, 18, 5, 5, true);
            drawRectangle(29, 23, 5, 5, true);
            drawRectangle(28, 28, 5, 6, true);
            drawRectangle(27, 34, 5, 5, true);
            drawRectangle(26, 39, 5, 5, true);
            drawRectangle(21, 44, 9, 6, true);
            drawRectangle(22, 50, 7, 5, true);
            drawRectangle(23, 55, 5, 3, true);

            drawRectangle(16, 24, 7, 3, true);
            drawRectangle(17, 27, 6, 1, true);
            drawRectangle(17, 28, 7, 4, true);
            drawRectangle(18, 32, 6, 4, true);
            drawRectangle(19, 34, 6, 7, true);
            drawRectangle(20, 41, 6, 5, true);

            drawRectangle(40, 14, 4, 2, true);
            drawRectangle(39, 16, 4, 6, true);
            drawRectangle(38, 22, 4, 5, true);
            drawRectangle(37, 27, 4, 6, true);
            drawRectangle(36, 33, 4, 5, true);
            drawRectangle(35, 38, 4, 6, true);
            drawRectangle(34, 44, 4, 5, true);
            drawRectangle(33, 49, 4, 4, true);

            drawRectangle(32, 55, 1, 3, true);
            drawRectangle(33, 53, 22, 5, true);
            drawRectangle(55, 53, 1, 2, true);

            drawRectangle(42, 31, 1, 3, true);
            drawRectangle(43, 30, 14, 4, true);
            drawRectangle(57, 30, 1, 3, true);
            drawPixel(58,30,true);

            drawRectangle(65, 14, 5, 4, true);
            drawRectangle(64, 18, 5, 6, true);
            drawRectangle(63, 24, 5, 6, true);
            drawRectangle(62, 30, 5, 6, true);
            drawRectangle(61, 36, 5, 6, true);
            drawRectangle(60, 42, 5, 6, true);
            drawRectangle(59, 48, 5, 4, true);
            drawRectangle(59, 52, 4, 3, true);
            drawRectangle(58, 53, 1, 5, true);
            drawRectangle(57, 55, 1, 3, true);

            drawRectangle(63, 53, 18, 2, true);
            drawRectangle(59, 55, 21, 3, true);

            drawRectangle(81, 35, 5, 3, true);
            drawRectangle(80, 38, 5, 3, true);
            drawRectangle(79, 41, 5, 4, true);
            drawRectangle(78, 45, 5, 3, true);
            drawRectangle(77, 48, 5, 3, true);
            drawRectangle(76, 51, 4, 2, true);
            drawRectangle(80, 51, 1, 4, true);

            drawPixel(72,33,true);
            drawRectangle(73, 31, 1, 3, true);
            drawRectangle(74, 30, 37, 4, true);

            drawRectangle(92, 18, 4, 4, true);
            drawRectangle(91, 22, 5, 3, true);
            drawRectangle(90, 25, 5, 3, true);
            drawRectangle(89, 28, 5, 2, true);
            drawRectangle(88, 34, 5, 1, true);
            drawRectangle(87, 35, 5, 4, true);
            drawRectangle(86, 39, 5, 3, true);
            drawRectangle(85, 42, 5, 4, true);
            drawRectangle(84, 46, 5, 3, true);
            drawRectangle(83, 49, 5, 3, true);
            drawRectangle(82, 52, 5, 4, true);
            drawRectangle(81, 56, 5, 2, true);

            drawRectangle(99, 9, 8, 3, true);
            drawRectangle(99, 12, 9, 2, true);
            drawRectangle(100, 14, 8, 4, true);
            drawRectangle(100, 18, 9, 1, true);
            drawRectangle(101, 19, 8, 4, true);
            drawRectangle(101, 23, 9, 1, true);
            drawRectangle(102, 24, 8, 5, true);
            drawRectangle(103, 28, 8, 2, true);
            drawRectangle(104, 34, 8, 5, true);
            drawRectangle(105, 39, 8, 5, true);
            drawRectangle(106, 44, 8, 5, true);
            drawRectangle(107, 49, 7, 5, true);
            drawRectangle(108, 54, 6, 4, true);
            drawRectangle(114, 50, 1, 8, true);
            drawRectangle(115, 55, 1, 3, true);
            updateDisplay();
            break;
        case 2: { //VEGAロゴ+アニメーション
            clearDisplay();
            float duration = 0.2; //アニメーションの速さ調整
            drawRectangle(16, 24, 7, 3, true);updateDisplay();delay(duration);
            drawRectangle(17, 27, 6, 1, true);updateDisplay();delay(duration);
            drawRectangle(17, 28, 7, 4, true);updateDisplay();delay(duration);
            drawRectangle(18, 32, 6, 4, true);updateDisplay();delay(duration);
            drawRectangle(19, 34, 6, 7, true);updateDisplay();delay(duration);
            drawRectangle(20, 41, 6, 5, true);updateDisplay();delay(duration);

            drawRectangle(32, 9, 64, 2, true);updateDisplay();delay(duration);
            drawRectangle(32, 11, 63, 2, true);updateDisplay();delay(duration);

            drawRectangle(31, 12, 5, 6, true);updateDisplay();delay(duration);
            drawRectangle(30, 18, 5, 5, true);updateDisplay();delay(duration);
            drawRectangle(29, 23, 5, 5, true);updateDisplay();delay(duration);
            drawRectangle(28, 28, 5, 6, true);updateDisplay();delay(duration);
            drawRectangle(27, 34, 5, 5, true);updateDisplay();delay(duration);
            drawRectangle(26, 39, 5, 5, true);updateDisplay();delay(duration);
            drawRectangle(21, 44, 9, 6, true);updateDisplay();delay(duration);
            drawRectangle(22, 50, 7, 5, true);updateDisplay();delay(duration);
            drawRectangle(23, 55, 5, 3, true);updateDisplay();delay(duration);

            drawRectangle(40, 14, 4, 2, true);updateDisplay();delay(duration);
            drawRectangle(39, 16, 4, 6, true);updateDisplay();delay(duration);
            drawRectangle(38, 22, 4, 5, true);updateDisplay();delay(duration);
            drawRectangle(37, 27, 4, 6, true);updateDisplay();delay(duration);
            drawRectangle(36, 33, 4, 5, true);updateDisplay();delay(duration);
            drawRectangle(35, 38, 4, 6, true);updateDisplay();delay(duration);
            drawRectangle(34, 44, 4, 5, true);updateDisplay();delay(duration);
            drawRectangle(33, 49, 4, 4, true);updateDisplay();delay(duration);

            drawRectangle(32, 55, 1, 3, true);updateDisplay();delay(duration);
            drawRectangle(33, 53, 22, 5, true);updateDisplay();delay(duration);
            drawRectangle(55, 53, 1, 2, true);updateDisplay();delay(duration);

            drawRectangle(42, 31, 1, 3, true);updateDisplay();delay(duration);
            drawRectangle(43, 30, 14, 4, true);updateDisplay();delay(duration);
            drawRectangle(57, 30, 1, 3, true);updateDisplay();delay(duration);
            drawPixel(58,30,true);updateDisplay();delay(duration);

            drawRectangle(65, 14, 5, 4, true);updateDisplay();delay(duration);
            drawRectangle(64, 18, 5, 6, true);updateDisplay();delay(duration);
            drawRectangle(63, 24, 5, 6, true);updateDisplay();delay(duration);
            drawRectangle(62, 30, 5, 6, true);updateDisplay();delay(duration);
            drawRectangle(61, 36, 5, 6, true);updateDisplay();delay(duration);
            drawRectangle(60, 42, 5, 6, true);updateDisplay();delay(duration);
            drawRectangle(59, 48, 5, 4, true);updateDisplay();delay(duration);
            drawRectangle(59, 52, 4, 3, true);updateDisplay();delay(duration);
            drawRectangle(58, 53, 1, 5, true);updateDisplay();delay(duration);
            drawRectangle(57, 55, 1, 3, true);updateDisplay();delay(duration);

            drawRectangle(63, 53, 18, 2, true);updateDisplay();delay(duration);
            drawRectangle(59, 55, 21, 3, true);updateDisplay();delay(duration);

            drawRectangle(81, 35, 5, 3, true);updateDisplay();delay(duration);
            drawRectangle(80, 38, 5, 3, true);updateDisplay();delay(duration);
            drawRectangle(79, 41, 5, 4, true);updateDisplay();delay(duration);
            drawRectangle(78, 45, 5, 3, true);updateDisplay();delay(duration);
            drawRectangle(77, 48, 5, 3, true);updateDisplay();delay(duration);
            drawRectangle(76, 51, 4, 2, true);updateDisplay();delay(duration);
            drawRectangle(80, 51, 1, 4, true);updateDisplay();delay(duration);

            drawPixel(72,33,true);updateDisplay();delay(duration);
            drawRectangle(73, 31, 1, 3, true);updateDisplay();delay(duration);
            drawRectangle(74, 30, 37, 4, true);updateDisplay();delay(duration);

            drawRectangle(92, 18, 4, 4, true);updateDisplay();delay(duration);
            drawRectangle(91, 22, 5, 3, true);updateDisplay();delay(duration);
            drawRectangle(90, 25, 5, 3, true);updateDisplay();delay(duration);
            drawRectangle(89, 28, 5, 2, true);updateDisplay();delay(duration);
            drawRectangle(88, 34, 5, 1, true);updateDisplay();delay(duration);
            drawRectangle(87, 35, 5, 4, true);updateDisplay();delay(duration);
            drawRectangle(86, 39, 5, 3, true);updateDisplay();delay(duration);
            drawRectangle(85, 42, 5, 4, true);updateDisplay();delay(duration);
            drawRectangle(84, 46, 5, 3, true);updateDisplay();delay(duration);
            drawRectangle(83, 49, 5, 3, true);updateDisplay();delay(duration);
            drawRectangle(82, 52, 5, 4, true);updateDisplay();delay(duration);
            drawRectangle(81, 56, 5, 2, true);updateDisplay();delay(duration);

            drawRectangle(99, 9, 8, 3, true);updateDisplay();delay(duration);
            drawRectangle(99, 12, 9, 2, true);updateDisplay();delay(duration);
            drawRectangle(100, 14, 8, 4, true);updateDisplay();delay(duration);
            drawRectangle(100, 18, 9, 1, true);updateDisplay();delay(duration);
            drawRectangle(101, 19, 8, 4, true);updateDisplay();delay(duration);
            drawRectangle(101, 23, 9, 1, true);updateDisplay();delay(duration);
            drawRectangle(102, 24, 8, 5, true);updateDisplay();delay(duration);
            drawRectangle(103, 28, 8, 2, true);updateDisplay();delay(duration);
            drawRectangle(104, 34, 8, 5, true);updateDisplay();delay(duration);
            drawRectangle(105, 39, 8, 5, true);updateDisplay();delay(duration);
            drawRectangle(106, 44, 8, 5, true);updateDisplay();delay(duration);
            drawRectangle(107, 49, 7, 5, true);updateDisplay();delay(duration);
            drawRectangle(108, 54, 6, 4, true);updateDisplay();delay(duration);
            drawRectangle(114, 50, 1, 8, true);updateDisplay();delay(duration);
            drawRectangle(115, 55, 1, 3, true);updateDisplay();delay(duration);
            updateDisplay();
            break;
            updateDisplay();
        }
        case 3:{//モードセレクト枠
            {//右側扇形
                drawLine(99,11,101,11);
                drawLine(102,10,103,10);
                drawPixel(104,9,true);
                drawLine(105,8,105,7);
                drawLine(106,6,106,4);
                drawLine(107,3,107,0);
                drawRectangle(115, 3, 5, 10, false); // 四角形追加

                drawLine(29,12,98,12);
            }
            drawLine(29,12,98,12);//上側直線
            {//左側扇形（左右対称）
                drawLine(26,11,28,11);
                drawLine(24,10,25,10);
                drawPixel(23,9,true);
                drawLine(22,8,22,7);
                drawLine(21,6,21,4);
                drawLine(20,3,20,0);
            }

            {//ボタン囲い
            drawLine(0,51,127,51);
            drawLine(39,52,39,63);
            drawLine(88,52,88,63);
            }

            
            break;
        }
        case 4: { //モードセレクト(アニメーション割り込み)
            clearDisplay();
            float temp = (float)select_frame_num; // float変数で正確な割り算
            for (int i = 0; i < select_frame_num; i++) {
                clearDisplay();
                preset(3); // モードセレクト枠を描画
                // float計算で正確な座標とサイズを計算
                float progress = (float)i / temp;

                drawRectangleFromCenter((int)(22 - 42 * progress),32,(int)(22 - 10 * progress),(int)(22 - 10 * progress),false); // 左

                drawRectangleFromCenter((int)(106 - 42 * progress),32,(int)(22 + 10 * progress),(int)(22 + 10 * progress),false); // 右

                drawRectangleFromCenter((int)(148 - 42 * progress),32,(int)(12 + 10 * progress),(int)(12 + 10 * progress),false); // 右2

                drawRectangleFromCenter((int)(64 - 42 * progress),32,(int)(32 - 10 * progress),(int)(32 - 10 * progress),false); // 中央

                drawText(2,54,"------",1);
                drawText(46,54,"------",1);
                drawText(91,54,"------",1);
                drawLine(94,50,122,50);
                drawLine(99,49,117,49);
                switch (generalDisplayMode)
                    {
                    case 0:
                            drawText(38+8+1, 3, "Attack ", 1);
                            break;
                        case 1:
                            drawText(36+8, 3, "Defense ", 1);
                            break;
                        case 2:
                            drawText(38+16-2, 3, "Test  ", 1);
                            break;
                        case 3:
                            drawText(38+8+4, 3, "motor ", 1);
                            break;
                        case 4:
                            drawText(38+16-1, 3, "line  ", 1);
                            break;
                        case 5:
                            drawText(38+16-1, 3, "ball  ", 1);
                            break;
                        case 6:
                            drawText(38+16-1, 3, "gyro  ", 1);
                            break;
                        case 7:
                            drawText(38+8+1, 3, "ATctrl ", 1);
                            break;
                        default:
                            drawText(36, 3, "Mode ? err", 1);
                            break;
                    }
                if(mySwitch.checkToggleSwitch()){drawRectangle(116,4,3,4,true);} else {drawRectangle(116,8,3,4,true);}
                updateDisplay();
            }
            updateDisplay();
            break;
        }
        default:
            drawText(0, 0, "Unknown Mode", 1);
            break;
    }
}

void MyDisplay::setGeneralDisplayMode(int mode) {
    generalDisplayMode = mode;
}
