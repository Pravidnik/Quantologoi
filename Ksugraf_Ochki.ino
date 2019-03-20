
/* В данном скетче с передающей части (ТХ) отправляется значение переменной counter,
   переменная эта с каждым шагом увеличивается на единицу. Приёмник (RX) принимает
   сигнал, и отправляет обратно то, что получил, используя функцию radio.writeAckPayload
   То есть наш приёмник на одно мгновение становится передатчиком! Если наш передатчик (TX)
   принимает ответный сигнал, он выдаёт то, что принял, и пишет посчитанное вермя между
   отправкой и приёмом сигнала в микросекундах. Данный скетч можно использовать для теста
   модулей на качество связи, а также для понимания работы функции radio.writeAckPayload
   by AlexGyver 2016
*/

#include <EEPROM.h>

int ep;

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#include <Servo.h>

Servo myservo;
unsigned long TimeServo;

//-------Настройки сервопривода-------------------
#define SERVO_PIN 5
#define MIN_POS 5 // Минимальная позиция сервопривода
#define MAX_POS 160 // Максимальная позиция сервопривода
#define servospeed 1000

#define NUM_LEDS 1

#define sped 200
#include "FastLED.h"
#define PIN_LED 3
#define ON_ALL 10000
#define BRIGHTNESS 255
CRGB leds[NUM_LEDS];



RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
int value;
int flag_servo = 0;
boolean LedFlag = 0;
boolean flag = 0;
unsigned long LastBlink;
unsigned long LastOn;
int pos;
byte pipeNo;
byte recieved_data[3]; // массив принятых данных

void setup() {
  FastLED.addLeds<WS2811, PIN_LED, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(255);
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600); //открываем порт для связи с ПК
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(5, 150);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openWritingPipe(address[0]);   //мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x71);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  // ВНИМАНИЕ!!! enableAckPayload НЕ РАБОТАЕТ НА СКОРОСТИ 250 kbps!

  radio.powerUp(); //начать работу
  radio.startListening();  //начинаем слушать эфир, мы приёмный модуль
  myservo.attach(SERVO_PIN);
  mig(0);
  servo(0);
}


void loop()
{
  //mig(1);
  //servo(1);
  start();
}
void start() {
  if (ep == 1)
  {
    ep = 0;
    EEPROM.write(0, ep);
    Serial.println(ep);
    Serial.println("Yes");
    resetFunc();
    Serial.println("Yes");
  }
/*
  if (ep == 2)
  {
    ep = 0;
    EEPROM.write(0, ep);
    Serial.println(ep);
    Serial.println("Yes");
    resetFunc();
    Serial.println("Yes");
  }
*/

  while (radio.available(&pipeNo))
  { // слушаем эфир со всех труб
    radio.read(&recieved_data, sizeof(recieved_data));      // чиатем входящий сигнал
    // if (recieved_data[0] == 1) value++;
    // radio.writeAckPayload(pipeNo,&LedStatus, 1 );  // отправляем обратно то что приняли
    if (recieved_data[0] == 1)
    {
      value++;
      Serial.println("On");
      LastOn = millis();
      while (millis() - LastOn < ON_ALL)
      {
        mig(1);
        servo(1);
        Serial.println(millis() - LastOn);
      }
      Serial.println("Off");
      ep = 1;
      EEPROM.write(0, ep);
      Serial.println(ep);
      resetFunc();
      delay(100);
      mig(0);
      servo(0);
      flag = 0;
      //      recieved_data[0] = 0;
    }
    //if (recieved_data[0] == 0) flag = 1;
    //radio.writeAckPayload(pipeNo,&gotByte, 1 );  // отправляем обратно то что приняли
    Serial.print(value); Serial.print("   ");
    Serial.println(recieved_data[0]);
  }
}



void resetFunc()
{
  digitalWrite(7, HIGH);
  pinMode(7, OUTPUT);
  delay(100);
  Serial.println("Perezagruzka");
  digitalWrite(7, LOW);
}


void mig(int mode) {
  if (mode == 1)
  {
    if (millis() - LastBlink > sped && LedFlag == 1)
    {
      for (int i = 0; i < NUM_LEDS; i++ ) {
        leds[i] = CHSV(0 , 0, 255);
        FastLED.show();
      }
      LastBlink = millis();
      LedFlag = 0;
    }
    if (millis() - LastBlink > sped  && LedFlag == 0)
    {
      for (int i = 0; i < NUM_LEDS; i++ ) {
        leds[i] = CHSV(0 , 0, 0);
        FastLED.show();
      }
      LastBlink = millis();
      LedFlag = 1;
    }
  }
  else if (mode == 0)
  {
    for (int i = 0; i < NUM_LEDS; i++ ) {
      leds[i] = CHSV(0 , 0, 0);
      FastLED.show();
    }
  }
}


void servo(int mode) {
  if (mode == 1)
  {

    if (millis() >= servospeed + TimeServo && flag_servo == 1)
    {
      myservo.write(MIN_POS);
      TimeServo = millis();
      flag_servo = 0;
      //Serial.println("Servo 0");
    }

    if (millis() >= servospeed + TimeServo && flag_servo == 0)
    {
      flag_servo = 1;
      myservo.write(MAX_POS);
      TimeServo = millis();
      //Serial.println("Servo 180");
    }
  }
  else if (mode == 0)
  {
    
    myservo.write(90
    );
  }
}
