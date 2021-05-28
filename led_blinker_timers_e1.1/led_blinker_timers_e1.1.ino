#define STRIP_PIN 13     // пин ленты
#define NUMLEDS 28      // кол-во светодиодов vvv.

#define BLINK_DELAY 10
#define BLINK_COLOR mRGB(64, 0, 64)  //заменить
#define LIGHT_COLOR mRGB(32, 0, 0)  //заменить
#define STOP_COLOR mRGB(128, 128, 0)  //заменить
#define NO_COLOR mRGB(0, 0, 0)
#define LEFT_YELLOW 2
#define RIGHT_BROWN 3
#define STOP_BLUE 4
#define LIGHTS_GREEN 5

#define ENDED 1
#define NOT_ENDED 0

#define COLOR_DEBTH 3
#define ARRAY_SIZE 8

#include "GyverButton.h"
#include "TimerOne.h"


byte bytes_left_in[ARRAY_SIZE];
byte bytes_right_in[ARRAY_SIZE];

GButton btn_left(LEFT_YELLOW);
GButton btn_right(RIGHT_BROWN);
GButton btn_stop(STOP_BLUE);
GButton btn_lights(LIGHTS_GREEN);
//int value = 0;


#include <microLED.h>   // подключаем библу
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip;

mData g_side_fill = NO_COLOR;
mData g_inner_fill = NO_COLOR;


byte g_counter_pos = 0;
byte g_counter_value = 0;
//byte g_left_flag = 0;


int state_left_button = 0;
int left_in_ended = 1;
int left_out_ended = 1;
int left_out_printed = 1;
int left_blink_value = 6;
int left_done = 1;

int state_right_button = 0;
int right_in_ended = 1;
int right_out_ended = 1;
int right_out_printed = 1;
int right_blink_value = 6;
int right_done = 1;

int start_flag = 1;

int counter_ended = 0;

bool flag = false;
uint32_t btnTimer = 0;

void setup() {
  Timer1.initialize(10000);           // установка таймера на каждые 10000 микросекунд (== 10 мс)
  Timer1.attachInterrupt(timerIsr); 
  
  btn_left.setDebounce(10);
  btn_right.setDebounce(10);
  btn_stop.setDebounce(10);
  btn_lights.setDebounce(10);





//  btn_left.setTickMode(AUTO);
//  btn_right.setTickMode(AUTO);
//  btn_stop.setTickMode(AUTO);
//  btn_lights.setTickMode(AUTO);

  //  pinMode(LEFT_YELLOW, INPUT_PULLUP);
  //  pinMode(RIGHT_BROWN, INPUT_PULLUP);
  //  pinMode(STOP_BLUE, INPUT_PULLUP);
  //  pinMode(LIGHTS_GREEN, INPUT_PULLUP);

  strip.setBrightness(128);
  Serial.begin(9600);
  Serial.println("started");

  for (byte i = 0; i < ARRAY_SIZE; i++)
    updateArray(2, bytes_left_in);

  for (byte i = 0; i < ARRAY_SIZE; i++)
    updateArray(2, bytes_right_in);


}


void timerIsr() {
  btn_left.tick();  // опрашиваем в прерывании, чтобы поймать нажатие в любом случае
  btn_right.tick();
  btn_lights.tick();
  btn_stop.tick();
}

void loop() {
  
  if (btn_left.isPress())
  {
    if (start_flag == 1)
    {
      //updateArray(1, bytes_left_in);
      reset_counter();
    }
    if (left_in_ended)
      reset_counter();
    start_flag = 0;
  }
  if (start_flag == 1 && btn_left.state())
  {
      //updateArray(1, bytes_left_in);
      reset_counter();
  }
//  if (btn_left.state())
//  {
//    updateArray(1, bytes_left_in);
//  }



  


  left_done = 1;
  right_done = 1;

  g_side_fill = NO_COLOR;
  if (btn_lights.state())
    g_side_fill = LIGHT_COLOR;

  strip.fill(0, NUMLEDS - 1, g_side_fill);

  g_inner_fill = NO_COLOR;
  if (btn_lights.state())
    g_inner_fill = LIGHT_COLOR;
  if (btn_stop.state())
    g_inner_fill = STOP_COLOR;

  if (btn_stop.state())
  {
    if (left_done == 1)
      fill_left(STOP_COLOR);
    if (g_counter_pos == 8 && g_counter_value == 7)
      fill_left(g_side_fill);
    if (right_done == 1)
      fill_right(STOP_COLOR);
    if (g_counter_pos == 8 && g_counter_value == 7)
      fill_right(g_side_fill);
  }

  left_blinker();
  left_blinker_loop();

  right_blinker();
  right_blinker_loop();

  fill_middle(g_inner_fill);
  counter_encrement();

  //text(9, 9, 'd');
  strip.show();
  delay(BLINK_DELAY);

}

void updateArray(int newVal, byte *bytes) {
  for (byte i = 0; i < ARRAY_SIZE - 1; i++) {
    // сдвигаем члены влево
    bytes[i] = bytes[i + 1];
  }
  // пишем новое значение в последний элемент
  bytes[ARRAY_SIZE - 1] = newVal;
  //Serial.println("update array");
  printArray (bytes);
}

void printArray(byte *bytes) {
  // выводим в порт
  for (byte i = 0; i < ARRAY_SIZE; i++) {
    Serial.print(bytes[i]);
    Serial.print('\t');
  }
  Serial.println();
}

void reset_counter()
{
  g_counter_pos = 0;
  g_counter_value = 0;
  //Serial.println("reset");
}

void left_blinker()
{
  if (btn_left.isPress())
  {
    left_blink_value = 1;

    if (bytes_left_in[ARRAY_SIZE - 1] != 1)
      reset_counter();

    updateArray(left_blink_value, bytes_left_in);
    left_in();
  }
  if (counter_ended)
  {
    if (btn_left.isHold())
    {
      left_blink_value = 1;
      updateArray(left_blink_value, bytes_left_in);

    }
    else
    {
      left_blink_value = 2;
      if ((bytes_left_in[ARRAY_SIZE - 1] == 0))
        left_blink_value = 0;
    }
    updateArray(left_blink_value, bytes_left_in);
  }
}

void left_blinker_loop()
{

  if ((bytes_left_in[ARRAY_SIZE - 1] == 1))
  {
    left_in_ended = 0;
    left_out_ended = 0;
    if (counter_ended)
    {
      left_out_ended = 1;
    }
  }


  if (counter_ended)
  {

    left_in_ended = 1; // важно
  }
  if (bytes_left_in[ARRAY_SIZE - 1] == 2 && bytes_left_in[ARRAY_SIZE - 2] == 2)
    updateArray(0, bytes_left_in);
  if (bytes_left_in[ARRAY_SIZE - 1] == 0)
  {
    left_out_ended = 1;
    left_out_printed = 0;
  }
  if (bytes_left_in[ARRAY_SIZE - 1] == 2 && bytes_left_in[ARRAY_SIZE - 2] == 2)
    bytes_left_in[ARRAY_SIZE - 1] = 3;



  if ((bytes_left_in[ARRAY_SIZE - 1]) != 1)
  {
    if (counter_ended)
    {
      left_in_ended = 1;
    }
  }
  if ((bytes_left_in[ARRAY_SIZE - 1] == 0) )
    fill_left(g_inner_fill);
  if (!left_in_ended)
  {
    //Serial.println("in");
    left_in();
    //text('i');
  }
  //text('n');
  //Serial.println(!left_in_ended);

  if ((left_in_ended) && bytes_left_in[ARRAY_SIZE - 1] == 2)
  {

    if (g_counter_pos == 8 && g_counter_value == 7)
    {
      g_counter_pos = 0;
      g_counter_value = 1;
      //fill_left(STOP_COLOR);
    }
    left_out();
    //text('o');

    if (counter_ended)
    {
      left_out_printed = 1;
      //Serial.println("out");
    }
  }

}

void right_blinker()
{
  if (btn_right.isPress())
  {
    right_blink_value = 1;

    if (bytes_right_in[ARRAY_SIZE - 1] != 1)
      reset_counter();

    updateArray(right_blink_value, bytes_right_in);
    right_in();
  }
  if (counter_ended)
  {
    if (btn_right.isHold())
    {
      right_blink_value = 1;
      updateArray(right_blink_value, bytes_right_in);

    }
    else
    {
      right_blink_value = 2;
      if ((bytes_right_in[ARRAY_SIZE - 1] == 0))
        right_blink_value = 0;
    }
    updateArray(right_blink_value, bytes_right_in);
  }
}

void right_blinker_loop()
{

  if ((bytes_right_in[ARRAY_SIZE - 1] == 1))
  {
    right_in_ended = 0;
    right_out_ended = 0;
    if (counter_ended)
    {
      right_out_ended = 1;
    }
  }


  if (counter_ended)
  {

    right_in_ended = 1; // важно
  }
  if (bytes_right_in[ARRAY_SIZE - 1] == 2 && bytes_right_in[ARRAY_SIZE - 2] == 2)
    updateArray(0, bytes_right_in);
  if (bytes_right_in[ARRAY_SIZE - 1] == 0)
  {
    right_out_ended = 1;
    right_out_printed = 0;
  }
  if (bytes_right_in[ARRAY_SIZE - 1] == 2 && bytes_right_in[ARRAY_SIZE - 2] == 2)
    bytes_right_in[ARRAY_SIZE - 1] = 3;



  if ((bytes_right_in[ARRAY_SIZE - 1]) != 1)
  {
    if (counter_ended)
    {
      right_in_ended = 1;
    }
  }
  if ((bytes_right_in[ARRAY_SIZE - 1] == 0) )
    fill_right(g_inner_fill);
  if (!right_in_ended)
  {
    //Serial.println("in");
    right_in();
    //text('i');
  }
  //text('n');
  //Serial.println(!right_in_ended);

  if ((right_in_ended) && bytes_right_in[ARRAY_SIZE - 1] == 2)
  {

    if (g_counter_pos == 8 && g_counter_value == 7)
    {
      g_counter_pos = 0;
      g_counter_value = 1;
      //fill_right(STOP_COLOR);
    }
    right_out();
    //text('o');

    if (counter_ended)
    {
      right_out_printed = 1;
      //Serial.println("out");
    }
  }

}




void counter_encrement() {
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

void text2(char c)
{
  Serial.print(c);
  Serial.print(' ');
  Serial.print(g_counter_pos);
  Serial.print(' ');
  Serial.println(g_counter_value);
}

void text(int a, int b, char c)
{
  Serial.print(c);
  Serial.print(' ');
  Serial.print(a);
  Serial.print(' ');
  Serial.print(b);
  Serial.print(" ==== ");
  Serial.print(' ');
  Serial.print(g_counter_pos);
  Serial.print(' ');
  Serial.println(g_counter_value);
}


void left_in() {
  int this_pos;
  int this_value;

  this_pos = map(g_counter_pos, 0, 8, 8, 0);
  this_value =  map(g_counter_value, 0, 7, 7, 0);
  strip.fill(0, 7, g_side_fill);
  strip.fill(this_pos, 7, BLINK_COLOR);
  if (this_pos != 8)
    strip.set(this_pos, getBlend(this_value, 8, BLINK_COLOR, g_side_fill));
  //text(this_pos, this_value, 'i');
  left_done = 0;
}

void left_out() {
  int this_pos;
  int this_value;

  this_pos = map(g_counter_pos, 0, 8, 8, 0);
  this_value =  map(g_counter_value, 0, 7, 0, 7);
  strip.fill(0, 7, BLINK_COLOR);
  strip.fill(this_pos, 7, g_inner_fill);
  if (this_pos != 8)
    strip.set(this_pos, getBlend(this_value, 8, BLINK_COLOR, g_inner_fill));
  //text(this_pos, this_value, 'o');
  left_done = 0;
}

void right_in() {
  int this_pos;
  int this_value;

  this_pos = map(g_counter_pos, 0, 8, 19, NUMLEDS - 1);
  this_value =  map(g_counter_value, 0, 7, 7, 0);

  strip.fill(20, NUMLEDS - 1, BLINK_COLOR);
  strip.fill(this_pos, NUMLEDS - 1, g_side_fill);
  if (this_pos != 19)
    strip.set(this_pos, getBlend(this_value, 8, BLINK_COLOR, g_side_fill));
  right_done = 0;

}



void right_out() {
  int this_pos;
  int this_value;

  this_pos = map(g_counter_pos, 0, 8, 19, NUMLEDS - 1);
  this_value =  map(g_counter_value, 0, 7, 0, 7);
  strip.fill(20, NUMLEDS - 1, g_inner_fill);
  strip.fill(this_pos, NUMLEDS - 1, BLINK_COLOR);
  if (this_pos != 19)
    strip.set(this_pos, getBlend(this_value, 8, BLINK_COLOR, g_inner_fill));
  //text(this_pos, this_value, 'r');
  right_done = 0;
}

void fill_middle(mData g_middle_fill) {
  static byte counter = 0;
  for (int i = 8; i < 20; i++)
    strip.set(i, g_middle_fill);
  //Serial.println("stops");
}

void fill_left(mData g_middle_fill) {
  static byte counter = 0;
  //if (start_flag == 0)
  {
    for (int i = 0; i < 8; i++)
      strip.set(i, g_middle_fill);
    //Serial.println("fill left");
  }
}

void fill_right(mData g_middle_fill) {
  static byte counter = 0;
  if (start_flag == 0)
  {
    for (int i = 20; i < NUMLEDS; i++)
      strip.set(i, g_middle_fill);
    //Serial.println("stops");
  }
}
