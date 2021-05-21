#ifndef FT_MAIN_H
#define FT_MAIN_H

#include  <LiquidCrystal.h> //подключаем библиотеку
#include "GyverTimers.h"

const int menuItems = 16;

const char menu0 [18] =  {"24:00 36C fan:au\0"};
const char menu1 [18] =  {"11:11 25%  lt:au\0"};
const char menu2 [18] =  {"time hh         \0"};
const char menu3 [18] =  {"time mm         \0"};
const char menu4 [18] =  {"light on at     \0"};
const char menu5 [18] =  {"light off at    \0"};
const char menu6 [18] =  {"wet min         \0"};
const char menu7 [18] =  {"wet mid         \0"};
const char menu8 [18] =  {"wet max         \0"};
const char menu9 [18] =  {"temp min        \0"};
const char menu10 [18] = {"temp mid        \0"};
const char menu11 [18] = {"temp max        \0"};
const char menu12 [18] = {"night hh        \0"};
const char menu13 [18] = {"night mm        \0"};
const char menu14 [18] = {"timer low hh    \0"};
const char menu15 [18] = {"timer low mm    \0"};

const char firstLine0 [18] =  {"                \0"};
const char firstLine1 [18] =  {"                \0"};
const char secondLine0 [18] =  {"                \0"};
const char secondLine1 [18] =  {"                \0"};

LiquidCrystal lcd(8, 9, 4, 5, 6, 7 );
int button;  //вводим числовые значения для кнопок
const int BUTTON_NONE   = 0; //присваиваем постоянное значение для BUTTON_NONE 
const int BUTTON_RIGHT  = 1; //присваиваем постоянное значение для BUTTON_RIGHT
const int BUTTON_UP     = 2; //присваиваем постоянное значение для BUTTON_UP
const int BUTTON_DOWN   = 3; //присваиваем постоянное значение для BUTTON_DOWN 
const int BUTTON_LEFT   = 4; //присваиваем постоянное значение для BUTTON_LEFT
const int BUTTON_SELECT = 5; //присваиваем постоянное значение для BUTTON_SELECT

const char* const names[] =
{
  menu0, menu1, menu2, menu3,
  menu4, menu5, menu6, menu7,
  menu8, menu9, menu10, menu11,
  menu12, menu13, menu14, menu15,  
};

const char* const firstLine[] =
{
  firstLine0, firstLine1,  
};

const char* const secondLine[] =
{
  secondLine0, secondLine1,  
};

static uint32_t tmr;
static uint32_t tmr2;

#endif