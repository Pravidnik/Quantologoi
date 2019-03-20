// пример с "бегущей радугой" для библиотеки FastLED
#define NUM_LEDS 39
#include "FastLED.h"
#define PIN 4
#define BRIGHTNESS 255
#define RAINBOW_SPEED  15  // скорость движения радуг
#define STEP_RAINBOW  7// Чем больше ставишь, тем меньше шаг радуги
CRGB leds[NUM_LEDS];
byte counter = 0;

#define ON_TIME 10000

unsigned long LastStep;
unsigned long LastLastStep;
#include "GyverEncoder.h"

// Пины энкодера
#define CLK 7
#define DT 8
#define SW 9


Encoder enc1(CLK, DT, SW);


byte value;

void setup() {
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  pinMode(PIN, OUTPUT);
  enc1.setType(TYPE2);    // тип энкодера TYPE1 одношаговый, TYPE2 двухшаговый. Если ваш энкодер работает странно, смените тип
  Serial.begin(9600);
}

void loop()
{
  ledOn();

}


void trig()
{

  enc1.tick();
  if (enc1.isRight() || enc1.isLeftH())
  {

    value = value + 5;   // если был поворот направо, увеличиваем на 1
    LastStep = millis();
  }
  if (enc1.isLeft() || enc1.isRightH())
  {

    value = value - 5;
    LastStep = millis();
  }
  /* if (value > 75) value = 25;
    if (value   < 0) value = 0;

    rain = map(value, 0, 75, 0, 25);
    if (rain > 25) rain = 25;
    if (rain < 0) rain = 0;
    Serial.println(rain);
    STEP_RAINBOW = rain;
  */
  //rain = random(0, 255);
  // Serial.println(value);
}

void rain() {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = CHSV(counter + i * STEP_RAINBOW , 255, 255);  // HSV. Увеличивать HUE (цвет)
    // умножение i уменьшает шаг радуги
  }
  counter++;        // counter меняется от 0 до 255 (тип данных byte)
  FastLED.show();
  delay(RAINBOW_SPEED);         // скорость движения радуг
  trig();
}

void ledOn() {
  trig();
  while (millis() - LastStep > ON_TIME) rain(); trig();
  while (millis() - LastStep < ON_TIME)
  {
    trig();
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CHSV(value, 255, 255);
    }
    //counter++;        // counter меняется от 0 до 255 (тип данных byte)
    FastLED.show();
    //delay(RAINBOW_SPEED);         // скорость движения радуг
    Serial.print(value); Serial.print("  "); Serial.println(LastStep);
  }



}
