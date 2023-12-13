#include <M5Stack.h>

int IR_RECEIVE_PIN = 5; // set the input pin.  设置引脚
int ir_send_pin = 2;

void setup() {
  M5.begin();
  M5.Power.begin();
  M5.Lcd.print("Press Button A");
  pinMode(IR_RECEIVE_PIN, INPUT);
}

int prevValue = 1;
int currX = 0;
int currY = 10;
const int X_DELTA = 2;
const int Y_DELTA = 8;
enum { STANDBY, ACTIVE, REACHED_END } state;

void loop() {
  M5.update();
  if (M5.BtnA.wasPressed()) {
    currX = 0;
    currY = 10;
    M5.Lcd.fillScreen(0x000000);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print("Ready");
    state = STANDBY;
  }
  if (state == STANDBY || state == ACTIVE) {
    int currValue = digitalRead(IR_RECEIVE_PIN);
    if (state == STANDBY ? (currValue == 0) : state == ACTIVE) {
      if (state == STANDBY) {
        M5.Lcd.fillScreen(0x000000);
        state = ACTIVE;
      } else if (prevValue != currValue) {
        M5.Lcd.drawLine(currX, currY - Y_DELTA, currX, currY + Y_DELTA,
                        0xffffff);
      }
      if (currValue) {
        M5.Lcd.drawLine(currX, currY + Y_DELTA, currX + X_DELTA,
                        currY + Y_DELTA, 0xffffff);
      } else {
        M5.Lcd.drawLine(currX, currY - Y_DELTA, currX + X_DELTA,
                        currY - Y_DELTA, 0xffffff);
      }
      prevValue = currValue;
      currX += X_DELTA;
      // 画面サイズ：320x240
      if (currX > 320) {
        currX = 0;
        currY += 20;
      }
      if (currY > 240) {
        state = REACHED_END;
      }
    }
  }
  delayMicroseconds(100); // ここの値は適当に変える（100μs = 10kHzくらい）
}