#include "General.h"
void General::setup() {
    Serial.begin(115200);
    mybuzzer.start(400, 100);
    delay(50);
    mybuzzer.start(300, 100);
    myDisplay.setup();
    myDisplay.clearDisplay();
    myDisplay.drawText(0,0,"VEGA",2);
    myDisplay.drawText(0,20,"displaydone",1);
    myDisplay.drawText(0,30,"switch<-",1);
    myDisplay.updateDisplay();
	mySwitch.setup(); // スイッチの初期化
    myDisplay.clearDisplay();
    myDisplay.drawText(0,0,"VEGA",2);
    myDisplay.drawText(0,20,"displaydone",1);
    myDisplay.drawText(0,30,"switch done",1);
    myDisplay.drawText(0,40,"buzzer<-",1);
    myDisplay.updateDisplay();
	mybuzzer.setup(); // ブザーの初期化
    myDisplay.clearDisplay();
    myDisplay.drawText(0,0,"VEGA",2);
    myDisplay.drawText(0,20,"displaydone",1);
    myDisplay.drawText(0,30,"switch done",1);
    myDisplay.drawText(0,40,"buzzer done",1);
    myDisplay.updateDisplay();
	myDisplay.preset(1); // 画面プリセット
	mybuzzer.preset(1); // ブザープリセット
}
void General::startUp() {
    bool exit=true;
    pushedNext=false;
    generalTimer.reset(); // ここで一度だけリセット
    while(exit){
        myDisplay.clearDisplay();
        tact=mySwitch.checkTactSwitch();
        toggle=mySwitch.checkToggleSwitch();

        myDisplay.preset(3);//枠線を描画
        Serial.println(generalTimer.read_milli());
        if(!pushedNext&&generalTimer.read_milli()<1000){//1秒後に自動でモードセレクト表示
        myDisplay.drawText(32, 3, "Mode select", 1);
        } else {
            switch (mode)
            {
            case 0:
                myDisplay.drawText(38, 3, " Attack ", 1);
                break;
            case 1:
                myDisplay.drawText(36, 3, " Defense ", 1);
                break;
            case 2:
                myDisplay.drawText(38, 3, "  Test  ", 1);
                break;
            default:
                myDisplay.drawText(36, 3, "Mode ? err", 1);
                break;
            }
        }

        if(tact>=100){//操作表示
            myDisplay.drawText(2,54,"------",1);
            myDisplay.drawText(46,54,"------",1);
            myDisplay.drawText(91,54,"------",1);
        } else {
            myDisplay.drawText(2,54,"Adjust",1);
            myDisplay.drawText(46,54,"Select",1);
            myDisplay.drawText(91,54,"next >",1);
        }
        {
            myDisplay.drawRectangleFromCenter(64,32,32,32,false);
            myDisplay.drawRectangleFromCenter(22,32,22,22,false);
            myDisplay.drawRectangleFromCenter(106,32,22,22,false);
        }

        if(toggle){//右上のトグル
            myDisplay.drawRectangle(116,4,3,4,true);
        } else {
            myDisplay.drawRectangle(116,8,3,4,true);
        }

        {//ボタンぷにぷに
            int modT = tact>=100 ? tact-100:tact;
            if(modT==1||modT==4||modT==6||modT==9){
                myDisplay.drawLine(5,50,33,50);
                myDisplay.drawLine(10,49,28,49);
            }
            if(modT==3||modT==4||modT==8||modT==9){
                myDisplay.drawLine(45,50,82,50);
                myDisplay.drawLine(50,49,77,49);
            }
            if(modT==5||modT==6||modT==8||modT==9){
                myDisplay.drawLine(94,50,122,50);
                myDisplay.drawLine(99,49,117,49);
            }
        }
        myDisplay.updateDisplay();
        if(tact==5){
            pushedNext=true;
            mode=(mode+1)%3;
            myDisplay.setGeneralDisplayMode(mode);
            myDisplay.preset(4);
        }
    }
}
int General::getMode() {
    // モード取得の処理があればここに追加
    return mode; // 仮の戻り値
}