#include <Arduino.h>
#include "output.h"
#include "input.h"
#include "AIP.h"
#include "Process.h"

int ball_ang=0;
float ball_dis=0;
float ball_x;
float ball_y;

void ball(void);

void setup() {
  Serial.begin(115200);
	general.setup();
}

void loop() {
	myDisplay.clearDisplay();
	general.startUp(); // 起動時の処理
  mybuzzer.stop();
  while(mySwitch.checkToggleSwitch()){
    mybuzzer.stop();
    myDisplay.clearDisplay();
    ball();
    myDisplay.updateDisplay();
  }
  mybuzzer.stop();
  general.setPhase(1);
}

void ball(){
    myDisplay.ButtonSilent(mySwitch.checkTactSwitch(),mySwitch.checkToggleSwitch());
    ball_ang+=10;
    if(ball_ang>=360)ball_ang=0;
    ball_dis+=2;
    if(ball_dis>300)ball_dis=0;

    //直径25　半径12.5
    myDisplay.drawCircle(32, 32, 25 , false);
    myDisplay.drawCircle(32, 32, 15 ,false);

    // 十字線（縦横）
    myDisplay.drawLine(32, 32-25, 32, 32+25); // 縦線
    myDisplay.drawLine(32-25, 32, 32+25, 32); // 横線

    // 斜め線（×印）
    myDisplay.drawLine(32-18, 32-18, 32+18, 32+18); // 左上→右下
    myDisplay.drawLine(32-18, 32+18, 32+18, 32-18); // 左下→右上

    // ボールの位置を計算
    float rad=radians(ball_ang-90<0? (ball_ang-90)+360:(ball_ang-90));
    ball_x=cos(rad)*(ball_dis/10)+32;
    ball_y=sin(rad)*(ball_dis/10)+32;
    // ボールを描画
    myDisplay.drawLine(32,32,static_cast<int>(ball_x),static_cast<int>(ball_y));
    myDisplay.drawCircle(static_cast<int>(ball_x), (int)ball_y, 4 , true);

    // myDisplay.drawRectangle(63, 0, 2, 64, true);

    myDisplay.drawText(96-21, 4, "BALL",2);
    myDisplay.drawRoundRectangle(68, 1, 58, 21, 7, false);

    myDisplay.drawCircle(76, 44, 8, false);
    myDisplay.drawLine(76, 42, 84, 32);

    myDisplay.drawText(86, 27, "ANG:",1);
    myDisplay.drawText(86+24, 27, String(ball_ang).c_str(),1);

    myDisplay.drawLine(76, 42, 84, 52);

    myDisplay.drawText(86, 48, "DST:",1);
    myDisplay.drawText(86+24, 48, String(static_cast<int>(ball_dis)).c_str(),1);
}
