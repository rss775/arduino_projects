
#include "ft_main.h"

void setup()
{
  lcd.begin(16, 2); //Инициализируем дисплей: 2 строки по 16 символов
  lcd.print("start");  //Выводи надпись
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

      val[i] = scroller(val[i], 59);


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

int getPressedButton() //инициализация переменной
{
  int buttonValue = analogRead(0); // считываем значения с аналогового входа
  if (buttonValue < 100) { //если при нажатии кнопки значение меньше 100
    return BUTTON_RIGHT;   // выводим значение BUTTON_RIGHT
  }
  else if (buttonValue < 200) { //если при нажатии кнопки значение меньше 200
    return BUTTON_UP; // выводим значение BUTTON_UP
  }
  else if (buttonValue < 400) { //если при нажатии кнопки значение меньше 400
    return BUTTON_DOWN; // выводим значение BUTTON_DOWN
  }
  else if (buttonValue < 600) { //если при нажатии кнопки значение меньше 600
    return BUTTON_LEFT; // выводим значение BUTTON_LEFT
  }
  else if (buttonValue < 800) { //если при нажатии кнопки значение меньше 800
    return BUTTON_SELECT; // выводим значение BUTTON_SELECT
  }
  return BUTTON_NONE; //иначе, выводим значение BUTTON_NONE
}
