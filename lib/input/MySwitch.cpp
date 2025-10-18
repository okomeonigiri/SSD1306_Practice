#include "MySwitch.h"

void MySwitch::setup() {
    pinMode(30, INPUT_PULLUP); // レイヤースイッチ
    pinMode(32, INPUT_PULLUP); // タクトスイッチ
    pinMode(31, INPUT_PULLUP); // タクトスイッチ
    pinMode(34, INPUT_PULLUP); // タクトスイッチ
    pinMode(33, INPUT_PULLUP); // トグルスイッチ
}

int MySwitch::checkTactSwitch() {
    int State = 0;
    // タクトスイッチの状態をチェックして処理を行う
    if (digitalRead(30) == LOW) {
        State += 100; // レイヤースイッチが押された
    }
    if (digitalRead(32) == LOW) {
        State += 1; // スイッチ1が押された
    }
    if (digitalRead(31) == LOW) {
        State += 3; // スイッチ2が押された
    }
    if (digitalRead(34) == LOW) {
        State += 5; // スイッチ3が押された
    }
    return State;
}
bool MySwitch::checkToggleSwitch() {
    // トグルスイッチの状態をチェックして処理を行う
    bool State = false;
    State = (digitalRead(33) == LOW); // LOWならON、HIGHならOFF
    return State;
}