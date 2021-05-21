#define STRIP_PIN 13     // пин ленты
#define NUMLEDS 28      // кол-во светодиодов vvv

#define BLINK_DELAY 10
#define BLINK_COLOR mRGB(128, 0, 0)
#define LIGHT_COLOR mRGB(32, 0, 0)
#define STOP_COLOR mRGB(255, 0, 0)
#define NO_COLOR mRGB(0, 0, 0)

#define COLOR_DEBTH 3
#include <microLED.h>   // подключаем библу
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip;

mData g_side_fill = NO_COLOR;

byte g_counter_scroller = 0;
byte g_counter_pos = 0;
byte g_counter_value = 0;
byte g_left_flag = 0;



void setup() {
  strip.setBrightness(128);
  Serial.begin(9600);
  Serial.println("started");
}

void loop() {
  g_side_fill = LIGHT_COLOR;
  strip.fill(0, NUMLEDS - 1, g_side_fill);

  scroller(0, 0);

  counter_encrement();

  strip.show();
  delay(BLINK_DELAY);
}

void counter_encrement(){
  g_counter_value++;
  if (g_counter_value == 8)
  {
    g_counter_pos++;
    g_counter_value = 0;
  }
  if (g_counter_pos == 9)
    g_counter_pos = 0;
}

void text(int this_pos, int this_value, char c)
{
  Serial.print(c);
  Serial.print(' ');
  Serial.print(this_pos);
  Serial.print(' ');
  Serial.print(this_value); 
  Serial.print(" ==== ");
  Serial.print(g_counter_pos);
  Serial.print(' ');
  Serial.println(g_counter_value);
}

void scroller(byte direction_nlrb, byte none_in_out){
  byte pos;
  byte value;
  int this_pos;
  int this_value;
  int inner_in;
  int inner_out;
  int outter_in;
  int outter_out;

  mData g_this_side_fill = g_side_fill;
  mData g_this_blink_fill = BLINK_COLOR;
  
  pos = g_counter_scroller / 8;
  value = g_counter_scroller % 8;

  if (direction_nlrb == 1 && none_in_out == 1)
  {
    left_in();
  }
  
  if (direction_nlrb == 1 && none_in_out == 2)
  {
    left_out();
  }

  if (direction_nlrb == 2 && none_in_out == 1)
  {
    right_in();
  }

  if (direction_nlrb == 2 && none_in_out == 2)
  {
    right_out();
  }

  if (direction_nlrb == 3 && none_in_out == 2)
  {
    left_out();
    right_out();
  }

  if (direction_nlrb == 3 && none_in_out == 1)
  {
    left_in();
    right_in();
  }
  
}

void left_in(){
  int this_pos;
  int this_value;
  
  this_pos = map(g_counter_pos, 0, 8, 8, 0); 
  this_value =  map(g_counter_value, 0, 7, 7, 0); 
  strip.fill(0, 7, g_side_fill);
  strip.fill(this_pos, 7, BLINK_COLOR);
  if (this_pos != 8)
    strip.set(this_pos, getBlend(this_value, 8, BLINK_COLOR, g_side_fill));
}

void right_in(){
  int this_pos;
  int this_value;
  
  this_pos = map(g_counter_pos, 0, 8, 19, 27); 
  this_value =  map(g_counter_value, 0, 7, 7, 0); 
  
  strip.fill(20, 27, BLINK_COLOR);
  strip.fill(this_pos, 27, g_side_fill);
  if (this_pos != 19)
    strip.set(this_pos, getBlend(this_value, 8, BLINK_COLOR, g_side_fill));
}

void left_out(){
  int this_pos;
  int this_value;
  
  this_pos = map(g_counter_pos, 0, 8, 8, 0); 
  this_value =  map(g_counter_value, 0, 7, 0, 7);
  strip.fill(0, 7, BLINK_COLOR);
  strip.fill(this_pos, 7, g_side_fill);
  if (this_pos != 8)
    strip.set(this_pos, getBlend(this_value, 8, BLINK_COLOR, g_side_fill));
  //text(this_pos, this_value, 'l');
}

void right_out(){
  int this_pos;
  int this_value;
  
  this_pos = map(g_counter_pos, 0, 8, 19, 27); 
  this_value =  map(g_counter_value, 0, 7, 0, 7);
  strip.fill(20, 27, g_side_fill);
  strip.fill(this_pos, 27, BLINK_COLOR);
  if (this_pos != 19)
    strip.set(this_pos, getBlend(this_value, 8, BLINK_COLOR, g_side_fill));
  //text(this_pos, this_value, 'r');
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
