#include <M5Stack.h>

int ir_recv_pin = 5; // set the input pin.  设置引脚
int ir_send_pin = 2;

int last_recv_value = 0;
int cur_recv_value = 0;

// header
void setupIR();
void loopButton();

// body
void setup() {

  // シリアル通信の初期化
  Serial.begin(115200);

  // M5Stackの初期化
  M5.begin();
  M5.Power.begin();

  setupIR();
}

void setupIR() {
  // IRの初期化
  pinMode(ir_recv_pin, INPUT);
  pinMode(ir_send_pin, OUTPUT);

  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("Test for IR receiver: ");
}

void loopButton() {
  M5.update(); // Read the press state of the key.  读取按键 A, B, C 的状态

  M5.Lcd.setCursor(0, 30);

  if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {
    M5.Lcd.print('1');
    digitalWrite(ir_send_pin, 1);
  } else if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200)) {
    M5.Lcd.print('2');
    digitalWrite(ir_send_pin, 0);
  } else if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(1000, 200)) {
    M5.Lcd.print('3');
    digitalWrite(ir_send_pin, 0.5);
  } else if (M5.BtnB.wasReleasefor(700)) {
    M5.Lcd.clear(WHITE); // Clear the screen and set white to the
                         // background color.  清空屏幕并将白色设置为底色
    M5.Lcd.setCursor(0, 0);
  } else if (M5.BtnC.wasReleasefor(700)) {
    M5.Lcd.clear(BLACK); // Clear the screen and set black to the
                         // background color.  清空屏幕并将黑色设置为底色
    M5.Lcd.setCursor(0, 0);
  }
}

void loop() {
  loopButton();

  // now, once you press the button on a remote controller to send infrared
  // light.  现在，一旦你按下遥控器上的按钮发送红外线 the screen will display
  // "detected!"  屏幕将显示“检测到!”
  // digitalWrite(ir_send_pin, 1);

  cur_recv_value = digitalRead(ir_recv_pin);
  if (last_recv_value != cur_recv_value) {
    M5.Lcd.setCursor(0, 60);
    // M5.Lcd.fillRect(0, 25, 150, 25, BLACK);

    M5.Lcd.print("detected! : " + String(cur_recv_value));

    // if (cur_recv_value ==
    //     0) { // 0: detected 1: not detected,  0检测到,1没有检测到

    // }
    last_recv_value = cur_recv_value;
  }
  Serial.println(cur_recv_value);
}