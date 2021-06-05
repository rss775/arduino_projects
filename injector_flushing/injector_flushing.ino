int injector_pin = 2;                 // Светодиод подключенный к вход/выходу 13
void setup()
{
  pinMode(injector_pin, OUTPUT);      // устанавливает режим работы - выход
}
 
void loop()
{
  int rps = 100;
  int percentage = 10;
  int both_delay = 1000 / rps;
  digitalWrite(injector_pin, HIGH);   // включает светодиод
  delay(10);                  // ждет секунду
  digitalWrite(injector_pin, LOW);    // выключает светодиод
  delay(100);                  // ждет секунду
}
