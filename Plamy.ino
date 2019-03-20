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
