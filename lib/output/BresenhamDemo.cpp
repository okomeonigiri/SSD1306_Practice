/*
 * ブレゼンハム直線アルゴリズム - 教育用デモ
 * 中学生向けに分かりやすく解説
 */

#include "MyDisplay.h"

class BresenhamDemo {
public:
    MyDisplay* display;
    
    BresenhamDemo(MyDisplay* disp) : display(disp) {}
    
    // 手順1：基本的な考え方を示す
    void showBasicConcept() {
        display->clearDisplay();
        display->drawText(0, 0, "Bresenham Demo", 1);
        display->drawText(0, 15, "Step by Step", 1);
        
        // 問題：(2,1)から(6,4)への直線
        display->drawPixel(2, 25, true);  // 開始点
        display->drawPixel(6, 35, true);  // 終了点
        display->drawText(8, 25, "Start", 1);
        display->drawText(8, 35, "End", 1);
        
        display->updateDisplay();
        delay(2000);
    }
    
    // 手順2：傾きを理解する
    void showSlope() {
        display->clearDisplay();
        display->drawText(0, 0, "Slope = dy/dx", 1);
        
        // dx = 4, dy = 3
        display->drawText(0, 15, "dx=4, dy=3", 1);
        display->drawText(0, 25, "slope=3/4=0.75", 1);
        
        // 格子を描画
        for(int x = 20; x <= 80; x += 10) {
            for(int y = 35; y <= 60; y += 5) {
                display->drawPixel(x, y, true);
            }
        }
        
        display->updateDisplay();
        delay(3000);
    }
    
    // 手順3：決定アルゴリズム
    void showDecisionMaking() {
        display->clearDisplay();
        display->drawText(0, 0, "Decision Making", 1);
        
        // 現在位置から次のピクセルは？
        display->drawText(0, 15, "Next pixel:", 1);
        display->drawText(0, 25, "Right? or", 1);
        display->drawText(0, 35, "Right+Up?", 1);
        
        // 例を示す
        display->drawPixel(30, 50, true);  // 現在位置
        display->drawPixel(35, 50, true);  // 右
        display->drawPixel(35, 45, true);  // 右上
        
        display->drawText(40, 50, "Right", 1);
        display->drawText(40, 40, "R+Up", 1);
        
        display->updateDisplay();
        delay(3000);
    }
    
    // 手順4：誤差の概念
    void showError() {
        display->clearDisplay();
        display->drawText(0, 0, "Error Concept", 1);
        display->drawText(0, 15, "Track how far", 1);
        display->drawText(0, 25, "we are from", 1);
        display->drawText(0, 35, "ideal line", 1);
        
        // 理想的な線と実際のピクセル
        display->drawLine(20, 50, 60, 35);  // 理想線（参考）
        
        // 実際のピクセル位置
        display->drawPixel(25, 48, true);
        display->drawPixel(30, 46, true);
        display->drawPixel(35, 44, true);
        
        display->updateDisplay();
        delay(3000);
    }
};

// 使用例関数
void runBresenhamLesson(MyDisplay* display) {
    BresenhamDemo demo(display);
    
    demo.showBasicConcept();
    demo.showSlope();
    demo.showDecisionMaking(); 
    demo.showError();
}