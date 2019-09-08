const int trigPin = 9;
const int echoPin = 10;
// defines variables
long duration;
int distance;
int ENA = 3; // скорость мотора
int IN1 = 4; // контакт 1 мотора
int IN2 = 5; // контакт 2 мотора
int ENB = 6;  // яркость светодиода
int IN3 = 7; // плюс светодиодов
int IN4 = 8; // минус светодиодов

unsigned long timerRotate;
unsigned long timerLed;

int flagRotate = 0;
int flagLed = 0;
int flagDist = 0;
const long interval = 4000;           // interval at which to blink (milliseconds)

void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  Serial.begin(9600); // Starts the serial communication
}

void loop()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  motorActive();
  ledActive();

  if (distance <= 20 && distance > 10)
  {
    flagDist = 1;
  }
}

// подсветка
void ledActive()
{
  if (flagDist == 1)
  {
    if (millis() - timerLed >= (interval / 8) && flagLed == 0) {
      timerLed = millis();
      analogWrite(ENA, 255);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      flagLed = 1;
      Serial.println("ON");
    }
    if (millis() - timerLed >= (interval / 8) && flagLed == 1) {
      timerLed = millis();
      analogWrite(ENA, 255);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      flagLed = 0;
      Serial.println("OFF");
    }
  }
  else {
    // выключаем все
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
}

// вращение моторчика
void motorActive()
{
  if (flagDist == 1) {
    if (millis() - timerRotate >= (interval / 4) && flagRotate == 0)
    {
      motorLeft();
      timerRotate = millis();
      flagRotate = 1;
    }
    if (millis() - timerRotate >= interval && flagRotate == 1)
    {
      motorStop();
      timerRotate = millis();
      flagRotate = 2;
    }
    if (millis() - timerRotate >= (interval / 4) && flagRotate == 2)
    {
      motorRight();
      timerRotate = millis();
      flagRotate = 3;
    }
    if (millis() - timerRotate >= interval && flagRotate == 3)
    {
      motorStop();
      timerRotate = millis();
      flagRotate = 0;
      flagDist = 0;
    }
  }
  else {
    // выключаем все
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
}

void motorLeft()
{
  analogWrite(ENB, 100);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("<===");
}

void motorRight()
{
  Serial.println("===>");
  analogWrite(ENB, 100);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void motorStop()
{
  Serial.println("=");
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
