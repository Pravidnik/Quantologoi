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
