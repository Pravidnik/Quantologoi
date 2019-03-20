//----------Настройка сонара--------
#include <NewPing.h>
#define TRIG 3
#define ECHO 7
#define DIST_MAX 300
#define REACT_DIST 15

NewPing sonar(TRIG, ECHO, DIST_MAX);

int Dst;
//-------Настройки сервопривода-------------------
#define SERVO_PIN 9
#define MIN_POS 10// Минимальная позиция сервопривода
#define MAX_POS 170 // Максимальная позиция сервопривода
#define ON_SPEED_SERVO 100
#define OFF_SPEED_SERVO 100


#define ON_TIME 5000

#define MIN_DELAY 150 // +25
#define MAX_DELAY 50 

// пример с "бегущей радугой" для библиотеки FastLED
#define NUM_LEDS 4
#include "FastLED.h"
#define PIN 13
#define BRIGHTNESS 255
#define RAINBOW_SPEED 15  // скорость движения радуг
#define STEP_RAINBOW 400// Чем больше ставишь, тем меньше шаг радуги
CRGB leds[NUM_LEDS];
byte counter;
unsigned long lastled;
unsigned long LastOn;
int ledstatus;
#include <Servo.h>
Servo servo;

#define TIME_SERVO_OFF 1054

unsigned long LastServoOff;

int Pos;
unsigned long TimeServo;
int Dist;
void setup() {
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(255);
  pinMode(PIN, OUTPUT);
  servo.attach(SERVO_PIN);
  Serial.begin(9600);
}

void loop228()
{
  Sonar(150);
  //delay(527);
}

void loop()
{
  Sonar(150);
  if (Dst > 5 && Dst < 30)
  {
   // delay(MIN_DELAY);
    delay(350);
    for (int i = 0; i <= MAX_DELAY; i++)
    {
      Sonar(125);
      if(Dst > 5 && Dst < 30){
      LastOn = millis();
      while (millis() - LastOn < ON_TIME)
      {
        onAll();
      }
      break;
    }
    delay(1);
    Serial.print("LOX "); Serial.println(Dst);
    }
    lightOn();
    ServoOff();
    delay(257);
  }
  if (millis - LastServoOff > TIME_SERVO_OFF)
  {
    lightOn();
    ServoOff();
    delay(257);
    LastServoOff = millis();
  }

}

void onAll() {
  while (Pos < MAX_POS)
  {
    ServoRight();
    light(400);

  }

  while (Pos > MIN_POS)
  {
    ServoLeft();
    light(400);
  }
}

void ServoOff()
{
 // Pos = servo.read();
  servo.write(90);
 // Serial.println(Pos);

}

void ServoRight() //Плавный подьем сервопривода
{
  if (millis() - TimeServo > ON_SPEED_SERVO && Pos < MAX_POS)
  {
    Pos++;
    servo.write(Pos);
    TimeServo = millis();
    // Serial.println(Pos);
  }
}

void ServoLeft() //Плавный подьем сервопривода
{
  if (millis() - TimeServo > OFF_SPEED_SERVO && Pos > MIN_POS)
  {
    Pos--;
    servo.write(Pos);
    TimeServo = millis();
    // Serial.println("ON");
  }
}

void lightOn()
{
for (int i = 0; i < NUM_LEDS; i++ ) {
      leds[i] = CHSV(0 , 0, 0);  // HSV. Увеличивать HUE (цвет)
      FastLED.show();
      // умножение i уменьшает шаг радуги
    }
}

void light(int sped) {
  if (millis() - lastled > sped && ledstatus == 0)
  {
    for (int i = 0; i < NUM_LEDS; i++ ) {
      leds[i] = CHSV(0 , 0, 255);  // HSV. Увеличивать HUE (цвет)
      FastLED.show();
      // умножение i уменьшает шаг радуги
    }
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
    lastled = millis();
    ledstatus = 0;
  }


}

void Sonar(int timeStop) {
  Dst = sonar.ping_cm();
  Serial.println(Dst);
  delay(timeStop);
}
