ффффф#include "DHT.h"

#define DHTPIN 2 // номер пина, к которому подсоединен датчик

// Раскомментируйте в соответствии с используемым датчиком

// Инициируем датчик

DHT dht(DHTPIN, DHT22);

//DHT dht(DHTPIN, DHT11);

void setup() {

Serial.begin(9600);

dht.begin();

}

void loop() {

pinMode(16, OUTPUT);
digitalWrite(16, 0);
delay(200);
digitalWrite(16, 0);
delay(200);


// Задержка 2 секунды между измерениями

delay(200);

//Считываем влажность

float h = dht.readHumidity();

// Считываем температуру

float t = dht.readTemperature();

// Проверка удачно прошло ли считывание.

if (isnan(h) || isnan(t)) {

Serial.println("Не удается считать показания");

return;

}

Serial.print("Влажность: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Тра: ");
Serial.print(t);
Serial.print(" *C \n");

}
