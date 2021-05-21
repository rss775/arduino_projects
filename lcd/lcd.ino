#include  <LiquidCrystal.h> //подключаем библиотеку
#include "GyverTimers.h"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7 );
int button;  //вводим числовые значения для кнопок
const int BUTTON_NONE   = 0; //присваиваем постоянное значение для BUTTON_NONE 
const int BUTTON_RIGHT  = 1; //присваиваем постоянное значение для BUTTON_RIGHT
const int BUTTON_UP     = 2; //присваиваем постоянное значение для BUTTON_UP
const int BUTTON_DOWN   = 3; //присваиваем постоянное значение для BUTTON_DOWN 
const int BUTTON_LEFT   = 4; //присваиваем постоянное значение для BUTTON_LEFT
const int BUTTON_SELECT = 5; //присваиваем постоянное значение для BUTTON_SELECT

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

int getPressedButton() //инициализация переменной
{
  int buttonValue = analogRead(0); // считываем значения с аналогового входа
  if (buttonValue < 100) { //если при нажатии кнопки значение меньше 100
    return BUTTON_RIGHT;   // выводим значение BUTTON_RIGHT
  }
  else if (buttonValue < 200) { //если при нажатии кнопки значение меньше 200
    return BUTTON_UP; // выводим значение BUTTON_UP
  }
  else if (buttonValue < 400){ //если при нажатии кнопки значение меньше 400
    return BUTTON_DOWN; // выводим значение BUTTON_DOWN
  }
  else if (buttonValue < 600){ //если при нажатии кнопки значение меньше 600
    return BUTTON_LEFT; // выводим значение BUTTON_LEFT
  }
  else if (buttonValue < 800){ //если при нажатии кнопки значение меньше 800
    return BUTTON_SELECT; // выводим значение BUTTON_SELECT
  }
  return BUTTON_NONE; //иначе, выводим значение BUTTON_NONE
}

void setup()
{
  lcd.begin(16, 2); //Инициализируем дисплей: 2 строки по 16 символов            
  lcd.print("start ");  //Выводи надпись www.helpduino.ru      
  pinMode (10, OUTPUT);
}

void loop()
{
  lcd.setCursor(0, 0);
  int   i = 0;
  int   j = 0;
  int   val[menuItems];
  char  spaces [18] =  {"                \0"};
    
  char  buf0 [18] =    {"                \0"};
  char  buf1 [18] =    {"                \0"};
  int   ticker = 0;
  int   tSec = 0;
  char  string0 [18] = {"                \0"};
  char  string1 [18] = {"                \0"};

  int count = 0;
  while (count < menuItems)
  {
    val[count] = count * count;
    count++;
  }
  val[0] = 0;
  val[1] = 0;

  
  button = getPressedButton();
  while (true)
  {
    button = getPressedButton();
    if (millis() - tmr >= 300) 
    {
      tmr = millis();
    
      if (button == BUTTON_DOWN) i++;
      if (button == BUTTON_UP) i--;
      if (button == BUTTON_RIGHT) val[i]++;
      if (button == BUTTON_LEFT) val[i]--;
      if (button != BUTTON_NONE) lcd.clear();
      
      
      j = i + 1;
      i = scroller(i, menuItems - 1);
      j = scroller(j, menuItems - 1);

      strcpy(string0, names[i]);
      itoa(val[i], buf0, 10);
      strcpy(&string0[14], buf0);
      lcd.setCursor(0, 0);
      lcd.print(string0);
      
      strcpy(string1, names[j]);
      itoa(val[j], buf1, 10);
      strcpy(&string1[14], buf1);
      lcd.setCursor(0, 1);
      lcd.print(string1);

      if (i == 4)
      {
        digitalWrite(10, 0);
        lcd.clear();
      }
      else
        digitalWrite(10, 1);
    }

    if (millis() - tmr2 >= 1000) 
    {
      tmr2 = millis();
      tSec++;
    }

    val[0] = tSec;

  }
}

int scroller(int i, int range)
{
  if (i > range)
      i = 0;
  if (i < 0)
      i = range;
  return (i);
}

void  displayTest()
{
  int i;
  i = 0;
  lcd.setCursor(0, 1); //устанавливаем курсор на 1 строку 1 столба
  lcd.print("asd"); //стираем текст дисплея
  button = getPressedButton();
  //Присваиваем занчение переменной getPressedButton к переменной button 
  switch (button) //перебираем значения в цикле
  {
  case BUTTON_RIGHT: // при нажатии кнопки со значением BUTTON_RIGHT 
    lcd.setCursor(0, 0); //устанавливаем курсор на 1 строку 1 столба
    lcd.print("                 "); //стираем текст дисплея
    lcd.setCursor(0, 0); //устанавливаем курсор на 1 строку 1 столба
    lcd.print("BUTTON: RIGHT");//выводим надпись BUTTON: RIGHT на экран
    break; //переходим к следующему значению цикла
  case BUTTON_LEFT: // при нажатии кнопки со значением BUTTON_LEFT
     lcd.setCursor(0, 0); //устанавливаем курсор на 1 строку 1 столба
    lcd.print("                 "); //стираем текст дисплея
    lcd.setCursor(0, 0); //устанавливаем курсор на 1 строку 1 столба
    lcd.print("BUTTON: LEFT"); //выводим надпись BUTTON: LEFT на экран
    break; //переходим к следующему значению цикла
  case BUTTON_UP: // при нажатии кнопки со значением BUTTON_UP
     lcd.setCursor(0, 0); //устанавливаем курсор на 1 строку 1 столба
    lcd.print("                 "); //стираем текст дисплея
    lcd.setCursor(0, 0); //устанавливаем курсор на 1 строку 1 столба
    lcd.print("BUTTON: UP"); //выводим надпись BUTTON: UP на экран
    break; //переходим к следующему значению цикла
  case BUTTON_DOWN: // при нажатии кнопки со значением BUTTON_DOWN
     lcd.setCursor(0, 0); //устанавливаем курсор на 1 строку 1 столба
    lcd.print("                 "); //стираем текст дисплея
    lcd.setCursor(0, 0); //устанавливаем курсор на 1 строку 1 столба
    lcd.print("BUTTON: DOWN"); //выводим надпись BUTTON: DOWN на экран
    break; //переходим к следующему значению цикла
  case BUTTON_SELECT: // при нажатии кнопки со значением BUTTON_SELECT
     lcd.setCursor(0, 0); //устанавливаем курсор на 1 строку 1 столба
    lcd.print("                 "); //стираем текст дисплея
    lcd.setCursor(0, 0); //устанавливаем курсор на 1 строку 1 столба
    lcd.print("BUTTON: SELECT"); //выводим надпись BUTTON: SELECT на экран
    break; //переходим к следующему значению цикла
  }
}
