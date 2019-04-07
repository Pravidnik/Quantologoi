/*------------------Проект квантологосы. Инсталляция ФГУП. Говорим в рупор - загораются лампочки, сыпятся шарки, 
 идет сигнал на бульканье в ЛИНКОКО.*/
 
unsigned long last_time; 
boolean led_flag;
//-----------Настройка диодной ленты
#include "Adafruit_NeoPixel.h" //Подключение библиотеки для ленты
#define ledPin 2               //Пин ленты РГБ
#define ledCount 13             //кол-во диодов в ленте
Adafruit_NeoPixel strip = Adafruit_NeoPixel(ledCount, 2); // создаем переменную , где указываем кол-во диодов в ленте

//----------Настройка цвета ленты
#define red 255 //255
#define green 70 //70
#define blue 0//0

//----Настройка микрофона
#define micPin 1  // микрофон подключаем к первому пину аналоговому
#define limitMicValue 100 //значение микрофона, превысив которое начинаются различные действия
int micValue;     //Переменная для значений от микрофона

//----Настройка сервопривода
#include <Servo.h>//Подключаем библиотеку серво
#define pinServo 4 //Управление сервоприводом 4 пин
#define timeServo 500 // Время открытия сервы для скатывания шариков
Servo myservo;//создаем серво объект для контроля сервоприводом

//----Настройка передатчика
#include <SPI.h>               // Подключаем библиотеку для работы с шиной SPI
#include <nRF24L01.h>         // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>              // Подключаем библиотеку РФ24
RF24 radio(9, 10);             // Создаём объект radio для работы с библиотекой RF24, указывая номера выводов nRF24L01+ (CE, CSN)
int radioValue;
int data[1];//создаем массив для передачи данных



void setup() {
  Serial.begin (9600);
  pinMode (micPin, INPUT);  //Настраиваем пин под микрофон на ввод в контроллер
  strip.begin (); //инициализация ленты
  strip.setBrightness(255);//яркость светодиодов 
  myservo.attach(pinServo); // наздначаем управление сервой на 3 пин
  myservo.write (145);//Начальная позиция сервопривода
  radio.begin(); //активировать модуль
  radio.setChannel(5);   // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  radio.setDataRate     (RF24_1MBPS); // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setPALevel      (RF24_PA_HIGH); // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
  radio.openWritingPipe (0x1234567890LL);// Открываем трубу с идентификатором 0x1234567890 для передачи данных (на одном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
}


void LED () {       //Создание функции для светодиодной ленты
  led_flag = 1;
  if (millis() - last_time < 500 && led_flag == 1){
  for (int i = 0; i <= ledCount ; i++) {
    strip.setPixelColor (i, red, green, blue);
  }   
  strip.show ();
  }
  led_flag = 0;
  
  if (millis() - last_time < 2500 && led_flag == 0){
   for (int i = 0; i < ledCount; i++) 
  { 
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }  
  strip.show ();
  }
  led_flag = 1;
    
  if (millis() - last_time < 3500 && led_flag == 1) {
    for (int i = 0; i <= ledCount ; i++) {
    strip.setPixelColor (i, red, green, blue);
  } 
  strip.show ();
  }
  led_flag = 0;
  
  if (millis() - last_time < 4500 && led_flag == 0){
    for (int i = 0; i < ledCount; i++) 
  { 
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  } 
  strip.show ();
  }
  if (millis() - last_time > 4500) {
    last_time = millis();
    for (int i = 0; i < ledCount; i++) 
  { 
    strip.setPixelColor(i, strip.Color(red, green, blue));
  } 
  strip.show ();
  }
  



 /* if (millis() - last_time < 10000) {
  for (int i = 0; i <= ledCount ; i++) {
    strip.setPixelColor (i, red, green, blue);
  }   
  strip.show ();
  }*/
  
} 

void servo () {    //Создание функции для серворивода
  if (millis() - last_time > 500) {
    last_time = millis();
  myservo.write (70);
  }
  else{
  myservo.write (145);
  }
}

void loop() {
  radioValue = 0;
  data[0] = radioValue;
  micValue =  analogRead(micPin);
  Serial.println (micValue);
  radio.write(&data, sizeof(data)); // отправляем данные из массива data указывая сколько байт массива мы хотим отправить. Отправить данные можно с проверкой их доставки: if( radio.write(&data, sizeof(data)) ){данные приняты приёмником;}else{данные не приняты приёмником;}

  if (micValue > limitMicValue) { //если значение с микрофона превысит лимитированное значение, то...
   radioValue = 1;
   data [0] = radioValue;
   radio.write(&data, sizeof(data)); // отправляем данные из массива data указывая сколько байт массива мы хотим отправить. Отправить данные можно с проверкой их доставки: if( radio.write(&data, sizeof(data)) ){данные приняты приёмником;}else{данные не приняты приёмником;}
   LED (); 
   servo();
   for (int i = 0; i <= ledCount ; i++) {
    strip.setPixelColor (i, 0, 0, 0);
  } 
  strip.show (); 
  }
  Serial.println (micValue);
 
}



