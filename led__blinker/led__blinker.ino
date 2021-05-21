// простейшие динамические эффекты
// сначала ознакомься с примером microLED_guide !!!

#define STRIP_PIN 13     // пин ленты
#define NUMLEDS 28      // кол-во светодиодов
#define R_BLINK 20      // кол-во светодиодов
#define L_BLINK 8
#define BLINK_DELAY 100
#define BLINK_COLOR mRGB(255, 0, 0)
#define LIGHT_COLOR mRGB(32, 0, 0)
#define STOP_COLOR mRGB(255, 0, 0)
#define NO_COLOR mRGB(0, 0, 0)

#define COLOR_DEBTH 3
#include <microLED.h>   // подключаем библу
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip;

int light_status = 0;
int blink_status = 0;
int left_blink_status = 0;
byte left_counter = 0;

void setup() {
  strip.setBrightness(128);
  Serial.begin(9600);
  Serial.println("started");
}

void loop() {

  if (Serial.available()) {       // есть что на вход?
    int ret = Serial.parseInt(); // принять в переменную buff
    light_status = ret / 10;
    blink_status = ret % 10;
    Serial.println(ret);
  }
  


  if (light_status == 0)  no_lights();
  if (light_status == 1)  lights();
  if (light_status == 2)  stops();
  if (blink_status == 1)  
  {
    //left_counter = L_BLINK - 1;
    blink_left();
  }
  if (blink_status == 2)  blink_right();
  if ((light_status == 0) && (blink_status == 0))  off_all();
  if ((light_status == 1) && (blink_status == 0)) lights_all();

  strip.show();   // вывод
  delay(30);      // 30 кадров в секунду
}



void blink_left() {
  //left_counter = L_BLINK - 1;
  //for (int i = 0; i < 8; i++) strip.leds[i] = strip.leds[i + 1];
  //strip.leds[8] = BLINK_COLOR;
  strip.leds [left_counter] = BLINK_COLOR;
  
  if (left_counter == 255)
  {
    left_counter = L_BLINK;
    for (int j = L_BLINK; j >= 0; j--)
    strip.set(j, LIGHT_COLOR);
    blink_status = 1;
  }
  left_counter--;

  if (blink_status != 1)  
  {
    //left_counter = L_BLINK - 1;
    left_counter = L_BLINK - 1;
  }
  
  Serial.println("bl");
  Serial.println(left_counter);
  delay(BLINK_DELAY);   // дополнительная задержка
}

void blink_right() {
  int i = R_BLINK;
  while (i < NUMLEDS)
  {
    strip.set(i, BLINK_COLOR);
    i++;
    
  }
  Serial.println(i);
  if (i == NUMLEDS)
    light_status = 1;
  Serial.println("br");
  delay(BLINK_DELAY);   // дополнительная задержка
}

void no_lights() {
  static byte counter = 0;
  for (int i = 0; i < NUMLEDS; i++)
    strip.set(i, NO_COLOR);
  //Serial.println("no_lights");
}

void off_all() {
  static byte counter = 0;
  for (int i = 0; i < NUMLEDS; i++)
    strip.set(i, NO_COLOR);
  //Serial.println("no_lights");
}

void lights() {
  static byte counter = 0;
  for (int i = 8; i <= 20; i++)
    strip.set(i, LIGHT_COLOR);
  //Serial.println("lights");
}

void lights_all() {
  static byte counter = 0;
  for (int i = 0; i < NUMLEDS; i++)
    strip.set(i, LIGHT_COLOR);
  //Serial.println("lights");
}

void stops() {
  static byte counter = 0;
  for (int i = 8; i <= 20; i++)
    strip.set(i, STOP_COLOR);
  //Serial.println("stops");
}

void breathing() {
  static int dir = 1;
  static int bright = 0;
  bright += dir * 5;    // 5 - множитель скорости изменения

  if (bright > 255) {
    bright = 255;
    dir = -1;
  }
  if (bright < 0) {
    bright = 0;
    dir = 1;
  }
  strip.setBrightness(bright);
}
