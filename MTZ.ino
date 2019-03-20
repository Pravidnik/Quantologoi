#include "FastLED.h"
//------Настройки ленты------------------
#define NUM_LEDS 57
#define PIN 13
#define BRIGHTNESS 255
#define MIN_BRIGHT 250
#define MAX_BRIGHT 255
#define HUE_MIN 0
#define HUE_MAX 20
#define HUE_NULL 0
#define FIRST_PIN 6
#define SECOND_PIN 4
#define THIRD_PIN 5
#define FOURTH_PIN 3
#define TIME_PRESS 5000

// пример с "бегущей радугой" для библиотеки FastLED
#define NUM_LEDS_MIG 4
#define PIN_MIG 2


CRGB leds[NUM_LEDS];

unsigned long FirstPress;
boolean Button1;
boolean Button2;
boolean Button3;
boolean Button4;
boolean Cout;
boolean mode = 0;
int Bright;
int Fading;
byte var = 0;
byte Red;

void setup() {
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  pinMode(PIN, OUTPUT);
  Serial.begin(9600);
  pinMode(FIRST_PIN, INPUT_PULLUP);
  pinMode(SECOND_PIN, INPUT_PULLUP);
  pinMode(THIRD_PIN, INPUT_PULLUP);
  pinMode(FOURTH_PIN, INPUT_PULLUP);
}

void loop()
{
ButtOn();
//LedOn(1);

}

void LedOn(int swit)
{
  if (swit == 1)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      Red = random(HUE_MIN, HUE_MAX);
     Red = HUE_NULL + Red;
    //  Serial.println(Red);
      Bright = random(MIN_BRIGHT, MAX_BRIGHT);
      leds[i] = CHSV(Red, 255, Bright);
      FastLED.show();
    }
  } 
  else
  {
      for (int i = Bright; i >= 0; i = i - 4)
      {
        Bright = i;
        for (int i = 0; i < NUM_LEDS; i++)
        {
          leds[i] = CHSV(Red, 255, Bright);
          FastLED.show();
        }
      if( Bright == 1) Bright = 0;
      }
    }
  }



void ButtOn() {
 // LedOn(mode);
  
  Button1 = !digitalRead(FIRST_PIN);
  if (Button1 == 1 && var == 0)
  {
    Serial.println(1);
    FirstPress = millis();
    var++;
    Cout = 0;
  }

  Button2 = !digitalRead(SECOND_PIN);
  if (Button2 == 1 && var == 1)
  {
    Serial.println(2);
    var++;
  }

  Button3 = !digitalRead(THIRD_PIN);
  if (Button3 == 1 && var == 2)
  {
    Serial.println(3);
    var++;
  }

  Button4 = !digitalRead(FOURTH_PIN);
  if (Button4 == 1 && var == 3)
  {

    var = 0;
    Cout = 1;
    mode = !mode;
    Serial.println(mode);
  }
  
  




  if (millis() - FirstPress > TIME_PRESS && Cout == 0)
  {
    var = 0;
    Serial.println("Lox");
    Cout = 1;
  }

}
