#include "General.h"
#include "Process.h"

#define VEGA_PRESET 1
#define BORDER_PRESET 3
#define MODE_SELECT_ANIMATINON 4
constexpr int mode_y=1;
int tact=0;
int cursor=0;
bool toggle=0;
bool exit_=true;
Timer generalTimer;
Timer startupTimer;
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
    int modT = tact >= 100 ? tact - 100 : tact;
    if(toggle) {
        myDisplay.drawRectangle(116, 4, 3, 4, true);
    } else {
        myDisplay.drawRectangle(116, 8, 3, 4, true);
    } if(modT == 1 || modT == 4 || modT == 6 || modT == 9){
        myDisplay.drawLine(5, 50, 33, 50); myDisplay.drawLine(10, 49, 28, 49);
    } if(modT == 3 || modT == 4 || modT == 8 || modT == 9) {
        myDisplay.drawLine(45, 50, 82, 50); myDisplay.drawLine(50, 49, 77, 49);
    } if(modT == 5 || modT == 6 || modT == 8 || modT == 9) {
        myDisplay.drawLine(94, 50, 122, 50); myDisplay.drawLine(99, 49, 117, 49);
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
    startupTimer.reset();
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
    if(!pushedNext && startupTimer.read_milli() < 1000) { // 1秒後に自動でモードセレクト表示
        myDisplay.drawText(32, 3, "Mode select", 1);
    } else {
        myDisplay.drawTextCenter(64, mode_y,UI.mode_names[mode], 1);
    }

    control_guide();

    // 四角形描画
    myDisplay.drawRoundRectangleFromCenter(64, 32, 32, 32,radius, false);
    myDisplay.drawRoundRectangleFromCenter(22, 32, 22, 22,radius, false);
    myDisplay.drawRoundRectangleFromCenter(106, 32, 22, 22,radius, false);

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
        cursor=0;
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
    const String* keysPtr = nullptr;
    float* valPtr = nullptr;
    int keysLen = 0;
    static int lastmode=mode;
    static bool anger=false;
    static bool lectured=false;
    if((anger||lectured)&&lastmode!=mode){
        anger=false;
        lectured=false;
        lastmode=mode;
    }
    //  int SettingPhase;

    UI.buttonList.first="< Exit";
    UI.buttonList.second="select";
    UI.buttonList.third="next >";
    UI.buttonList.fourth="-----";
    UI.buttonList.fifth="-----";
    UI.buttonList.sixth="------";
    control_guide();
    if(tact == 1) {// 戻る
        phase = 0;
        mybuzzer.start(1600, 50);
        delay(50);
        mybuzzer.start(1600, 50);
        delay(50);
    }
    else if(tact == 5) {// next >
        cursor=(cursor+1);
        mybuzzer.start(800, 50);
        delay(50);
    }
    switch (mode) {
        case 0: // Attack
            keysPtr = attack.keys;
            valPtr = attack.value;
            keysLen = size(attack.keys);
            break;
        case 1: // Defense
            keysPtr = defense.keys;
            valPtr = defense.value;
            keysLen = size(defense.keys);
            break;
        default:// まだないから一旦ね
            keysPtr = nullptr;
            valPtr = nullptr;
            keysLen = 999;
            break;
    }

    myDisplay.drawTextCenter(64, 0, UI.mode_names[mode], 1);

    if (keysLen != 999) {
        cursor = cursor % keysLen;
        const int prev = (cursor - 1 + keysLen) % keysLen;
        const int next = (cursor + 1) % keysLen;

        myDisplay.drawTextCenter(64, 16, keysPtr[prev].c_str(), 1);
        myDisplay.drawTextCenter(64, 26, keysPtr[cursor].c_str(), 1);
        myDisplay.drawTextCenter(64, 36, keysPtr[next].c_str(), 1);
        int W = (int)strlen(keysPtr[cursor].c_str()) * 6;
        myDisplay.drawRectangle(64 - W/2 -2, 25, W +4, 10, false);
    } else {
        if(lectured){
            myDisplay.clearDisplay();
            myDisplay.drawTextCenter(64, 16, "No settings", 1);
            myDisplay.updateDisplay();
            delay(750);
            myDisplay.drawTextCenter(64, 26, "No more easter egg", 1);
            myDisplay.updateDisplay();
            delay(2500);
            myDisplay.clearDisplay();
            myDisplay.drawTextCenter(64, 32, "OK?", 2);
            myDisplay.updateDisplay();
            delay(1000);
            myDisplay.clearDisplay();
            myDisplay.drawTextCenter(64, 16, "Don't waste time more", 1);
            myDisplay.drawText(0, 48, "I spent 20 minutes", 1);
            myDisplay.drawText(0, 56, "for this...", 1);
            myDisplay.updateDisplay();
            delay(2000);
            phase=0;
        } else {
        if(anger){
            myDisplay.clearDisplay();
            delay(800);
            myDisplay.drawTextCenter(64, 16, "I told u", 1);
            myDisplay.updateDisplay();
            delay(1100);
            myDisplay.drawTextCenter(64, 26, "NO settings", 1);
            myDisplay.updateDisplay();
            delay(1100);
            myDisplay.drawTextCenter(64, 36, "in this mode", 1);
            myDisplay.updateDisplay();
            delay(2000);
            myDisplay.clearDisplay();
            delay(700);
            myDisplay.drawTextCenter(64, 16, "If settings exist", 1);
            myDisplay.drawTextCenter(64, 26, "but it says NO", 1);
            myDisplay.drawTextCenter(64, 36, "check your code", 1);
            myDisplay.updateDisplay();
            delay(2400);
            myDisplay.clearDisplay();
            delay(600);
            myDisplay.drawTextCenter(64, 16, "...or your brain", 1);
            myDisplay.updateDisplay();
            delay(2200);
            myDisplay.clearDisplay();
            phase=0;
            anger=false;
            mybuzzer.start(1900, 1500);
            lectured=true;myDisplay.drawResizedBitmap
        }else{
        if(cursor == 0) {
            myDisplay.drawTextCenter(64, 16, "No settings", 1);
        } else if (cursor==1){
            myDisplay.drawTextCenter(64, 16, "No settings", 1);
            myDisplay.drawTextCenter(64, 26, "I said No settings", 1);
        } else if(cursor==2){
            myDisplay.drawTextCenter(64, 16, "No settings", 1);
            myDisplay.drawTextCenter(64, 26, "I said No settings", 1);
            myDisplay.drawTextCenter(64, 36, "Seriously,No settings", 1);
        } else if(cursor==3){
            myDisplay.drawTextCenter(64, 26, "Go back", 2);
        } else if(cursor==4){
            myDisplay.drawTextCenter(64, 16, "why...?", 1);
        } else if(cursor==5){
            myDisplay.drawTextCenter(64, 16, "......", 1);
        } else if(cursor==6){
            myDisplay.drawTextCenter(64, 16, "Please stop", 1);
        } else if(cursor==7){
            myDisplay.drawTextCenter(64, 26, "< Back", 2);
        } else if(cursor==8){
            myDisplay.drawTextCenter(64, 16, "okey", 1);
        } else if(cursor==9){
            myDisplay.drawTextCenter(64, 16, "okey", 1);
            myDisplay.drawTextCenter(64, 26, "may u cant back", 1);
        } else if (cursor==10){
            myDisplay.drawTextCenter(64, 16, "okey", 1);
            myDisplay.drawTextCenter(64, 26, "may u cant back", 1);
            myDisplay.drawTextCenter(64, 36, "I'll do instead of u", 1);
            myDisplay.updateDisplay();
            delay(1000);
            phase = 0;
            mybuzzer.start(1600, 50);
            delay(50);
            mybuzzer.start(1600, 50);
            delay(50);
            anger=true;
        }
    }
    }
}

    if(tact==3){
        if (keysLen > 0) {
            cursor=(cursor+1)%keysLen;
        }
        mybuzzer.start(800,50);
        delay(50);
        Edit((char*)keysPtr[cursor].c_str(), &valPtr[cursor]);
    }
}

void General::Edit(char* key, float* value) {
    while (1){
    myDisplay.drawTextCenter(64, 10, "Editing...", 1);
    myDisplay.drawTextCenter(64, 20, key, 1);
    String valStr = String(*value, 2);
    myDisplay.drawTextCenter(64, 30, valStr.c_str(),1);
    myDisplay.updateDisplay();
    delay(1000);
    }
}

int General::getMode() {
    // モード取得の処理があればここに追加
    return mode; // 仮の戻り値
}

#undef UI