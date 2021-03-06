/*------------------Проект квантологосы. Инсталляция ЛИНКОКО. Принимаем сигнал с ФГУП
булькаем и загорается лента в проекторе. */

//-----------Настройка диодной ленты
#include "Adafruit_NeoPixel.h" //Подключение библиотеки для ленты
#define ledPin 6               //Пин ленты РГБ
#define ledCount 10             //кол-во диодов в ленте
Adafruit_NeoPixel strip = Adafruit_NeoPixel(ledCount, 2); // создаем переменную , где указываем кол-во диодов в ленте


//-----------Настройка приёмника
#include <SPI.h>      // Подключаем библиотеку  для работы с шиной SPI
#include <nRF24L01.h> // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>     // Подключаем библиотеку  для работы с nRF24L01+
RF24  radio (9, 10); //создаем объект radio для работы с библиотекой RF24 и указываем номера выводов
int data[2]; //массив для приёма

void setup() {
  Serial.begin(9600);
  pinMode (4, OUTPUT);//пин компрессора
strip.begin (); //инициализация ленты
strip.setBrightness (255); //настройка яркости светодиодов
radio.begin(); //активировать модуль
radio.setChannel(5);                                  // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
radio.setDataRate     (RF24_1MBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
radio.setPALevel      (RF24_PA_HIGH);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
radio.openReadingPipe (1, 0x1234567890LL);            // Открываем 1 трубу с идентификатором 0x1234567890 для приема данных (на ожном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
radio.startListening  ();                             // Включаем приемник, начинаем прослушивать открытую трубу
  
}
void LED () {
  for (int i = 0; i < ledCount ; i++) {
    strip.setPixelColor (i, 254,0 , 254);//настройка цветов
  } 
  strip.show ();//передать на ленту

}

void loop() {

 
if(radio.available()) {
  radio.read(&data, sizeof(data));
   Serial.println(data[0]);   
}
if (data[0] == 1) {
 
 delay(2000);
  digitalWrite(4,HIGH);
   LED(); 
    delay(6000); 
   
} else {
  digitalWrite(4,LOW);
  for (int i = 0; i <= ledCount ; i++) {
    strip.setPixelColor (i, 0, 0, 0);//настройка цветов 
    
  } 
  strip.show();

}
}
