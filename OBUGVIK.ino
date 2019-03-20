#define NUM_LEDS 4
#include "FastLED.h"
#define PIN 3
#define BRIGHTNESS 255
CRGB leds[NUM_LEDS];

#include <SharpIR.h>

#define ir A3
#define model 1080
// ir: the pin where your sensor is attached
// model: an int that determines your sensor:  1080 for GP2Y0A21Y
//                                            20150 for GP2Y0A02Y
//                                            (working distance range according to the datasheets)

SharpIR SharpIR(ir, model);

//-------Настройки сервопривода-------------------
#define SERVO_PIN 10

#define MIN_POS 5// Минимальная позиция сервопривода
#define MAX_POS 175 // Максимальная позиция сервопривода
#define DELAY_1 750

#define DELAY_2 750
#define MIN_DELAY 250
#define MAX_DELAY 350

#define TIME_ON 15000

#include <Servo.h>

Servo servo;
unsigned long lastled;
int ledstatus;
int Pos;
unsigned long LastOn = 0;
int Dst;
int flag = 0;
void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
  servo.write(MIN_POS);
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(255);
  pinMode(PIN, OUTPUT);
}


void loop()
{
  light(150);
  servoOn();
}



void light(int sped) {
  if (millis() - lastled > sped && ledstatus == 0)
  {
    for (int i = 0; i < NUM_LEDS; i++ ) {
      leds[i] = CHSV(0 , 0, 255);  // HSV. Увеличивать HUE (цвет)
      FastLED.show();
      // умножение i уменьшает шаг радуги
    }
    Serial.println("led0");
    lastled = millis();
    ledstatus = 1;
  }


  if (millis() - lastled > sped && ledstatus == 1)
  {
    for (int i = 0; i < NUM_LEDS; i++ ) {
      leds[i] = CHSV(0 , 0, 0);  // HSV. Увеличивать HUE (цвет)
      FastLED.show();
      // умножение i уменьшает шаг радуги
    }
    Serial.println("led1");
    lastled = millis();
    ledstatus = 0;
  }


}





void servoOn() {
  if (millis() - LastOn > DELAY_1 && flag == 0)
  {
    servo.write(MIN_POS);
    flag = 1;
    LastOn = millis();
    Serial.println("0000");
  }
  if (millis() - LastOn > DELAY_2 && flag == 1)
  {
    servo.write(MAX_POS);
    flag = 0;
    LastOn = millis();
  Serial.println("1111");
  }
}
