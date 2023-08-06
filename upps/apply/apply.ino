#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define Water_level_Sensor_Pin A0

int Water_level_Value = 0;
int hour = 0, minute = 0, second = 0;
char ch = ':';

#include <Servo.h>
Servo RS;
Servo LS;
int Rpin = 3;
int Lpin = 5;
int ang1 = 20;
int ang2 = 20;
char inputkey;
int pump_left = 4;
int pump_right = 2;


int IN1Pin = 6;
int IN2Pin = 7;
int IN3Pin = 8;
int IN4Pin = 9;
int ENAPin = 10;
int ENBPin = 11;

void setup() {
  // lcd
  lcd.init();
  lcd.init();
  lcd.backlight();
  pinMode(Water_level_Sensor_Pin, INPUT);


  // 서보모터 (팔)
  Serial.begin(9600);
  RS.attach(Rpin);
  LS.attach(Lpin);
  RS.write(ang1);
  LS.write(ang2);


  // DC모터 (워터펌프)
  pinMode(pump_right, OUTPUT);
  pinMode(pump_left, OUTPUT);
  digitalWrite(pump_right, HIGH);
  digitalWrite(pump_left, HIGH);


  // DC모터 (바퀴)
  pinMode(IN1Pin, OUTPUT);
  pinMode(IN2Pin, OUTPUT);
  pinMode(IN3Pin, OUTPUT);
  pinMode(IN4Pin, OUTPUT);
  analogWrite(ENAPin, 255);
  analogWrite(ENBPin, 255);

}






void loop() {
  char in_data;
  in_data = Serial.read();
  Water_level_Value = analogRead(Water_level_Sensor_Pin);
  Serial.println(in_data);




  // 조향 코드  (+속도제어)
  if (in_data == '+')
  {
    analogWrite(ENAPin, 255);
    analogWrite(ENBPin, 255);
  }
  if (in_data == '.')
  {
    analogWrite(ENAPin, 150);
    analogWrite(ENBPin, 150);
  }
  if (in_data == '-')
  {
    analogWrite(ENAPin, 80);
    analogWrite(ENBPin, 80);
  }



  if (in_data == 's')
  {
    digitalWrite(IN1Pin, HIGH);
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, HIGH);
    digitalWrite(IN4Pin, LOW);
    delay(1000);
  }
  else if (in_data == 'w')//맞음
  {
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, HIGH);
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, HIGH);
    delay(1000);
  }
  else if (in_data == 'd')
  {
    digitalWrite(IN1Pin, HIGH);
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, HIGH);
  }
  else if (in_data == 'a') //a
  {
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, HIGH);
    digitalWrite(IN3Pin, HIGH);
    digitalWrite(IN4Pin, LOW);
  }
  else if (in_data == 'x')
  {
    digitalWrite(IN1Pin, HIGH);
    digitalWrite(IN2Pin, HIGH);
    digitalWrite(IN3Pin, HIGH);
    digitalWrite(IN4Pin, HIGH);
  }






  // lcd 수위랑 타이머(다른 기능이 작동할 때에는 초단위 정상작동 x) 코드


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("wl:");

  if (Water_level_Value >= 650)
  {
    lcd.setCursor(3, 0);
    lcd.print("100");
  }
  else if ((Water_level_Value < 650) && (Water_level_Value >= 500))
  {
    lcd.setCursor(4, 0);
    int wl = (Water_level_Value - 450) * 0.5;
    lcd.print(wl);
  }
  else
  {
    lcd.setCursor(4, 0);
    int wl = Water_level_Value * 0.05;
    lcd.print(wl);
  }

  lcd.setCursor(6, 0);
  lcd.print("%");


  lcd.setCursor(0, 1);
  lcd.print("Time ");
  lcd.print(hour);
  lcd.print(ch);
  lcd.print(minute);
  lcd.print(ch);
  lcd.print(second);
  second++;
  delay(1000);

  if (second == 60)
  {
    second = 0;
    minute = minute + 1;
    lcd.clear();
  }
  if (minute == 60)
  {
    minute = 0;
    hour = hour + 1;
    lcd.clear();
  }





  // 서보모터 제어 코드

  if (Serial.available()) {
    if ( in_data == '0')
    {
      for (int i = 0; i < 71; i++)
      {
        if (ang1 > 20)
        {
          ang1 -= 1;
          RS.write(ang1);
        }
        if (ang2 < 90)
        {
          ang2 += 1;
          LS.write(ang2);

        }
        delay(50);
      }
    }
  }

  else if (in_data == '1') {
    for (int i = 0; i < 71; i++) {
      if (ang1 < 90)
      {
        ang1 += 1;
        RS.write(ang1);
        delay(50);
      }
    }
  }

  else if (in_data == '2') {
    for (int i = 0; i < 71; i++) {
      if (ang2 > 20)
      {
        ang2 -= 1;
        LS.write(ang2);
      }
      delay(50);
    }

    if ( in_data == '3')
    {
      for (int i = 0; i < 71; i++)
      {
        if (ang1 < 90)
        {
          ang1 += 1;
          RS.write(ang1);
        }
        if (ang2 > 20)
        {
          ang2 -= 1;
          LS.write(ang2);
        }
        delay(50);
      }
    }



    // 워터펌프 티쳐블머신 연동 제어 코드

    else if (in_data == 'k') {
      digitalWrite(pump_right, LOW);
      delay(2000);
      digitalWrite(pump_right, HIGH);
    }
    else if (in_data == 'j') {
      digitalWrite(pump_left, LOW);
      delay(2000);
      digitalWrite(pump_left, HIGH);
    }
  }
}
