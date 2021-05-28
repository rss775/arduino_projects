#ifndef FT_MAIN_H
#define FT_MAIN_H

#define DHTPIN 2 // номер пина, к которому подсоединен датчик
//#define RTC_CLK 11
//#define RTC_DAT 12
//#define RTC_RST 13

#include  <LiquidCrystal.h> //подключаем библиотеку
#include "GyverTimers.h"
#include <EEPROM.h>
#include "DHT.h"
#include <iarduino_RTC.h>
iarduino_RTC time(RTC_DS1307);


DHT dht(DHTPIN, DHT22);
//DHT dht(DHTPIN, DHT11);

#define INIT_ADDR 1023  // номер резервной ячейки
#define INIT_KEY 50     // ключ первого запуска. 0-254, на выбор
#define SAVE_TIMER_MSEC 600000 // период сохранения настроек 600000
#define MINUTE_IS 60000  //минута должна быть 60000, иначе отладка 1000

#define SENSOR_TIMER 1000                     //опрос датчика влаги 60000
//#define SWITCH_FAN_TIMER 100000                 // самый рабочий, тикер в 1000
//#define SWITCH_LIGHT_TIMER 1000               //самый рабочий, тикер в 1000
//#define SWITCH_RELAY_TIMER_FAN 10000          //рабочий
//#define SWITCH_RELAY_TIMER_LIGHT 6000        //рабочий
#define TIME_SYNC_TIMER 60000
#define TEST_TEXT 1
#define RELAY_DELAY 150
#define RELAY_TMR_DELAY 150
//static uint32_t value_timer = 10000;        
//static uint32_t value_fan = 1000;             //задержка включения вентилятора
static uint32_t value_light = 1000;           //задержка включения света

#define RELAY_LIGHT 15    //a1
#define RELAY_FAN_MAX 16
#define RELAY_FAN_LOW_0 17
//#define RELAY_FAN_LOW_1 18
#define RELAY_LIGHT_FAN 12


#define STATE_FAN_OFF 0
#define STATE_FAN_LOW 1
#define STATE_FAN_MAX 2
#define STATE_FAN_AUTO 3

#define STATE_LIGHT_OFF 0
#define STATE_LIGHT_ON 1
#define STATE_LIGHT_AUTO 2

const int menuItems = 20;
int     test_text = TEST_TEXT;
int     button_flag = 0;
int     button_flag_on_start = 0;
int     val[menuItems];
int     display_state;
int     i = 0;
int     j = 0;
int     error_sec = 0;
int     error_min = 0;
int     temp_stat = 0;
int     wet_stat = 0;
int     fan_status = 0;
int     light_status = 0;
int     last_fan_status = 0;
int     last_light_status = 0;
int     relay_on_light = 0;
int     relay_on_fan_max = 0;
int     relay_on_fan_low_0 = 0;
//int     relay_on_fan_low_1 = 0;
int     relay_on_light_fan = 0;

int     error_time_flag = 0;
int     error_wet_flag = 0;

float   temp = 5;
float   wet = 5;
float   filt_temp = 75;
float   filt_wet = 70;

char    string0 [18] = {"                \0"};
char    buf0 [18] =    {"                \0"};

static uint32_t tmr;
static uint32_t tmr2;
static uint32_t tmr3;
static uint32_t tmr_sensor;
static uint32_t tmr_fan_switch;
static uint32_t tmr_light_switch;
static uint32_t tmr_relay_switch_fan;
static uint32_t tmr_relay_switch_light;
static uint32_t tmr_time_sync;


int   max_scroller[menuItems] = {3, 2,
  23, 59, 
  2, 2,
  99, 99, 99,
  99, 99, 99,
  23, 59, 23, 59,
  23, 59, 23, 59};

int   initial_value[menuItems] = {3, 2,
  12, 0, 
  1, 1,
  20, 70, 2,
  20, 40, 50,
  22, 1, 7, 2,
  8, 3, 21, 4};
  
const char menu0 [18] =  {"================\0"};
const char menu1 [18] =  {"================\0"};
const char menu2 [18] =  {"time hh         \0"};
const char menu3 [18] =  {"time mm         \0"};
const char menu4 [18] =  {"wet_stat        \0"};
const char menu5 [18] =  {"temp_stat       \0"};
const char menu6 [18] =  {"wet min         \0"};
const char menu7 [18] =  {"wet max         \0"};
const char menu8 [18] =  {"relay minute    \0"};
const char menu9 [18] =  {"temp min        \0"};
const char menu10 [18] = {"temp max        \0"};
const char menu11 [18] = {"reserved        \0"};
const char menu12 [18] = {"night hh        \0"};
const char menu13 [18] = {"night mm        \0"};
const char menu14 [18] = {"morning hh      \0"};
const char menu15 [18] = {"morning mm      \0"};
const char menu16 [18] =  {"lit of at hh    \0"};
const char menu17 [18] =  {"lit of at mm    \0"};
const char menu18 [18] =  {"lit on at hh    \0"};
const char menu19 [18] =  {"lit on at mm    \0"};

const char firstLine0 [18] =  {"                \0"};
const char firstLine1 [18] =  {"                \0"};
const char secondLine0 [18] =  {"                \0"};
const char secondLine1 [18] =  {"                \0"};
const char empty_line [18] =  {"                \0"};

LiquidCrystal lcd(8, 9, 4, 5, 6, 7 );
int button;  //вводим числовые значения для кнопок
const int BUTTON_NONE   = 0; //присваиваем постоянное значение для BUTTON_NONE 
const int BUTTON_RIGHT  = 1; //присваиваем постоянное значение для BUTTON_RIGHT
const int BUTTON_UP     = 2; //присваиваем постоянное значение для BUTTON_UP
const int BUTTON_DOWN   = 3; //присваиваем постоянное значение для BUTTON_DOWN 
const int BUTTON_LEFT   = 4; //присваиваем постоянное значение для BUTTON_LEFT
const int BUTTON_SELECT = 5; //присваиваем постоянное значение для BUTTON_SELECT

const int FAN_STATE     = 0;
const int LIGHT_STATE   = 1;
const int TIME_HH       = 2;
const int TIME_MM       = 3;
const int WET_STAT_MENU = 4;
const int TEMP_STAT_MENU= 5;
const int WET_MIN       = 6;
const int WET_MID       = 7;
const int RELAY_MINUTE  = 8;
const int TEMP_MIN      = 9;
const int TEMP_MID      = 10;
const int TEMP_MAX      = 11;
const int NIGHT_HH      = 12;
const int NIGHT_MM      = 13;
const int MORNING_HH    = 14;
const int MORNING_MM    = 15;
const int LIGHT_OFF_AT_HH  = 16;
const int LIGHT_OFF_AT_MM  = 17;
const int LIGHT_ON_AT_HH   = 18;
const int LIGHT_ON_AT_MM   = 19;

const char* const names[] =
{
  menu0, menu1, menu2, menu3,
  menu4, menu5, menu6, menu7,
  menu8, menu9, menu10, menu11,
  menu12, menu13, menu14, menu15, 
  menu16, menu17, menu18, menu19,  
};

const char* const firstLine[] =
{
  firstLine0, firstLine1,  
};

const char* const secondLine[] =
{
  secondLine0, secondLine1,  
};

#endif
