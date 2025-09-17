#include "MySwitch.h"

void MySwitch::setup() {
    pinMode(22, INPUT_PULLUP); // レイヤースイッチ
    pinMode(24, INPUT_PULLUP); // タクトスイッチ
    pinMode(26, INPUT_PULLUP); // タクトスイッチ
    pinMode(28, INPUT_PULLUP); // タクトスイッチ
    pinMode(30, INPUT_PULLUP); // トグルスイッチ
}

int MySwitch::checkTactSwitch() {
    int State = 0;
    // タクトスイッチの状態をチェックして処理を行う
    if (digitalRead(22) == LOW) {
        State += 100; // レイヤースイッチが押された
    }
    if (digitalRead(24) == LOW) {
        State += 1; // スイッチ1が押された
    }
    if (digitalRead(26) == LOW) {
        State += 3; // スイッチ2が押された
    }
    if (digitalRead(28) == LOW) {
        State += 5; // スイッチ3が押された
    }
    return State;
}
bool MySwitch::checkToggleSwitch() {
    // トグルスイッチの状態をチェックして処理を行う
    bool State = false;
    State = (digitalRead(30) == LOW); // LOWならON、HIGHならOFF
    return State;
}