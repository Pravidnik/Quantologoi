/* В данном скетче с передающей части (ТХ) отправляется значение переменной counter,
   переменная эта с каждым шагом увеличивается на единицу. Приёмник (RX) принимает
   сигнал, и отправляет обратно то, что получил, используя функцию radio.writeAckPayload
   То есть наш приёмник на одно мгновение становится передатчиком! Если наш передатчик (TX)
   принимает ответный сигнал, он выдаёт то, что принял, и пишет посчитанное вермя между
   отправкой и приёмом сигнала в микросекундах. Данный скетч можно использовать для теста
   модулей на качество связи, а также для понимания работы функции radio.writeAckPayload
   by AlexGyver 2016
*/

#define PIN_BOT 7
#define NUM_BOT 10

//-------Настройки сервопривода-------------------
#define SERVO_PIN 4
#define MIN_POS 15 // Минимальная позиция сервопривода
#define MAX_POS 175 // Максимальная позиция сервопривода
#define servospeed 1000

//------Настройки ленты------------------
#define NUM_LEDS_PLAMY 20
#define PIN_PLAMY 6
#define BRIGHTNESS_PLAMY 255
#define MIN_BRIGHT 250
#define MAX_BRIGHT 255
#define HUE_MIN 0
#define HUE_MAX 20
#define HUE_NULL 0

#include <Servo.h>

Servo myservo;
unsigned long TimeServo;
byte peredacha[3];

int flag_servo = 0;
#define sped 300


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// пример с "бегущей радугой" для библиотеки FastLED
#define NUM_LEDS 5

#include "FastLED.h"
#define PIN 2
#define PIN_TRIG 3
#define ON_ALL 10000
#define BRIGHTNESS 255
#define RAINBOW_SPEED 300  // скорость движения радуг
#define STEP_RAINBOW 400// Чем больше ставишь, тем меньше шаг радуги
CRGB leds[NUM_LEDS];
CRGB plamy[NUM_LEDS_PLAMY];
CRGB bot[NUM_BOT];
byte counter;
int Bright;
byte Red;

RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно
byte margo;
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
long RandNum;
boolean butt_flag = 0;
boolean butt;
boolean led_flag = 0;
boolean Status;
unsigned long last_press;
boolean flag = 0;
boolean LedFlag = 0;
unsigned long LastBlink;
unsigned long LastOn;

int pos;

byte pipeNo;

void setup() {
  FastLED.addLeds<WS2811, 6, GRB>(plamy, 20).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2811, 2, GRB>(leds, 4).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2811, PIN_BOT, GRB>(bot, NUM_BOT).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(255);
  pinMode(2, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(PIN_BOT, OUTPUT);
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
  radio.stopListening();  //не слушаем радиоэфир, мы передатчик
  pinMode(PIN_TRIG, INPUT_PULLUP);
  Serial.begin(9600);
  myservo.attach(4);
}

void loop()
{
  //mig(1);
  start();
  //botinok(1);
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

    if (millis() >= 1000 + TimeServo && flag_servo == 1)
    {
      myservo.write(0);
      TimeServo = millis();
      flag_servo = 0;
      //Serial.println("Servo 0");
    }

    if (millis() >= 1000 + TimeServo && flag_servo == 0)
    {
      flag_servo = 1;
      myservo.write(179);
      TimeServo = millis();
      //Serial.println("Servo 180");
    }
  }
  else if (mode == 0)
  {
    pos = myservo.read();
    myservo.write(pos);
  }
}

void plamyOn(int mode)
{
  if (mode == 1)
  {

    for (int i = 0; i < NUM_LEDS_PLAMY; i++)
    {
      Red = random(0, 20);
      Red = 0 + Red;
      // Serial.println(Red);
      Bright = random(200, 255);
      plamy[i] = CHSV(Red, 255, Bright);
      FastLED.show();
    }

  }
  else if (mode == 0)
  {
    for (int i = 0; i < NUM_LEDS_PLAMY; i++)
    {
      plamy[i] = CHSV(0, 0, 0);
      FastLED.show();
    }

  }
}


void start()
{
  butt = !digitalRead(3); // считать текущее положение кнопки
  if (butt == 0)
  {
    flag = 1;
  //  radio.write(&peredacha, sizeof(peredacha));
  }
  Serial.println(butt);

  if (butt == 1 && flag == 1)
  {
    peredacha[0] = 1;
    Serial.println("On");
    LastOn = millis();
    while (millis() - LastOn < ON_ALL)
    {
      plamyOn(1);
      mig(1);
      servo(1);
      botinok(1);
      Serial.println(millis() - LastOn);
    }
    Serial.println("Off");
    plamyOn(0);
    mig(0);
    servo(0);
    botinok(0);
    delay(100);
    radio.write(&peredacha, sizeof(peredacha));
    /* while (margo == 0)
      {
        radio.write(&peredacha, 1);
        if (radio.available(&pipeNo)) radio.read(&margo, 1);
        mig(1);
        Serial.println(peredacha);
      }
    */
    peredacha[0] = 0;
    flag = 0;
  }
}

void botinok(int mode)
{
  if (mode == 1)
  {
    for (int i = 0; i < NUM_BOT; i++)
    {
      bot[i] = CHSV(0, 255, 255);
      FastLED.show();
    }
  }
  if (mode == 0)
  {
    for (int i = 0; i < NUM_BOT; i++)
    {
      bot[i] = CHSV(0, 0, 0);
      FastLED.show();
    }
  }
}
