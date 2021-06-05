/*
  Часы с отображением температуры и влажности с индикацией на VFD (ВЛИ) дисплее покупателя 20*2 Futaba (vfd9cb1010 rev 1.2)
  AMatroskin 2020 год.
*/


#include <SoftwareSerial.h>
#include <Wire.h>

#include <LiquidCrystal_I2C.h>


#define SEALEVELPRESSURE_HPA (1010.7)


SoftwareSerial mySerial(11, 10);




uint32_t counter = 1000, counter2 = 0; //____________________________Переменные счетчиков
byte index, min_hour = 0; //_________________________________________Служебные переменные
bool flag = 1, flag2 = 0, set_view = 0;


void setup() {
  delay(3000); //____________тут надо подождать
  pinMode(10, OUTPUT);


  mySerial.begin(19200);
  Serial.begin(9600);
  mySerial.write(0x1B); //
  mySerial.write(0x40); //____инициализация дисплея
  delay(1000); //_____________и тут надо подождать
  delay(2000); //_____________и тут надо подождать
  mySerial.write(0x0b); //____курсор домой
  show_time (); //____________выводим информацию на дисплей
}

void loop() {
  if (millis() - counter >= 1000) {
    show_time ();
    //mySerial.write(0xc8);
    flag = ! flag;
    counter += 1000;
    set_brightness(4);
    
    
  }
}



void show_time () {
  mySerial.write(0x0b);
  mySerial.print("                huy");
  mySerial.print("                   ");
}

void set_brightness(byte lev) {//______________________Установка яркости дисплея (1 - 4)
  byte level = lev;
  switch (level) {
    case 1:
      mySerial.write(0x1B); //_________________________Первый уровень яркости
      mySerial.write(0x2A);
      mySerial.write(0x01);
      break;
    case 2:
      mySerial.write(0x1B); //_________________________Второй уровень яркости
      mySerial.write(0x2A);
      mySerial.write(0x02);
      break;
    case 3:
      mySerial.write(0x1B); //_________________________Третий уровень яркости
      mySerial.write(0x2A);
      mySerial.write(0x03);
      break;
    case 4:
      mySerial.write(0x1B); //_________________________Четвертый уровень яркости
      mySerial.write(0x2A);
      mySerial.write(0x04);
      break;
  }
}
