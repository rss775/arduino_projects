#define CLK 19
#define DT 13
#define SW 18
#define ITEMS 16

#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW);

#include  <LiquidCrystal.h> //подключаем библиотеку
LiquidCrystal lcd(8, 9, 4, 5, 6, 7 );

#include <iarduino_RTC.h>                                 
iarduino_RTC watch(RTC_DS1302, 12, 10, 11);  

const char menu0 [17] = {"menu 0:         "};
const char menu1 [17] = {"menu 1:         "};
const char menu2 [17] = {"menu 2:         "};
const char menu3 [17] = {"menu 3:         "};
const char menu4 [17] = {"menu 4:         "};
const char menu5 [17] = {"menu 5:         "};
const char menu6 [17] = {"menu 6:         "};
const char menu7 [17] = {"menu 7:         "};
const char menu8 [17] = {"menu 8:         "};
const char menu9 [17] = {"menu 9:         "};
const char menu10 [17] = {"menu 10:        "};
const char menu11 [17] = {"menu 11:        "};
const char menu12 [17] = {"menu 12:        "};
const char menu13 [17] = {"menu 13:        "};
const char menu14 [17] = {"menu 14:        "};
const char menu15 [17] = {"menu 15:        "};

const char* const names[] =
{
  menu0, menu1, menu2, menu3,
  menu4, menu5, menu6, menu7,
  menu8, menu9, menu10, menu11,
  menu12, menu13, menu14, menu15,  
};


void setup()
{
  Serial.begin(9600);
  enc1.setType(TYPE1);    // тип энкодера TYPE1 одношаговый, TYPE2 двухшаговый. Если ваш энкодер работает странно, смените тип

  //watch.begin(); 
  
  lcd.begin(16, 2); //Инициализируем дисплей: 2 строки по 16 символов           


}

void loop()
{
                              
  //Serial.println(watch.gettime("d-m-Y, H:i:s, D"));

  lcd.setCursor(0, 0);
  int   i = 0;
  int   j = 0;
  int   val[16];
  char  string0 [17] = {"0-23456789abcdef"};
  char  string1 [17] = {"1-23456789abcdef"};  
  char  buf0 [17] =    {"                "};
  int   counter = 0;
  //digitalWrite(10, 0);
  //while (true) {}

  int count = 0;

  while (count < 16)
  {
    val[count] = count * count;
    count++;
  }

  lcd.setCursor(0, 0);
  lcd.print(string0);
  lcd.setCursor(0, 1);
  lcd.print(string1);
  while (true)
  {
    while (counter < 1000)
    {
      enc1.tick();
      counter++;
    }
    delay(100);
    counter = 0;
    if (enc1.isRight()) i++;
    if (enc1.isLeft()) i--;
    if (enc1.isRightH()) Serial.println("Right holded"); // если было удержание + поворот
    if (enc1.isLeftH()) Serial.println("Left holded");
    j = i + 1;
    
    if (i > 15)
      i = 0;
    if (i < 0)
      i = 15;

    if (j > 15)
      j = 0;
    if (j < 0)
      j = 15;

    lcd.setCursor(0, 0);
    lcd.print(names[i]);
    lcd.setCursor(0, 1);
    lcd.print(names[j]);
    lcd.setCursor(10, 0);
    lcd.print(itoa(val[i], buf0, 10));
    lcd.setCursor(10, 2);
    lcd.print(itoa(val[j], buf0, 10));

  }
  
}
