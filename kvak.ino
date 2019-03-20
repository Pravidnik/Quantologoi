
//-------Настройки сервопривода-------------------
#define SERVO_PIN_1 11
#define MIN_POS_1 15 // Минимальная позиция сервопривода
#define MAX_POS_1 110
#define DELAY_1 1047




#define SERVO_PIN_2 9
#define MIN_POS_2 30 // Минимальная позиция сервопривода
#define MAX_POS_2 115 // Максимальная позиция сервопривода
#define DELAY_2 1047

#define TIME_ON 15000

#include <Servo.h>

int Red;

int Delay_Led;

Servo servo1;
Servo servo2;
int Pos;
unsigned long LastOn;
unsigned long LastOn_1;
unsigned long LastOn_2;
boolean flag = 0;
boolean flag_1;
boolean flag_2;
#include "FastLED.h"
//------------------
#define NUM_LEDS 35
#define PIN 5
#define BRIGHTNESS 255
#define MIN_HUE 110
#define MAX_HUE 170
#define MAX_BRIGH 255
#define MIN_BRIGH 200

#define MAX_DIST 80
#define AVER_DIST 40
#define MIN_DIST 20 // Меньше 15 не ставить!!!!!!!!!!!!!!!!!!

#define TRIG1_PIN 3
#define TRIG2_PIN 4
#define TRIG3_PIN 6
#define TRIG4_PIN 7

CRGB leds[NUM_LEDS];

int Dist;
int RanNum;
int Bright;
boolean Trig1;
boolean Trig2;
boolean Trig3;
boolean Trig4;

void setup() {
  servo1.attach(SERVO_PIN_1);
  servo2.attach(SERVO_PIN_2);
  servo1.write(MAX_POS_1);
  servo2.write(MAX_POS_2);
  //Pos = MIN_POS;

  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  pinMode(PIN, OUTPUT);
  pinMode(TRIG1_PIN, INPUT_PULLUP);
  pinMode(TRIG2_PIN, INPUT_PULLUP);
  pinMode(TRIG3_PIN, INPUT_PULLUP);
  pinMode(TRIG4_PIN, INPUT_PULLUP);
  Serial.begin(9600);

}

void loop()
{
  Trig2 = !digitalRead(TRIG2_PIN);
  //Trig1 = !digitalRead(TRIG1_PIN);
  //Serial.println(Trig2);
  if (Trig2 == 0 && flag == 1) flag = 0;
  if (Trig2 == 1 && flag == 0)
  {
    LastOn = millis();
    flag = 1;
    while (millis() - LastOn < TIME_ON)
    {
      servoOn1(1);
      servoOn2(1);
    }
  }
  //plamyOn(1);
  servoOn1(0);
  servoOn2(0);
  Serial.println(Trig2);
  //servo2.write(35);
  //delay(1000);
  // servo1.write(110);
  //delay(1000);
  //servo2.write(115);
  //delay(1000);
  //servo1.write(25);
  //delay(1000);

}

void loop228() {
  Trig1 = !digitalRead(TRIG1_PIN);
  //Serial.println(Trig1);
  Trig2 = !digitalRead(TRIG2_PIN);
  Serial.println(Trig2);
  // plamyOn(Trig2);
  if (Trig1 == 1)
  {
    servo2.write(30);
    // servo1.write(25);
    delay(1000);
    // servo2.write(100);
    //  servo1.write(110);
    delay(1000);

  }
  else
  {
    servo2.write(100);
    //servo1.write(110);
  }
  /*servo1.write(65);
    servo2.write(20);
    delay(1000);
    servo1.write(160);
    servo2.write(110);
    delay(1000);
  */

}

void plamyOn(int mode)
{
  if (mode == 1)
  {

    for (int i = 0; i < NUM_LEDS; i++)
    {
      Red = random(MIN_HUE, MAX_HUE);
      // Serial.println(Red);
      Bright = random(200, 255);
      leds[i] = CHSV(Red, 255, Bright);
      FastLED.show();
      Delay_Led = random(18, 29);
      delay(Delay_Led);
    }

  }
  else if (mode == 0)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CHSV(0, 0, 0);
      FastLED.show();
    }

  }
}







void servoOn1(int mode_1) {
  if (mode_1 == 1)
  {
    if (millis() - LastOn_1 > DELAY_1 && flag_1 == 0)
    {
      servo1.write(MIN_POS_1);
      flag_1 = 1;
      LastOn_1 = millis();
      // Serial.println("0011");
    }
    if (millis() - LastOn_1 > DELAY_1 && flag_1 == 1)
    {
      servo1.write(MAX_POS_1);
      flag_1 = 0;
      LastOn_1 = millis();
      //  Serial.println("1111");
    }
  }
  else
  {
    servo1.write(MAX_POS_1);
    flag_1 = 0;
  }
}

void servoOn2(int mode_2) {
  if (mode_2 == 1)
  {


    if (millis() - LastOn_2 > DELAY_2 && flag_2 == 0)
    {
      servo2.write(MIN_POS_2);
      flag_2 = 1;
      LastOn_2 = millis();
      // Serial.println("0022");
    }
    if (millis() - LastOn_2 > DELAY_2 && flag_2 == 1)
    {
      servo2.write(MAX_POS_2);
      flag_2 = 0;
      LastOn_2 = millis();
      // Serial.println("1122");
    }

  }
  else
  {
    servo2.write(MAX_POS_2);
    flag_2 = 0;
  }
}
