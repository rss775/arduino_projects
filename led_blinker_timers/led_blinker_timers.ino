#define STRIP_PIN 13     // пин ленты
#define NUMLEDS 28      // кол-во светодиодов vvv.

#define BLINK_DELAY 10
#define BLINK_COLOR mRGB(128, 0, 0)
#define LIGHT_COLOR mRGB(32, 0, 0)
#define STOP_COLOR mRGB(255, 255, 0)
#define NO_COLOR mRGB(0, 0, 0)
#define LEFT_YELLOW 2
#define RIGHT_BROWN 3
#define STOP_BLUE 4
#define LIGHTS_GREEN 5

#define ENDED 1
#define NOT_ENDED 0

#define COLOR_DEBTH 3
//#include "button.h"
#include "GyverButton.h"

GButton btn_left(LEFT_YELLOW);
GButton btn_right(RIGHT_BROWN);
GButton btn_stop(STOP_BLUE);
GButton btn_lights(LIGHTS_GREEN);
int value = 0;


#include <microLED.h>   // подключаем библу
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip;

mData g_side_fill = NO_COLOR;

//byte g_counter_scroller = 0;
byte g_counter_pos = 0;
byte g_counter_value = 0;
byte g_left_flag = 0;

int left_blinker_in_flag = 0;
int left_blinker_in_ended_flag = 1;
int left_blinker_out_flag = 0;
int left_blinker_out_ended_flag = 1;
int left_blinker_out_permission_flag = 0;
int left_blinker_ended_previous = 0;
int left_blinker_ended_this_cycle = 0;


int right_blinker_in_flag = 0;
int both_blinker_in_flag = 0;
int counter_ended = 0;

//button btn_left(LEFT_YELLOW);
//button btn_right(RIGHT_BROWN);
//button btn_stop(STOP_BLUE);
//button btn_lights(LIGHTS_GREEN);

void setup() {

  btn_left.setDebounce(90);
  btn_right.setDebounce(90);
  btn_stop.setDebounce(90);
  btn_lights.setDebounce(90);
  
  btn_left.setTimeout(300);
  btn_right.setTimeout(300);
  btn_stop.setTimeout(300);
  btn_lights.setTimeout(300);
  
  btn_left.setType(HIGH_PULL);
  btn_right.setType(HIGH_PULL);
  btn_stop.setType(HIGH_PULL);
  btn_lights.setType(HIGH_PULL);

  btn_left.setDirection(NORM_OPEN);
  btn_right.setDirection(NORM_OPEN);
  btn_stop.setDirection(NORM_OPEN);
  btn_lights.setDirection(NORM_OPEN);

  btn_left.setTickMode(AUTO);
  btn_right.setTickMode(AUTO);
  btn_stop.setTickMode(AUTO);
  btn_lights.setTickMode(AUTO);
  
//  pinMode(LEFT_YELLOW, INPUT_PULLUP);
//  pinMode(RIGHT_BROWN, INPUT_PULLUP);
//  pinMode(STOP_BLUE, INPUT_PULLUP);
//  pinMode(LIGHTS_GREEN, INPUT_PULLUP);
  
  strip.setBrightness(128);
  Serial.begin(9600);
  Serial.println("started");
}

bool flag = false;
uint32_t btnTimer = 0;

void loop() {
  //g_side_fill = LIGHT_COLOR;

  //if (btn1.click()) Serial.println("press 1");



  g_side_fill = NO_COLOR;
  if (btn_lights.isHold())
    g_side_fill = LIGHT_COLOR;
  
  strip.fill(0, NUMLEDS - 1, g_side_fill);
  
  scroller(0, 0);
  
  if (btn_left.isPress())  
    left_blinker(1);
  if (btn_left.isRelease())  
  {
    
    left_blinker(2);
  }
  left_blinker(0);
  
  if (btn_right.isPress()) 
    scroller(2, 1);
  if (btn_left.isPress() && btn_right.isPress())
    scroller(3, 1);

  if (btn_stop.isHold())
  {
    stops();
    if (!btn_left.isPress() && !btn_right.isPress())
      stops_all();
  }

    
  
  

   
  
    
  //scroller(0, 0);

  counter_encrement();

  strip.show();
  delay(BLINK_DELAY);
}

void reset_counter()
{
  g_counter_pos = 0;
  g_counter_value = 0;
}

void left_blinker(int state_from_main)
{
  if (state_from_main == 1)
  {
    Serial.println("main1");
    left_blinker_in_flag = 1;
    left_blinker_out_flag = 0;
    left_blinker_in_ended_flag = NOT_ENDED;
    //left_blinker_out_ended_flag = ENDED;
    left_blinker_out_permission_flag = 1;
    reset_counter();
  }
 
  if (state_from_main == 2)
  {
    Serial.println("main2");
    if (left_blinker_in_ended_flag == ENDED)
    {
      left_blinker_in_flag = 0;
      Serial.println("left_blinker_in_ended_flag == ENDED");
    }
  }

  if (left_blinker_in_ended_flag == NOT_ENDED)
  {
    if (counter_ended && !(btn_left.state()))
    {
      Serial.println("left_blinker_in_ended_flag == NOT_ENDED");
      left_blinker_in_flag = 0;
      left_blinker_out_flag = 1;
      left_blinker_out_ended_flag = NOT_ENDED;
    }
  }

  if (left_blinker_out_ended_flag == NOT_ENDED)
  {
    if (counter_ended && (btn_left.state() == 0))
    {
      Serial.println("counter_ended && (btn_left.state())");
      left_blinker_out_flag = 1;
      left_blinker_out_ended_flag = ENDED;
    }
    if ((left_blinker_ended_previous == 1) && (btn_left.state() == 0))
    {
      Serial.println("left_blinker_out_permission_flag == 0");
      left_blinker_out_flag = 0;
      left_blinker_out_ended_flag = ENDED;
      left_blinker_out_permission_flag = 0;
    }
    if ((left_blinker_out_permission_flag == 1) && (btn_left.state() == 0))
      left_blinker_ended_previous = left_blinker_out_permission_flag;
    
  }
}

void counter_encrement(){
  g_counter_value++;
  counter_ended = 0;

  if (g_counter_value == 7 && g_counter_pos == 8)
    counter_ended = 1;

  if (g_counter_value == 8)
  {
    g_counter_pos++;
    g_counter_value = 0;
  }
  
  if (g_counter_pos == 9)
    g_counter_pos = 0;
}

void counter_set(int count_value, int count_pos)
{
    g_counter_value = count_value;
    g_counter_pos = count_pos;
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
//  byte pos;
//  byte value;
//  int this_pos;
//  int this_value;
//  int inner_in;
//  int inner_out;
//  int outter_in;
//  int outter_out;

  //mData g_this_side_fill = g_side_fill;
  //mData g_this_blink_fill = BLINK_COLOR;
  
  //pos = g_counter_scroller / 8;
  //value = g_counter_scroller % 8;


  if (left_blinker_in_flag)
  {
    left_in();
  }

  
  if (left_blinker_out_flag)
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

void stops_all() {
  static byte counter = 0;
  for (int i = 0; i < NUMLEDS; i++)
    strip.set(i, STOP_COLOR);
  //Serial.println("stops");
}
