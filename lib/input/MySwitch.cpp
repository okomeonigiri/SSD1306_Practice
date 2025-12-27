#include "MySwitch.h"

void MySwitch::setup() {
    pinMode(LAYER_SWITCH, INPUT_PULLUP); // レイヤースイッチ
    pinMode(TACT_LEFT, INPUT_PULLUP); // タクトスイッチ
    pinMode(TACT_CENTER, INPUT_PULLUP); // タクトスイッチ
    pinMode(TACT_RIGHT, INPUT_PULLUP); // タクトスイッチ
    pinMode(TOGGLE_SWITCH, INPUT_PULLUP); // トグルスイッチ
}

int MySwitch::checkTactSwitch() {
    int State = 0;
    // タクトスイッチの状態をチェックして処理を行う
    if (digitalRead(LAYER_SWITCH) == LOW) {
        State += 100; // レイヤースイッチが押された
    }
    if (digitalRead(TACT_LEFT) == LOW) {
        State += 1; // スイッチ1が押された
    }
    if (digitalRead(TACT_CENTER) == LOW) {
        State += 3; // スイッチ2が押された
    }
    if (digitalRead(TACT_RIGHT) == LOW) {
        State += 5; // スイッチ3が押された
    }
    return State;
}
bool MySwitch::checkToggleSwitch() {
    // トグルスイッチの状態をチェックして処理を行う
    bool State = false;
    State = (digitalRead(TOGGLE_SWITCH) == LOW); // LOWならON、HIGHならOFF
    return State;
}