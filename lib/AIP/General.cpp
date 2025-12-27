#include "General.h"
#include "Process.h"

#define VEGA_PRESET 1
#define BORDER_PRESET 3
#define MODE_SELECT_ANIMATINON 4

constexpr byte mode_y=1;
int tact=0;
bool toggle=0;
int cursor=0;
Timer generalTimer;
struct BUTTON_LIST{
    const char* first;
    const char* second;
    const char* third;
    const char* fourth;
    const char* fifth;
    const char* sixth;
};
BUTTON_LIST buttonList;
bool pushedNext=false;
int lastBuzzer=0;
const int radius=5;
template<typename T, size_t N>
int size(T (&a)[N]){
    return N;
}

inline void control_guide(){
    if(tact >= 100) { // 操作表示
        myDisplay.drawText(2, 54, buttonList.fourth, 1);
        myDisplay.drawText(46, 54, buttonList.fifth, 1);
        myDisplay.drawText(91, 54, buttonList.sixth, 1);
    } else {
        myDisplay.drawText(2, 54, buttonList.first, 1);
        myDisplay.drawText(46, 54, buttonList.second, 1);
        myDisplay.drawText(91, 54, buttonList.third, 1);
    }
}

void General::setup() {
    Serial.begin(115200);
    mybuzzer.start(400, 100);
    delay(50);
    mybuzzer.start(300, 100);
    myDisplay.setup();
    myDisplay.clearDisplay();
    myDisplay.drawText(0, 0, "VEGA", 2);
    myDisplay.drawText(0, 20, "displaydone", 1);
    myDisplay.drawText(0, 30, "switch<-", 1);
    myDisplay.updateDisplay();
    mySwitch.setup(); // スイッチの初期化
    myDisplay.clearDisplay();
    myDisplay.drawText(0, 0, "VEGA", 2);
    myDisplay.drawText(0, 20, "displaydone", 1);
    myDisplay.drawText(0, 30, "switch done", 1);
    myDisplay.drawText(0, 40, "buzzer<-", 1);
    myDisplay.updateDisplay();
    mybuzzer.setup(); // ブザーの初期化
    myDisplay.clearDisplay();
    myDisplay.drawText(0, 0, "VEGA", 2);
    myDisplay.drawText(0, 20, "displaydone", 1);
    myDisplay.drawText(0, 30, "switch done", 1);
    myDisplay.drawText(0, 40, "buzzer done", 1);
    myDisplay.updateDisplay();
    myDisplay.preset(1); // 画面プリセット
    mybuzzer.preset(0); // ブザープリセット
    setMode(0);
    setPhase(0);
}

void General::startUp() {
    bool exit = true;
    pushedNext = false;
    generalTimer.reset(); // ここで一度だけリセット

    while(exit) {
        myDisplay.clearDisplay();
        tact = mySwitch.checkTactSwitch();
        toggle = mySwitch.checkToggleSwitch();
        switch(phase) {
            case 0: {
                buttonList.first="Adjust";
                buttonList.second="Select";
                buttonList.third="next >";
                buttonList.fourth="------";
                buttonList.fifth="------";
                buttonList.sixth="< back";
                myDisplay.preset(BORDER_PRESET); // 枠線を描画
                if(!pushedNext && generalTimer.read_milli() < 1000) { // 1秒後に自動でモードセレクト表示
                    myDisplay.drawText(32, 3, "Mode select", 1);
                } else {
                    switch (mode) {
                        case 0:
                            myDisplay.drawText(38+8+1, mode_y, "Attack ", 1);
                            break;
                        case 1:
                            myDisplay.drawText(36+8, mode_y, "Defense ", 1);
                            break;
                        case 2:
                            myDisplay.drawText(38+16-2, mode_y, "Test  ", 1);
                            break;
                        case 3:
                            myDisplay.drawText(38+8+4, mode_y, "motor ", 1);
                            break;
                        case 4:
                            myDisplay.drawText(38+16-1, mode_y, "line  ", 1);
                            break;
                        case 5:
                            myDisplay.drawText(38+16-1, mode_y, "ball  ", 1);
                            break;
                        case 6:
                            myDisplay.drawText(38+16-1, mode_y, "gyro  ", 1);
                            break;
                        case 7:
                            myDisplay.drawText(38+8+1, mode_y, "ATctrl ", 1);
                            break;
                        default:
                            myDisplay.drawText(36, mode_y, "Mode ? err", 1);
                            break;
                    }
                }

                control_guide();

                // 四角形描画
                {myDisplay.drawRoundRectangleFromCenter(64, 32, 32, 32,radius, false);myDisplay.drawRoundRectangleFromCenter(22, 32, 22, 22,radius, false);myDisplay.drawRoundRectangleFromCenter(106, 32, 22, 22,radius, false);}

                // ボタンぷにぷに+トグル
                { int modT = tact >= 100 ? tact - 100 : tact; if(toggle) { myDisplay.drawRectangle(116, 4, 3, 4, true); } else { myDisplay.drawRectangle(116, 8, 3, 4, true); } if(modT == 1 || modT == 4 || modT == 6 || modT == 9) { myDisplay.drawLine(5, 50, 33, 50); myDisplay.drawLine(10, 49, 28, 49); } if(modT == 3 || modT == 4 || modT == 8 || modT == 9) { myDisplay.drawLine(45, 50, 82, 50); myDisplay.drawLine(50, 49, 77, 49); } if(modT == 5 || modT == 6 || modT == 8 || modT == 9) { myDisplay.drawLine(94, 50, 122, 50); myDisplay.drawLine(99, 49, 117, 49); } }
                if(tact == 5) {
                    pushedNext = true;
                    mode = (mode + 1) % 8;
                    myDisplay.setGeneralDisplayMode(mode);
                    mybuzzer.start(500, 999);
                    myDisplay.preset(MODE_SELECT_ANIMATINON);
                    mybuzzer.stop();
                }

                if(tact == 3) {
                    // control_guide();
                    myDisplay.drawText(2, 54, "------", 1);
                    myDisplay.drawText(46, 54, "------", 1);
                    myDisplay.drawText(91, 54, "------", 1);
                    myDisplay.updateDisplay();
                    mybuzzer.start(500, 500);
                    generalTimer.set_milli(500); // タイマーリセット
                    phase = 1;
                }

                if(tact == 1){//設定に移行
                    phase = 2;
                    mybuzzer.start(1600, 50);
                    delay(50);
                    mybuzzer.start(1600, 50);
                    delay(50);
                    mybuzzer.start(1600, 50);
                    delay(50);
                    mybuzzer.stop();
                }

                if(tact == 105){
                    pushedNext = true;
                    mode = (mode - 1) < 0 ? 7 : mode - 1;
                    myDisplay.setGeneralDisplayMode(mode);
                    mybuzzer.start(500, 999);
                    myDisplay.preset(4,1);
                    mybuzzer.stop();
                }
                break;
            }

            case 1: {
                myDisplay.drawText(36, 3, " Stand by ", 1);
                if(generalTimer.read_milli()>=500) {
                    mybuzzer.startS(400, 999);
                }
                if(generalTimer.read_milli()>=550) {
                    mybuzzer.stop();
                    generalTimer.reset();
                }
                buttonList.first="< Back";
                buttonList.second="SetDir";
                buttonList.third="CODSET";
                buttonList.fourth="------";
                buttonList.fifth="------";
                buttonList.sixth="------";
                myDisplay.preset(BORDER_PRESET); // 枠線を描画
                // ボタンぷにぷに+トグル
                { int modT = tact >= 100 ? tact - 100 : tact; if(toggle) { myDisplay.drawRectangle(116, 4, 3, 4, true); } else { myDisplay.drawRectangle(116, 8, 3, 4, true); } if(modT == 1 || modT == 4 || modT == 6 || modT == 9) { myDisplay.drawLine(5, 50, 33, 50); myDisplay.drawLine(10, 49, 28, 49); } if(modT == 3 || modT == 4 || modT == 8 || modT == 9) { myDisplay.drawLine(45, 50, 82, 50); myDisplay.drawLine(50, 49, 77, 49); } if(modT == 5 || modT == 6 || modT == 8 || modT == 9) { myDisplay.drawLine(94, 50, 122, 50); myDisplay.drawLine(99, 49, 117, 49); } }
                if(tact >= 100) { // 操作表示
                    myDisplay.drawText(2, 54, buttonList.fourth, 1);
                    myDisplay.drawText(46, 54, buttonList.fifth, 1);
                    myDisplay.drawText(91, 54, buttonList.sixth, 1);
                } else {
                    myDisplay.drawText(2, 54, buttonList.first, 1);
                    myDisplay.drawText(46, 54, buttonList.second, 1);
                    myDisplay.drawText(91, 54, buttonList.third, 1);
                }
                if(tact == 1) {// 戻る
                    phase = 0;
                    if(tact >= 100) { // 操作表示
                        myDisplay.drawTextB(2, 54, buttonList.fourth, 1);
                        myDisplay.drawTextB(46, 54, buttonList.fifth, 1);
                        myDisplay.drawTextB(91, 54, buttonList.sixth, 1);
                    } else {
                        myDisplay.drawTextB(2, 54, buttonList.first, 1);
                        myDisplay.drawTextB(46, 54, buttonList.second, 1);
                        myDisplay.drawTextB(91, 54, buttonList.third, 1);
                    }
                    myDisplay.drawText(2, 54, "------", 1);
                    myDisplay.drawText(46, 54, "------", 1);
                    myDisplay.drawText(91, 54, "------", 1);
                    myDisplay.updateDisplay();
                    mybuzzer.start(500, 50);
                    mybuzzer.start(500, 50);
                    phase = 0;
                }

                if(toggle==1){
                    exit=false;
                }
                break;
            }

            case 2: {
                // myDisplay.drawText(36, 3, " Setting ", 1);
                buttonList.first="< Back";
                buttonList.second="select";
                buttonList.third="next >";
                buttonList.fourth="-----";
                buttonList.fifth="-----";
                buttonList.sixth="------";
                myDisplay.preset(BORDER_PRESET); // 枠線を描画
                // ボタンぷにぷに+トグル
                { int modT = tact >= 100 ? tact - 100 : tact; if(toggle) { myDisplay.drawRectangle(116, 4, 3, 4, true); } else { myDisplay.drawRectangle(116, 8, 3, 4, true); } if(modT == 1 || modT == 4 || modT == 6 || modT == 9) { myDisplay.drawLine(5, 50, 33, 50); myDisplay.drawLine(10, 49, 28, 49); } if(modT == 3 || modT == 4 || modT == 8 || modT == 9) { myDisplay.drawLine(45, 50, 82, 50); myDisplay.drawLine(50, 49, 77, 49); } if(modT == 5 || modT == 6 || modT == 8 || modT == 9) { myDisplay.drawLine(94, 50, 122, 50); myDisplay.drawLine(99, 49, 117, 49); } }
                if(tact >= 100) { // 操作表示
                    myDisplay.drawText(2, 54, buttonList.fourth, 1);
                    myDisplay.drawText(46, 54, buttonList.fifth, 1);
                    myDisplay.drawText(91, 54, buttonList.sixth, 1);
                } else {
                    myDisplay.drawText(2, 54, buttonList.first, 1);
                    myDisplay.drawText(46, 54, buttonList.second, 1);
                    myDisplay.drawText(91, 54, buttonList.third, 1);
                }
                if(tact == 1) {// 戻る
                    phase = 0;
                    mybuzzer.start(1600, 50);
                    delay(50);
                    mybuzzer.start(1600, 50);
                    delay(50);
                }
                switch (mode){
                case 1:{// Defense
                myDisplay.drawTextCenter(64,0,"Defense",1);
                cursor = cursor % size(defense.keys);
                myDisplay.drawTextCenter(64,16,defense.keys[cursor-1<0 ? size(defense.keys)-1 : cursor-1].c_str(),1);
                myDisplay.drawTextCenter(64,26,defense.keys[cursor].c_str(),1);
                myDisplay.drawTextCenter(64,36,defense.keys[(cursor+1)%size(defense.keys)].c_str(),1);
                int W = (int)strlen(defense.keys[cursor].c_str()) * 6;
                myDisplay.drawRectangle(64 - W/2 -2, 25, W +4, 10, false);

                if(mySwitch.checkTactSwitch()==5){
                    cursor=(cursor+1)%size(defense.keys);
                    mybuzzer.start(800,50);
                }
                break;
            }
                default:{
                myDisplay.drawText(0,0,"No setting lol",1);
                break;
                }
            }
        }
        myDisplay.updateDisplay();
    }
}
}

int General::getMode() {
    // モード取得の処理があればここに追加
    return mode; // 仮の戻り値
}