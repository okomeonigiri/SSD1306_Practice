#include "Mybuzzer.h"


void MyBUZZER::setup() {
    pinMode(BUZZER_PIN, OUTPUT);
}

void MyBUZZER::start(int BUZZERnote, int BUZZERduration,bool unblock) {
    if (useBUZZER == 1) {
        if (BUZZERduration != 999) {
            if(!unblock){
                tone(BUZZER_PIN, BUZZERnote, BUZZERduration);
                delay(BUZZERduration*1.1);
                noTone(BUZZER_PIN);
            } else {
                tone(BUZZER_PIN, BUZZERnote, BUZZERduration);
            }
        } else {
            tone(BUZZER_PIN, BUZZERnote);
        }
    }
}

void MyBUZZER::stop() {
    noTone(BUZZER_PIN);
}

void MyBUZZER::preset(int BUZZERpresetNUM) {
    if (useBUZZER == 1) {
        if (BUZZERpresetNUM == 0) {
            int Melody_Preset1[] = {260, 260, 260, 255, 0, 300, 250, 330};
            int NoteDurs[] = {7, 7, 7, 7, 14, 4, 5, 3};
            int Size_Melody_Preset1 = sizeof(Melody_Preset1)/sizeof(Melody_Preset1[0]);
            for (int playing_Note = 0; playing_Note < Size_Melody_Preset1; playing_Note++) {
                    int noteDur = 1000 / NoteDurs[playing_Note];
                    tone(BUZZER_PIN, Melody_Preset1[playing_Note], noteDur);
                    int PBTWNotes = noteDur * 1.4;
                    delay(PBTWNotes);
                    noTone(BUZZER_PIN);
            }
            delay(100);
        } else if (BUZZERpresetNUM == 1) {
            int Melody_Preset1[] = {350, 360, 380};
            int NoteDurs[] = {8, 8, 8};
            int Size_Melody_Preset1 = sizeof(Melody_Preset1)/sizeof(Melody_Preset1[0]);
            for (int playing_Note = 0; playing_Note < Size_Melody_Preset1; playing_Note++) {
                    int noteDur = 1000 / NoteDurs[playing_Note];
                    tone(BUZZER_PIN, Melody_Preset1[playing_Note], noteDur);
                    int PBTWNotes = noteDur * 1.4;
                    delay(PBTWNotes);
                    noTone(BUZZER_PIN);
            }
            delay(100);
        }
    }
}