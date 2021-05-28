#include "ft_main.h"

void setup()
{ 
  pinMode(RELAY_LIGHT, OUTPUT);
  pinMode(RELAY_FAN_MAX, OUTPUT);
  pinMode(RELAY_FAN_LOW_0, OUTPUT);
  pinMode(RELAY_LIGHT_FAN, OUTPUT);
  display_initialize();
  first_run();
  
  time.begin();
  delay(500);
  Serial.begin(9600);
  dht.begin();
  time_sync();                      //загрузить время
  load_settings();                    // загрузка данных
  //time.settime(0,51,21,27,10,15,2);
  //0  сек, 51 мин, 21 час, 27 числа, 10 месяца, 2015 года, 2 день нед - вт
  display_state = 1;
  sensor_timer_tick(1);
  forced_fan_timer_tick(1);
  forced_light_timer_tick(1);
  forced_relay_timer_tick_fan_light(1, 1);
  time_sync();
  error_wet_flag = 0;
}

void loop()
{
  while (true)
  {
    button = getPressedButton();
    display_switch(1);
    if (millis() - tmr >= 300)
    {
      tmr = millis();
      i = button_handler(i);
      if ((button == BUTTON_RIGHT) || (button == BUTTON_LEFT))
      {
        button_flag = 1;
        time_save_on_button();              //загрузка времени в модуль
      }
      val[i] = scroller(val[i], max_scroller[i]);
      i_j_scroller();
      if (button)  
        button_flag_on_start = 1;
      if (button_flag_on_start && display_state == 1)
        menu_print();
    }
    
    sensor_timer_tick(0);
    forced_fan_timer_tick(0);
    forced_light_timer_tick(0);
    forced_relay_timer_tick_fan_light(0, 0);
    if (button_flag)
    {
      forced_fan_timer_tick(1);
      forced_light_timer_tick(1);
      forced_relay_timer_tick_fan_light(1, 1);
    }
    timer_time_sync();
    save_on_timer();
    if (button_flag_on_start && display_state == 1)    
      menu_print();
    val[WET_STAT_MENU] = wet_stat;
    val[TEMP_STAT_MENU] = temp_stat;
    display_switch(0);
    button_flag = 0;
  }
}

void time_save_on_button()
{
  if ((button == BUTTON_RIGHT) || (button == BUTTON_LEFT)
  && (i == TIME_HH || i == TIME_MM))
  {
    time.settime(0,val[TIME_MM],val[TIME_HH],27,10,15,2);
  }
}

void time_sync()
{
    val[TIME_HH] = atoi(time.gettime("H"));
    val[TIME_MM] = atoi(time.gettime("i"));
    if (atoi(time.gettime("y")) == 0)
    {
      error_time_flag = 1;
    }
    Serial.println(time.gettime("d-m-Y, H:i:s, D")); // выводим время
}

void timer_time_sync()
{
  if (millis() - tmr_time_sync >= TIME_SYNC_TIMER)
  {
    tmr_time_sync = millis();
    time_sync();
  }
}

int is_day_light()
{
  if(is_in_range_time(val[LIGHT_ON_AT_HH], val[LIGHT_ON_AT_MM], 
  val[LIGHT_OFF_AT_HH], val[LIGHT_OFF_AT_MM]))
    return (1);
  else
    return (0);
}

int is_silent_mode()
{
  if(is_in_range_time(val[NIGHT_HH], val[NIGHT_HH], 
  val[MORNING_HH], val[MORNING_MM]))
    return (1);
  else
    return (0);
}

int is_in_range_time(int start_hh, int start_mm, int finish_hh, int finish_mm)
{
  int all_minutes;
  int start_minutes;
  int finish_minutes;
  all_minutes = val[TIME_HH] * 60 + val[TIME_MM];
  start_minutes = start_hh * 60 + start_mm;
  finish_minutes = finish_hh * 60 + finish_mm;

  if (start_minutes < finish_minutes)
  {
    if (all_minutes > start_minutes && all_minutes < finish_minutes)
      return (1);
    else
      return (0);
  }
  else
  {
    if (((all_minutes < (23 * 60 + 59)) && (all_minutes > start_minutes))
      ||  (all_minutes < finish_minutes))
      return (1);
    else
      return (0);
  }
  
}

void forced_relay_timer_tick_fan_light(int fan_forced, int light_forced)
{
  if  (fan_forced)
    relay_fan_run();
  if  (light_forced)
    relay_light_run();
  if ((fan_status != last_fan_status) || (last_light_status != light_status))
  {
    if ((millis() - tmr_relay_switch_fan >= RELAY_TMR_DELAY)  && (fan_status != last_fan_status))  //|| (fan_forced))
    {
      tmr_relay_switch_fan = millis();
      relay_fan_run();
    }
  }
  
  if ((millis() - tmr_relay_switch_light >= RELAY_TMR_DELAY) && (last_light_status != light_status)) //|| (light_forced))
  {
    tmr_relay_switch_light = millis();
    relay_light_run();
  }  
  //last_fan_status = fan_status;
  //last_light_status = light_status;
}

void relay_light_run()
{
  if (light_status == STATE_LIGHT_OFF)
  {
    delay(RELAY_DELAY);
    digitalWrite(RELAY_LIGHT, LOW);
    delay(RELAY_DELAY);
    digitalWrite(RELAY_LIGHT_FAN, LOW);
    delay(RELAY_DELAY);
    relay_on_light = 0;
    relay_on_light_fan = 0;
    last_light_status = light_status;
  }
  else if (light_status == STATE_LIGHT_ON)
  {
    delay(RELAY_DELAY);
    digitalWrite(RELAY_LIGHT, HIGH);
    delay(RELAY_DELAY);
    digitalWrite(RELAY_LIGHT_FAN, HIGH);
    //delay(RELAY_DELAY);
    relay_on_light = 1;
    relay_on_light_fan = 1;
    last_light_status = light_status;
  }
}

void relay_fan_run()
{
  if (fan_status == 0)
  {
    delay(RELAY_DELAY);
    digitalWrite(RELAY_FAN_LOW_0, LOW);
    //digitalWrite(RELAY_FAN_LOW_1, LOW);
    delay(RELAY_DELAY);
    digitalWrite(RELAY_FAN_MAX, LOW);
    delay(RELAY_DELAY);
    relay_on_fan_max = 0;
    relay_on_fan_low_0 = 0;
    //relay_on_fan_low_1 = 0;
    last_fan_status = fan_status;
  }
  else if (fan_status == 1)
  {
    delay(RELAY_DELAY);
    digitalWrite(RELAY_FAN_MAX, LOW);
    delay(RELAY_DELAY);
    digitalWrite(RELAY_FAN_LOW_0, HIGH);
    delay(RELAY_DELAY);
    //digitalWrite(RELAY_FAN_LOW_1, HIGH);
    relay_on_fan_max = 0;
    relay_on_fan_low_0 = 1;
    //relay_on_fan_low_1 = 1;
    last_fan_status = fan_status;
  }
  else if (fan_status == 2)
  {
    delay(RELAY_DELAY);
    digitalWrite(RELAY_FAN_LOW_0, LOW);
    //digitalWrite(RELAY_FAN_LOW_1, LOW);
    delay(RELAY_DELAY);
    digitalWrite(RELAY_FAN_MAX, HIGH);
    delay(RELAY_DELAY);
    relay_on_fan_max = 1;
    relay_on_fan_low_0 = 0;
    //relay_on_fan_low_1 = 0;
    last_fan_status = fan_status;
  }
}

void forced_light_timer_tick(int forced)
{
  if (forced)
    light_timer_run();
  if ((millis() - tmr_light_switch >= value_light) || (forced == 1))
  {
    tmr_light_switch = millis();
    light_timer_run();
  }
}

void light_timer_run()
{
  stats_read(filt_wet, filt_temp);
  if (val[LIGHT_STATE] == STATE_LIGHT_OFF)
    light_status = STATE_LIGHT_OFF;
  else if (val[LIGHT_STATE] == STATE_LIGHT_ON)
    light_status = STATE_LIGHT_ON;
  else if (val[LIGHT_STATE] == STATE_LIGHT_AUTO)
    light_status = is_day_light();
}

void forced_fan_timer_tick(int forced)
{
  uint32_t fftt_delay;
  if (forced)
    fan_timer_run();
  fftt_delay = 60000 * val[RELAY_MINUTE];
  if (millis() - tmr_fan_switch >= (fftt_delay))
  {
    tmr_fan_switch = millis();
    fan_timer_run();
  }
}

void fan_timer_run()
{
  stats_read(filt_wet, filt_temp);
  // код на переключение
  fan_status = max(wet_stat, temp_stat);
  if (val[FAN_STATE] == STATE_FAN_OFF)
    fan_status = STATE_FAN_OFF;
  else if (val[FAN_STATE] == STATE_FAN_LOW)
    fan_status = STATE_FAN_LOW;
  else if (val[FAN_STATE] == STATE_FAN_MAX)
    fan_status = STATE_FAN_MAX;
  else
  {
    fan_status = max(wet_stat, temp_stat);
    if (is_silent_mode() == 1)
      fan_status = constrain(fan_status, 0, STATE_FAN_LOW);
  }
}

void sensor_timer_tick(int forced)
{
  float t;
  float h;
  if ((millis() - tmr_sensor >= SENSOR_TIMER) || forced)
  {
    
    tmr_sensor = millis();
    // опрос датчика
    t = dht.readTemperature();
    h = dht.readHumidity();
    if (isnan(h) || isnan(t)) 
    {
      Serial.println("Не удается считать показания");
      error_wet_flag = 1;
      error_sec++;
    }
    else
    {
      temp = t;
      wet = h;
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Температура: ");
      Serial.print(t);
      Serial.print(" *C \t\t\t");
      Serial.println(time.gettime("d-m-Y, H:i:s, D")); // выводим время
      error_wet_flag = 0;
      error_sec = 0;
    }
    filt_wet = expRunningAverageAdaptive(wet);
    filt_temp = expRunningAverageAdaptive(temp);
  }
}



float expRunningAverageAdaptive(float newVal) 
{
  if (true)
  {
    return (newVal);
  }
  else
  {
    static float filVal = 0;
    float k;
    // резкость фильтра зависит от модуля разности значений
    if (abs(newVal - filVal) > 1.5) k = 0.9;
    else k = 0.03;
    
    filVal += (newVal - filVal) * k;
    return filVal;
  }
}

void stats_read(int wet1, int temp1)
{
  if (wet1 <= val[WET_MIN])
    wet_stat = 0;
  else if (wet1 <= val[WET_MID])
    wet_stat = 1;
  else 
    wet_stat = 2;

  if (temp1 <= val[TEMP_MIN])
    temp_stat = 0;
  else if (temp1 <= val[TEMP_MID])
    temp_stat = 1;
  else 
    temp_stat = 2;
}

void menu_print()
{
  if (i == 0)
  {
    print_first_line(0, i, string0, buf0);
    print_second_line(1, j, string0, buf0);
  }
  else if (i == menuItems - 1)
  {
    print_first_line(1, i, string0, buf0);
    print_normal_string(0, i, string0, buf0);
  }
  else if (i == 1)
  {
    print_second_line(0, j, string0, buf0);
    print_normal_string(1, j, string0, buf0);
  }
  else
  {
    print_normal_string(0, i, string0, buf0);
    print_normal_string(1, j, string0, buf0);
  }
}

void print_first_line(int column, int val_list, char *string, char *buf)
{
  //char  colon [2] =  {":"};
  char  text_auto [3] =  {"au"};
  char  text_low [3] =  {"lo"};
  char  text_max [3] =  {"mx"};
  char  text_off [3] =  {"of"};
  char  text_error [3] =  {"er"};
  
  strcpy(string, names[val_list]);
  
  itoa(val[TIME_HH], buf, 10);
  if (val[TIME_HH] / 10 == 0)
    {
      strcpy(&string[1], buf);
      string[0] = '0';
    }
  else
    strcpy(&string[0], buf);
  if (error_time_flag == 0)
    string[2] = ':';
  else
    string[2] = ' ';
  
  itoa(val[TIME_MM], buf, 10);
  if (val[TIME_MM] / 10 == 0)
  {
    strcpy(&string[4], buf);
    string[3] = '0';
  }
  else
    strcpy(&string[3], buf);
    
  //strlcat(&string[2], colon, 1);
  
  string[5] = ' ';
  itoa(temp, buf, 10);
  if (temp >= 100)
    temp = 99;
  if (temp < 10)
  {
    strcpy(&string[7], buf);
    string[6] = '0';
  }
  else
    strcpy(&string[6], buf);

  string[8] = 'C';
  string[9] = ' ';
  string[10] = 'f';
  string[11] = 'n';
  string[12] = ':';

  switch (val[FAN_STATE])
  {
    case STATE_FAN_OFF:
      strcpy(&string[13], text_off);
      break;
    case STATE_FAN_LOW:
      strcpy(&string[13], text_low);
      break;
    case STATE_FAN_MAX:
      strcpy(&string[13], text_max);
      break;
    case STATE_FAN_AUTO:
      strcpy(&string[13], text_auto);
      break;      
    default:
      strcpy(&string[13], text_error);
      break;
  }
  string[15] = ' ';
  itoa(fan_status, buf, 10);
  strcpy(&string[15], buf);
  //fan_status 
  lcd.setCursor(0, column);
  lcd.print(string);
}

void print_second_line(int column, int val_list, char *string, char *buf)
{
  char  colon [2] =  {":"};
  char  text_off [3] =  {"of"};
  char  text_on [3] =  {"on"};
  char  text_auto [3] =  {"au"};
  char  text_error [3] =  {"er"};
  
  strcpy(string, names[val_list]);
  string[0] = ' ';
  string[1] = ' ';
  string[2] = ' ';
  string[3] = ' ';
  string[4] = ' ';
  string[5] = ' ';
  itoa(wet, buf, 10);
  if (wet >= 100)
    wet = 99;
  if (wet < 10)
  {
    strcpy(&string[7], buf);
    string[6] = '0';
  }
  else
    strcpy(&string[6], buf);

  string[8] = '%';
  string[9] = ' ';
  string[10] = 'l';
  string[11] = 't';
  string[12] = ':';

  switch (val[LIGHT_STATE])
  {
    case STATE_LIGHT_OFF:
      strcpy(&string[13], text_off);
      break;
    case STATE_LIGHT_ON:
      strcpy(&string[13], text_on);
      break;     
    case STATE_LIGHT_AUTO:
      strcpy(&string[13], text_auto);
      break;        
    default:
      strcpy(&string[13], text_error);
      break;
  }
  string[15] = ' ';
  itoa(light_status, buf, 10);
  strcpy(&string[15], buf);
//  if (relay_on_light == 1)
//    string[0] = 'L';
//  if (relay_on_fan_max == 1)
//    string[1] = 'M';
//  if (relay_on_fan_low_0 == 1)
//    string[2] = 'm';
//  if (relay_on_light_fan == 1)
//    string[3] = 'l';
  if (error_wet_flag == 1)
    string[4] = 'E';
  if (is_silent_mode() == 1)
    string[5] = 'S';

  error_min = error_sec / 60;
  if (error_min > 999)
    error_min = 999;
  itoa(error_min, buf, 10);
  strcpy(&string[0], buf);

  if (error_min / 100 == 0)
    {
      strcpy(&string[1], buf);
      string[0] = '0';
    }
  
  if (error_min / 10 == 0)
    {
      strcpy(&string[2], buf);
      string[0] = '0';
      string[1] = '0';
    }
  
  string[3] = ' ';
  if (error_wet_flag == 1)
    string[3] = 'E';
  if (error_wet_flag == 1)
    string[4] = 'E';
  
  lcd.setCursor(0, column);
  lcd.print(string);
}

void i_j_scroller()
{
  j = i + 1;
  i = scroller(i, menuItems - 1);
  j = scroller(j, menuItems - 1);
}

void display_initialize()
{
  lcd.begin(16, 2); //Инициализируем дисплей: 2 строки по 16 символов
  lcd.setCursor(0, 0);
  lcd.print("started                ");  //Выводи надпись
  lcd.setCursor(0, 1);
  lcd.print("press any key                ");  //Выводи надпись
  pinMode (10, OUTPUT);
  lcd.setCursor(0, 0);
}

void display_wait()
{
  lcd.setCursor(12, 0);
  lcd.print("WAIT");  //Выводи надпись
    
  //lcd.setCursor(0, 1);
  //lcd.print("BE PATIENT                ");  //Выводи надпись
  //pinMode (10, OUTPUT);
}

void load_settings()
{
  int counter;
  
  counter = 0;
  while (counter < menuItems)
  {
    if (!((counter == TIME_HH) || (counter == TIME_MM))) //загрузка кроме времени
    {
      EEPROM.get(counter, val);
    }
    counter++;
  }
}

void first_run()
{
  int counter;
  
  counter = 0;
  if (EEPROM.read(INIT_ADDR) != INIT_KEY) 
  { // первый запуск
    EEPROM.write(INIT_ADDR, INIT_KEY);    // записали ключ
    while (counter < menuItems)
    {
    if ((counter != TIME_HH) || (counter != TIME_MM))
      {
        EEPROM.put(counter, initial_value[counter]);
        counter++;
      }
    }
  }
}

void display_switch(int val)
{
  if (display_state % 2 == 1)
    {
      digitalWrite(10, 1);
      display_state = 1;
    }
   else
   {
      digitalWrite(10, 0);
      lcd.clear();
    }
}

void save_on_timer()
{
  if (millis() - tmr3 >= SAVE_TIMER_MSEC)
    {
      tmr3 = millis();
      save_changes();
    }
}

int button_handler(int i)
{
  if (button == BUTTON_DOWN) 
  {
    i++;
    i_j_scroller();
    val[i] = scroller(val[i], max_scroller[i]);
  }
  if (button == BUTTON_UP) 
  {
    i--;
    i_j_scroller();
    val[i] = scroller(val[i], max_scroller[i]);
  }
  if (button == BUTTON_RIGHT) val[i]++;
  if (button == BUTTON_LEFT) val[i]--;
  if (button != BUTTON_NONE) lcd.clear();
  if (button == BUTTON_SELECT) 
  {
    save_changes();
    display_state++;
  }
  
  return (i);
}

void time_ticker()
{
  if (millis() - tmr2 >= MINUTE_IS)
    {
      tmr2 = millis();
      val[TIME_MM]++;
      val[TIME_MM] = scroller(val[TIME_MM], 59);
      if (val[TIME_MM] == 0)
      {
        val[TIME_HH]++;
        val[TIME_HH] = scroller(val[TIME_HH], 23);
      }
    }
}

void save_changes()
{
  int count;
  count = 0;
  while (count < menuItems)
  {
    if ((count != TIME_HH) || (count != TIME_MM))   //сохранение кроме времени
    {
      val[count] = constrain(val[count], 0, 255); // ограничили
      EEPROM.put(count, val);           // записали
      count++;
    }
  }
}

void print_normal_string(int column, int val_list, char *string, char *buf) 
{
  strcpy(string, names[val_list]);
  itoa(val[val_list], buf, 10);
  strcpy(&string[13], buf);
  strlcat(string, empty_line, 18);
  lcd.setCursor(0, column);
  lcd.print(string);
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
