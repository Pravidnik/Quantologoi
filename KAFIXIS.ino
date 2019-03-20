#include "FastLED.h"
#define PIN 13        // пин DI
#define NUM_LEDS 30   // число диодов
#define Brightness 255 // Яркость светодиодов 255 Максимум
#define SPEED_LEDS 3 // Скорость светодидов
#define BUTTON_PIN 3  // пин кнопки button1

#define DEBOUNCE 3        // (мс), антидребезг

#define LENGHT 7




#include "Adafruit_NeoPixel.h"
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

//----------Настройка синего цвета-------
#define BLUE_PIN 7
#define BLUE_LEDS 3
#define MIN_BLUE 0
#define MAX_BLUE 100
#define HUE_MIN_BLUE 130
#define HUE_MAX_BLUE 180

//----------Настройка красного цвета-------
#define RED_PIN 10
#define RED_LEDS 14
#define MIN_RED 250
#define MAX_RED 250
#define NULL_RED 0
#define HUE_MIN_RED 0
#define HUE_MAX_RED 15 

//----------Настройка оранжевого цвета-------
#define ORANGE_PIN 9
#define ORANGE_LEDS 7
#define MIN_ORANGE 150
#define MAX_ORANGE 255
#define HUE_MIN_ORANGE 15
#define HUE_MAX_ORANGE 30

//----------Настройка желтого цвета-------
#define YELLOW_PIN 7
#define YELLOW_LEDS 10
#define MIN_YELLOW 150
#define MAX_YELLOW 255
#define HUE_MIN_YELLOW 30
#define HUE_MAX_YELLOW 60



#define BRIGHTNESS 255
CRGB Blue[BLUE_LEDS];
CRGB Red[RED_LEDS ];
CRGB Orange[ORANGE_LEDS];
CRGB Yellow[YELLOW_LEDS];

byte BrightBlue;
byte BrightRed;
byte BrightOrange;
byte BrightYellow;
byte HueBlue;
byte HueRed;
byte HueOrange;
byte HueYellow;

boolean butt_flag = 1;
boolean butt;
boolean led_flag = 0;
unsigned long last_press;

int FirstLed;
int LastLed;
int Brightless;
int NumberLed;



void setup() {
  strip.begin();
  strip.setBrightness(Brightness);    // яркость, от 0 до 255
  strip.clear();                          // очистить
  strip.show();                           // отправить на ленту
  FastLED.addLeds<WS2812, BLUE_PIN, GRB>(Blue, BLUE_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812, RED_PIN, GRB>(Red, RED_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812, ORANGE_PIN, GRB>(Orange, ORANGE_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812, YELLOW_PIN, GRB>(Yellow, YELLOW_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  pinMode(PIN3, OUTPUT);
  pinMode(PIN4, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);
}


void loop()
{
  butt = !digitalRead(BUTTON_PIN); // считать текущее положение кнопки

  if (butt == 1 && butt_flag == 0 && millis() - last_press > DEBOUNCE)
  {
    butt_flag = 1;
    Serial.println("Button pressed");
    last_press = millis();
  }
  if (butt == 0 && butt_flag == 1)
  {
    butt_flag = 0;
    Serial.println("Button released");
    if (led_flag == 0)
    {
      led_flag = 1;
    }
    else if  (led_flag == 1)
    {
      led_flag = 0;
    }
    line(led_flag);
  }
  red(led_flag);
  Serial.println(led_flag);
}

// plamy

void blue(int mode) {
  if (mode == 0)
  {
    for (int i = 0; i < BLUE_LEDS; i++)
    {
      Blue[i] = CHSV(0, 0, 0);
      FastLED.show();
    }
  }
  if (mode == 1)
  {
    for (int i = 0; i < BLUE_LEDS; i++)
    {
      BrightBlue = random(MIN_BLUE, MAX_BLUE);
      HueBlue = random(HUE_MIN_BLUE, HUE_MAX_BLUE);
      Blue[i] = CHSV(HueBlue, 255, BrightBlue);
      FastLED.show();
     // delay(5);
     // Serial.println(BrightBlue);
    }
  }
}

void red(int mode) {
  if (mode == 0)
  {
    for (int i = 0; i < RED_LEDS; i++)
    {
      Red[i] = CHSV(0, 0, 0);
      FastLED.show();
    delay(5);
    }
  }
  if (mode == 1)
  {
    for (int i = 0; i < RED_LEDS; i++)
    {
      BrightRed = random(MIN_RED, MAX_RED);
      HueRed = random(HUE_MIN_RED, HUE_MAX_RED);
      HueRed = NULL_RED + HueRed;
      Red[i] = CHSV(HueRed, 255, BrightRed);
      FastLED.show();
      //delay(10);
     // Serial.println(BrightRed);
    }
  }
}

void orange(int mode) {
  if (mode == 0)
  {
    for (int i = 0; i < ORANGE_LEDS; i++)
    {
      Orange[i] = CHSV(0, 0, 0);
      FastLED.show();
    }
  }
  if (mode == 1)
  {
    for (int i = 0; i < ORANGE_LEDS; i++)
    {
      BrightOrange = random(MIN_ORANGE, MAX_ORANGE);
      HueOrange = random(HUE_MIN_ORANGE, HUE_MAX_ORANGE);
      Orange[i] = CHSV(HueOrange, 255, BrightOrange);
      FastLED.show();
      //delay(10);
      //Serial.println(BrightOrange);
    }
  }
}

void yellow(int mode) {
  if (mode == 0)
  {
    for (int i = 0; i < YELLOW_LEDS; i++)
    {
      Yellow[i] = CHSV(0, 0, 0);
      FastLED.show();
    }
  }
  if (mode == 1)
  {
    for (int i = 0; i < YELLOW_LEDS; i++)
    {
      BrightYellow = random(MIN_YELLOW, MAX_YELLOW);
      HueYellow = random(HUE_MIN_YELLOW, HUE_MAX_YELLOW);
      Yellow[i] = CHSV(HueYellow, 255, BrightYellow);
      FastLED.show();
      //delay(10);
      Serial.println(BrightYellow);
    }
  }
}

// Stepler

void line(int mode)
{
  if (mode == 1)
  {
    Serial.println("On");
   for (int i = NUM_LEDS + LENGHT; i >= -LENGHT; i--)
    {
      LastLed = i;
      FirstLed = LastLed - LENGHT;
      
      for (int i = FirstLed; i <= LastLed; i++)
      {
        Brightless = 255 / LENGHT;
        NumberLed = LastLed - i;
        Brightless = Brightless * NumberLed;
        strip.setPixelColor(i, Brightless, 0, 0);    // залить красным
        strip.show();                         // отправить на ленту
      Serial.println(i);
       delay(SPEED_LEDS);
      }
      strip.setPixelColor(LastLed, 0, 0, 0);     // залить красным
      strip.show();                         // отправить на ленту
    }
  }
  if (mode == 0)
  {
    Serial.println("Off");
   for (int i = NUM_LEDS + LENGHT; i >= -LENGHT; i--)
    {
      LastLed = i;
      FirstLed = LastLed - LENGHT;
      
      for (int i = FirstLed; i <= LastLed; i++)
      {
        Brightless = 255 / LENGHT;
        NumberLed = LastLed - i;
        Brightless = Brightless * NumberLed;
        strip.setPixelColor(i, 0, 0, Brightless);    // залить красным
        strip.show();                         // отправить на ленту
      Serial.println(i);
       delay(SPEED_LEDS);
      }
      strip.setPixelColor(LastLed, 0, 0, 0);     // залить красным
      strip.show();                         // отправить на ленту
    }
  }
}
