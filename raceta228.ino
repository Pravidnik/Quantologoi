
#define NUM_LEDS_2 50
#include "FastLED.h"
#define PIN_2 10

#define PIN 2        // пин DI
#define NUM_LEDS 53   // число диодов
#define Brightness 255 // 255 Максимум
#define SpeedRock 25 // Скорость погашения светодиодов при взлете
#define GREEN 10
#define RED 15
// Пины энкодера
#define CLK 4
#define DT 5
#define SW 6

#define POT_PIN A0 // Пин потенциометра

#define TIME_ON 5000


#define POT_ANGLE 90 // Минимальный угол поворота второй ручки для запуска(угол выставляется не совсем точно, нужно подбирать)

#include "GyverEncoder.h"

#define TURN 2

int value = 0;
int Angle;
int Number;
Encoder enc1(CLK, DT, SW);

int br;

#include "Adafruit_NeoPixel.h"
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);


CRGB leds[NUM_LEDS];
byte counter;

unsigned long LastOn;

void setup() {
  strip.begin();
  strip.setBrightness(Brightness);    // яркость, от 0 до 255
  strip.clear();                          // очистить
  strip.show();                           // отправить на ленту
  enc1.setType(TYPE2);    // тип энкодера TYPE1 одношаговый, TYPE2 двухшаговый. Если ваш энкодер работает странно, смените тип
  Serial.begin(9600);
  FastLED.addLeds<WS2811, PIN_2, GRB>(leds, NUM_LEDS_2).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(255);
  pinMode(PIN_2, OUTPUT);
}




void loop() {
  // put your main code here, to run repeatedly:
  LedOn();
  StarRock();
}

void Encod()
{
  enc1.tick();

  if (enc1.isRightH() || enc1.isRight() || enc1.isLeft() || enc1.isLeftH())
  {
    value++;      // если был поворот направо, увеличиваем на 1

  }
  // if (enc1.isLeft()) value++;     // если был поворот налево, увеличиваем на 1

}

void LedOn()
{
  Encod();
  if (value > NUM_LEDS * TURN) value = NUM_LEDS * TURN;
  Number = map(value, 0, NUM_LEDS * TURN, -1, NUM_LEDS);
  if (Number > NUM_LEDS) Number = NUM_LEDS;
  if (Number >= 0 && Number <= GREEN)
  {
    for (int i = 0; i <= Number; i++)
    {
      strip.setPixelColor(i, 0x00ff00);     // залить зелёным
      strip.show();                         // отправить на ленту
    }
  }
  if (Number > GREEN && Number < NUM_LEDS - RED)
  {
    for (int i = GREEN; i <= Number; i++)
    {
      strip.setPixelColor(i, 0xffff00);      // залить жёлтым
      strip.show();                         // отправить на ленту
    }
  }
  if (Number >= NUM_LEDS - RED)
  {
    for (int i = NUM_LEDS - RED; i < Number; i++)
    {
      strip.setPixelColor(i, 0xff0000);     // залить красным
      strip.show();                         // отправить на ленту
    }
  }
}

void led()
{
  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = CHSV(120, 255, 255);  // HSV. Увеличивать HUE (цвет)
    FastLED.show();
    // умножение i уменьшает шаг радуги
  }

  //  delay(RAINBOW_SPEED);         // скорость движения радуг

  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = CHSV(0 , 0, 0);  // HSV. Увеличивать HUE (цвет)
    FastLED.show();
    // умножение i уменьшает шаг радуги
  }

  //delay(RAINBOW_SPEED);         // скорость движения радуг


}

void led3()
{
  for (int d = 180; d >= 0; d = d - 5)
  {
    for (int i = 0; i < NUM_LEDS; i++ ) {
      br = 170 - (d / 3);
      leds[i] = CHSV( br, 255, 255);  // HSV. Увеличивать HUE (цвет)
      FastLED.show();
      // умножение i уменьшает шаг радуги
    }
    delay(d);         // скорость движения радуг
    for (int i = 0; i < NUM_LEDS; i++ ) {
      leds[i] = CHSV(0 , 0, 0);  // HSV. Увеличивать HUE (цвет)
      FastLED.show();
      // умножение i уменьшает шаг радуги
    }
  }
}

void led4()
{
  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = CHSV(165, 255, 200);  // HSV. Увеличивать HUE (цвет)
    FastLED.show();
    // умножение i уменьшает шаг радуги
  }
  delay(5);         // скорость движения радуг

  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = CHSV(0 , 0, 0);  // HSV. Увеличивать HUE (цвет)
    FastLED.show();
    // умножение i уменьшает шаг радуги
  }
  delay(5);
}

void StarRock()
{
  Angle = analogRead(POT_PIN);
  Angle = Angle / 5.68;
  Serial.print(value);
  Serial.print("  ");
  Serial.println(Angle);
  if (value >= 106)
  {
    if (Angle < POT_ANGLE)
    {
      for (int i = NUM_LEDS; i >= 0; i-- ) {   // всю ленту
        strip.setPixelColor(i, 0x000000);     // залить чёрным
        strip.show();                         // отправить на ленту
        delay(SpeedRock);
      }


      value = 0;
      // тут код для запуска ракеты(Свечение ракеты и кручение)
      led3();
      LastOn = millis();

      while (millis() - LastOn < TIME_ON)
      {
        led4();
      }

    }
  }
}
