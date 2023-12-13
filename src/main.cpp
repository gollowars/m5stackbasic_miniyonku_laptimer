#include <M5Stack.h>

TFT_eSprite img = TFT_eSprite(&M5.Lcd);

// const
int LIGHT_ANALOG_PIN = 35;
int LIGHT_DIGITAL_PIN = 26;

bool is_timer_ready = false;
uint16_t digitalRead_value = 0, analogRead_value = 0;
uint16_t digitalRead_prev_value = 0, analogRead_prev_value = 0;

// 時間保存
const int arraySize = 21;
unsigned long timeArray[arraySize];
int currentIndex = 0;

// header
void setupLightSesor();
void loopButtonEvent();
void updateLightSensor();
void OnDetectLightOff();
void startMeasureTime();
void endMeasureTime();

// body
void setupLightSesor() { pinMode(LIGHT_DIGITAL_PIN, INPUT); }

void updateLightSensor() {
  analogRead_value =
      analogRead(LIGHT_ANALOG_PIN); // Store the analog quantity read from
                                    // pin 36. 将36号引脚读取到的模拟量存储
  digitalRead_value =
      digitalRead(LIGHT_DIGITAL_PIN); // Store the number read from pin 26.
                                      // 将26号引脚读取到的数字量存储
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Analog:%d\n", analogRead_value);
  M5.Lcd.setCursor(160, 0);
  M5.Lcd.printf("Digital:%d\n", digitalRead_value);

  // M5.lcd.setCursor(0, 80);
  // M5.lcd.print("TESTTEST");
}

void loopButtonEvent() {
  M5.update(); // Read the press state of the key.  读取按键 A, B, C 的状态

  if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {
    // M5.Lcd.print('A');
    startMeasureTime();
  } else if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200)) {
    // M5.Lcd.print('B');
    endMeasureTime();
  } else if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(1000, 200)) {
    // M5.Lcd.print('C');
  }

  if (is_timer_ready == true) {
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.printf("Ready");
  }
}

void startMeasureTime() {
  is_timer_ready = true;
  currentIndex = 0;
  for (int i = 0; i < arraySize; i++) {
    timeArray[i] = 0;
  }

  M5.Lcd.clear();
}

void endMeasureTime() { is_timer_ready = false; }

void OnDetectLightOff() {

  if (currentIndex < arraySize) {
    timeArray[currentIndex] = millis();

    float sum;
    if (currentIndex >= 1) {
      for (size_t i = 0; i < currentIndex; i++) {

        float time = (timeArray[i + 1] - timeArray[i]) / 1000.0f;
        int x = (i > 9) ? 160 : 0;
        int y = (i > 9) ? (i - 10) * 20 : i * 20;
        M5.Lcd.setCursor(x, 40 + y);
        M5.Lcd.printf("Time%d: %.3f\n", i + 1, time);

        sum += time;
      }

      float avg = sum / currentIndex;
      M5.Lcd.setCursor(160, 20);
      M5.Lcd.printf("Avg: %.3f\n", avg);
    }

    currentIndex++;

  } else {
  }
}

// lifecyecle
void setup() {
  M5.begin();       // Init M5Stack.  初始化M5Stack
  M5.Power.begin(); // Init power  初始化电源模块
  M5.Speaker.begin();
  M5.Speaker.mute();

  // M5.Lcd.setColorDepth(8);
  // M5.Lcd.createSprite(320, 240); // 创建一块320x240的画布
  M5.Lcd.setTextSize(2);

  setupLightSesor();
}

void loop() {
  // M5.Lcd.fillRect(0, 0, 320, 240, 0x00);

  loopButtonEvent();
  updateLightSensor();

  // M5.Lcd.pushSprite(0, 0); // 把画布推送到屏幕(0,0)处

  if (is_timer_ready == true && digitalRead_value == 1 &&
      digitalRead_prev_value == 0) {
    OnDetectLightOff();
  }

  digitalRead_prev_value = digitalRead_value;

  delay(10);
}