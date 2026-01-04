#include "General.h"
#include "Process.h"

#define VEGA_PRESET 1
#define BORDER_PRESET 3
#define MODE_SELECT_ANIMATINON 4
constexpr byte mode_y=1;
int tact=0;
bool toggle=0;
bool exit_=true;
Timer generalTimer;
bool pushedNext=false;
const int radius=5;
template<typename T, size_t N>
int size(T (&a)[N]){return N;}

inline void General::WaitReleaseTact() {
    while(mySwitch.checkTactSwitch()!=0){};
}

//auto& UI=myDisplay.UI;
//UI→myDisplay.UI

inline void General::control_guide(bool brack) {
    auto& UI=myDisplay.UI;
    if(brack) {
        if(tact >= 100) { // 操作表示
            myDisplay.drawTextB(2, 54, UI.buttonList.fourth, 1);
            myDisplay.drawTextB(46, 54, UI.buttonList.fifth, 1);
            myDisplay.drawTextB(91, 54, UI.buttonList.sixth, 1);
        } else {
            myDisplay.drawTextB(2, 54, UI.buttonList.first, 1);
            myDisplay.drawTextB(46, 54, UI.buttonList.second, 1);
            myDisplay.drawTextB(91, 54, UI.buttonList.third, 1);
        }
    } else {
        if(tact >= 100) { // 操作表示
            myDisplay.drawText(2, 54, UI.buttonList.fourth, 1);
            myDisplay.drawText(46, 54, UI.buttonList.fifth, 1);
            myDisplay.drawText(91, 54, UI.buttonList.sixth, 1);
        } else {
            myDisplay.drawText(2, 54, UI.buttonList.first, 1);
            myDisplay.drawText(46, 54, UI.buttonList.second, 1);
            myDisplay.drawText(91, 54, UI.buttonList.third, 1);
        }
    }
}

void General::setup() {
    generalTimer.reset();
    Serial.begin(115200);
    mybuzzer.start(400, 100);
    delay(50);
    mybuzzer.start(300, 100);
    myDisplay.setup();
    myDisplay.clearDisplay();
    myDisplay.drawText(0, 0, "VEGA", 2);
    myDisplay.drawText(0, 20, "display done", 1);
    myDisplay.updateDisplay();
    mySwitch.setup(); // スイッチの初期化
    myDisplay.drawText(0, 30, "switch done", 1);
    myDisplay.updateDisplay();
    mybuzzer.setup(); // ブザーの初期化
    myDisplay.drawText(0, 40, "buzzer done", 1);
    myDisplay.updateDisplay();
    myDisplay.clearDisplay();
    myDisplay.drawText(0, 0, "VEGA", 2);
    myDisplay.drawText(0, 20, "setup done", 1);
    myDisplay.drawText(0, 30, "[SUCCESS]", 1);
    String msg = String("Took ") + generalTimer.read_milli() + " ms";
    myDisplay.drawText(0, 40, msg.c_str(), 1);
    myDisplay.updateDisplay();
    while(mySwitch.checkTactSwitch()!=0){};
    myDisplay.preset(1); // 画面プリセット
    mybuzzer.preset(0); // ブザープリセット
    setMode(0);
    setPhase(0);
}
void General::startUp() {
    exit_ = true;
    pushedNext = false;
    while(exit_) {
        generalTimer.reset();
        myDisplay.clearDisplay();
        tact = mySwitch.checkTactSwitch();
        toggle = mySwitch.checkToggleSwitch();
        myDisplay.preset(BORDER_PRESET);
        myDisplay.setGeneralDisplayMode(mode);
        switch(phase) {
            case 0: {//モードセレクト
                ModeSelect();
                break;
            }

            case 1: {//スタンバイ
                StandBy();
                break;
            }

            case 2: {//設定
                Setting();
                break;
            }
        }
        myDisplay.updateDisplay();
    }
}

void General::ModeSelect() {
    auto& UI=myDisplay.UI;
    UI.buttonList.first="Adjust";
    UI.buttonList.second="Select";
    UI.buttonList.third="next >";
    UI.buttonList.fourth="------";
    UI.buttonList.fifth="------";
    UI.buttonList.sixth="< back";
    if(!pushedNext && generalTimer.read_milli() < 1000) { // 1秒後に自動でモードセレクト表示
        myDisplay.drawText(32, 3, "Mode select", 1);
    } else {
        myDisplay.drawTextCenter(64, mode_y,UI.mode_names[mode], 1);
    }

    control_guide();

    // 四角形描画
    myDisplay.drawRoundRectangleFromCenter(64, 32, 32, 32,radius, false);
    myDisplay.drawRoundRectangleFromCenter(22, 32, 22, 22,radius, false);
    myDisplay.drawRoundRectangleFromCenter(106, 32, 22, 22,radius, false);

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

    if(tact == 3) {//next
        control_guide(true);
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
        mybuzzer.start(500, 999);
        myDisplay.preset(4,1);
        mybuzzer.stop();
    }
}

void General::StandBy() {
    auto& UI=myDisplay.UI;
    static unsigned long lastbuzzer = millis();;
    myDisplay.drawText(36, 3, " Stand by ", 1);
    if(millis()-lastbuzzer>=500) {
        mybuzzer.start(400, 999);
    }
    if(millis()-lastbuzzer>=550) {
        mybuzzer.stop();
        lastbuzzer=millis();
    }
    UI.buttonList.first="< Back";
    UI.buttonList.second="SetDir";
    UI.buttonList.third="CODSET";
    UI.buttonList.fourth="------";
    UI.buttonList.fifth="------";
    UI.buttonList.sixth="------";
    // ボタンぷにぷに+トグル
    { int modT = tact >= 100 ? tact - 100 : tact; if(toggle) { myDisplay.drawRectangle(116, 4, 3, 4, true); } else { myDisplay.drawRectangle(116, 8, 3, 4, true); } if(modT == 1 || modT == 4 || modT == 6 || modT == 9) { myDisplay.drawLine(5, 50, 33, 50); myDisplay.drawLine(10, 49, 28, 49); } if(modT == 3 || modT == 4 || modT == 8 || modT == 9) { myDisplay.drawLine(45, 50, 82, 50); myDisplay.drawLine(50, 49, 77, 49); } if(modT == 5 || modT == 6 || modT == 8 || modT == 9) { myDisplay.drawLine(94, 50, 122, 50); myDisplay.drawLine(99, 49, 117, 49); } }

    control_guide();

    if(tact == 1) {// 戻る
        phase = 0;
        if(tact >= 100) { // 操作表示
            myDisplay.drawTextB(2, 54, UI.buttonList.fourth, 1);
            myDisplay.drawTextB(46, 54, UI.buttonList.fifth, 1);
            myDisplay.drawTextB(91, 54, UI.buttonList.sixth, 1);
        } else {
            myDisplay.drawTextB(2, 54, UI.buttonList.first, 1);
            myDisplay.drawTextB(46, 54, UI.buttonList.second, 1);
            myDisplay.drawTextB(91, 54, UI.buttonList.third, 1);
        }
        myDisplay.drawText(2, 54, "------", 1);
        myDisplay.drawText(46, 54, "------", 1);
        myDisplay.drawText(91, 54, "------", 1);
        myDisplay.updateDisplay();
        mybuzzer.start(500, 50);
        mybuzzer.start(500, 50);
        phase = 0;
    }

    if(tact==3) {// SetDir
        mybuzzer.start(300, 500);
        lastbuzzer=millis()-500;
    }

    if(tact==5) {// CODSET
        mybuzzer.start(500, 50);
        delay(50);
        mybuzzer.start(500, 50);
        WaitReleaseTact();
        lastbuzzer=millis()-500;
    }

    if(toggle==1){
        exit_=false;
    }
}

void General::Setting() {
    auto& UI=myDisplay.UI;
    static int cursor=0;
    UI.buttonList.first="< Back";
    UI.buttonList.second="select";
    UI.buttonList.third="next >";
    UI.buttonList.fourth="-----";
    UI.buttonList.fifth="-----";
    UI.buttonList.sixth="------";
    { int modT = tact >= 100 ? tact - 100 : tact; if(toggle) { myDisplay.drawRectangle(116, 4, 3, 4, true); } else { myDisplay.drawRectangle(116, 8, 3, 4, true); } if(modT == 1 || modT == 4 || modT == 6 || modT == 9) { myDisplay.drawLine(5, 50, 33, 50); myDisplay.drawLine(10, 49, 28, 49); } if(modT == 3 || modT == 4 || modT == 8 || modT == 9) { myDisplay.drawLine(45, 50, 82, 50); myDisplay.drawLine(50, 49, 77, 49); } if(modT == 5 || modT == 6 || modT == 8 || modT == 9) { myDisplay.drawLine(94, 50, 122, 50); myDisplay.drawLine(99, 49, 117, 49); } }
    if(tact >= 100) { // 操作表示
        myDisplay.drawText(2, 54, UI.buttonList.fourth, 1);
        myDisplay.drawText(46, 54, UI.buttonList.fifth, 1);
        myDisplay.drawText(91, 54, UI.buttonList.sixth, 1);
    } else {
        myDisplay.drawText(2, 54, UI.buttonList.first, 1);
        myDisplay.drawText(46, 54, UI.buttonList.second, 1);
        myDisplay.drawText(91, 54, UI.buttonList.third, 1);
    }
    if(tact == 1) {// 戻る
        phase = 0;
        mybuzzer.start(1600, 50);
        delay(50);
        mybuzzer.start(1600, 50);
        delay(50);
    }
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
        delay(50);
    }
}


int General::getMode() {
    // モード取得の処理があればここに追加
    return mode; // 仮の戻り値
}

#undef UI