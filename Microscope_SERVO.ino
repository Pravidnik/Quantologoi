#define PIN_NIZ     9 // светодиод снизу
#define PIN_LEV     6 // светодиод слева
#define PIN_VERH    7 // светодиод сверху
#define PIN_LBOT    12 // лента в левом ботинке
#define PIN_RBOT    11 // лента в правом ботинке
#define PIN_IR      A3 // пин для подключения датчика расстояния. Обязательно аналоговый!
#define PIN_SERVO   3 // пин для подключения серво-привода
#define NUM_LEDS    20 // кол-во светодиодов в ленте
#define DIST_NEAR   30 // ближняя дистанция срабатывания
#define DIST_MID    40 // средняя
#define DIST_FAR    60 // дальняя
#define SERVO_START 170  // начальное положение серво-привода
#define SERVO_END   30 // конечное положение серво-привода
#define ARRAY       5 // массив для вычисления среднего числа при измерении дистанции

#include <SharpIR.h>
#include <Servo.h>

SharpIR sensor(PIN_IR, 1080);
Servo myServo;

int posNow;
int distArray[ARRAY];
int distAverage;

void setup() {
  myServo.attach(PIN_SERVO);
  myServo.write(SERVO_START);
  Serial.begin(9600);

  // записываем начальные значения расстояни в массив
  for (int i = 0; i < ARRAY; i++) {
    distArray[i] = 30;
  }
}

void loop() {
  for (int i = 0; i < ARRAY; i++) {
    if (i == ARRAY - 1) { // если добрались до последнего элемента
      distArray[ARRAY - 1] = sensor.distance(); // получаем расстояние с датчика и сохраняем в последний элемент массива
    } else {
      distArray[i] = distArray[i + 1]; // двигаем все значения в массие назад на один
    }
  }

  arrayAverage(); // корректирует distAverageage в зависимости от значений в массиве distArray
  arrayDisplay(); // выведение значений массива и их среднего значения на экран - ВАЖНО закомментить его при "рабочей прошивке", так как это влияет на серву - глючить может

  // поднимаем или опускаем серву от полученных значений
  posNow = myServo.read();
  if (distAverage < DIST_NEAR) {
    /*servoUp();*/
    myServo.write(SERVO_END);
    
  } else {
    myServo.write(SERVO_START);
    /*servoDown();*/
  }

  delay(200); // выставляем задержку, чтобы показания с датчика собирались не слишком часто
}

void arrayAverage() {
  int distSumForAverage = 0;
  for (int i = 0; i < ARRAY; i++)
  {
    distSumForAverage = distArray[i] + distSumForAverage;
  }
  distAverage = distSumForAverage / ARRAY; // вычисление средней длины из последний пяти, например, значений
  
}

void arrayDisplay() {
  for (int i = 0; i < ARRAY; i++) {
    Serial.print(distArray[i]);
    Serial.print(" ");
  }
  Serial.print("/ average = ");
  Serial.print(distAverage);
  Serial.println("");
}

void servoUp() {
  if (posNow > SERVO_END) {
    for (int i = posNow; i <= SERVO_END; i--) {
      myServo.write(i);
      delay (7);
    }
  } else {
    myServo.write(SERVO_END);
  }
}

void servoDown() {
  if (posNow < SERVO_START) {
    for (int i = posNow; i <= SERVO_START; i++) {
      myServo.write(i);
      delay (7);
    }
  } else {
    myServo.write(SERVO_START);
  }
}
