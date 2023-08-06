#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#define Water_level_Sensor_Pin A0

Servo RS;
Servo LS;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int Water_level_Value = 0;
int hour = 0, minute = 0, second = 0;
char ch = ':';

char inputkey = 's';

int IN1Pin = 11;
int IN2Pin = 10;
int IN3Pin = 9;
int IN4Pin = 6;
int relay_r = 2; //in1
int relay_l = 4; //in2
int Rpin = 5;
int Lpin = 7;

int ang1 = 0;
int ang2 = 90;

unsigned long time_previous, time_current;

void Motor1(int);
void Motor2(int);

void setup() {

  // 릴레이 워터펌프
  pinMode(relay_r, OUTPUT);
  pinMode(relay_l, OUTPUT);
  digitalWrite(relay_r, HIGH);
  digitalWrite(relay_l, HIGH); // HIGH가 꺼진 것

  // 서보모터 (팔)
  RS.attach(Rpin);
  LS.attach(Lpin);
  RS.write(ang1);
  LS.write(ang2);



  // DC모터 (바퀴)
  pinMode(IN1Pin, OUTPUT);
  pinMode(IN2Pin, OUTPUT);
  pinMode(IN3Pin, OUTPUT);
  pinMode(IN4Pin, OUTPUT);
  Motor1(0);
  Motor2(-0);
  Serial.begin(9600);

  lcd.init();
  lcd.init();
  lcd.backlight();
  pinMode(Water_level_Sensor_Pin, INPUT);
}



void loop() {    //1

  lcd.init();
  lcd.init();
  lcd.backlight();
  pinMode(Water_level_Sensor_Pin, INPUT);
  char in_data;
  int i;
  if (Serial.available()) {     //2
    in_data = Serial.read();
    Serial.println(in_data);

    if (in_data == 'w') {        // 앞

      for (i = 0; i < 10000; i++) {
        Motor1(100);
        Motor2(-100);
        delay(200);
        Motor1(0);
        Motor2(0);
        delay(300);
        in_data = Serial.read();
        if (in_data == 's')
          break;

        if (in_data == 'r') {
          digitalWrite(relay_r, LOW);
          delay(1000);
          digitalWrite(relay_r, HIGH);
        }
        if (in_data == 'l') {
          digitalWrite(relay_l, LOW);
          delay(1000);
          digitalWrite(relay_l, HIGH);
        }
      }
    }

    else if (in_data == 's') { //정지
      Motor1(0);
      Motor2(0);
    }
    else if (in_data == 'd') { //왼쪽
      Motor1(150);
      Motor2(150);
      delay(1500);
      Motor1(0);
      Motor2(0);
      delay(150);
      Motor1(150);
      Motor2(150);
      delay(1500);
      
    }
    else if (in_data == 'a') { // 오른쪽
      Motor1(-150);
      Motor2(-150);
      delay(1500);
      Motor1(0);
      Motor2(0);
      delay(1500);
      Motor1(-150);
      Motor2(-150);
      delay(1500);
    }
    else if (in_data == 'x') { // 뒷쪽
      for (i = 0; i < 10000; i++) {
        Motor1(-200);
        Motor2(200);
        delay(100);
        Motor1(0);
        Motor2(0);
        delay(300);
        in_data = Serial.read();
        if (in_data == 's')
          break;
      }
    }
    else if ( in_data == '0') { // 양쪽 접힌 상태
      for (int i = 0; i < 100; i++) {
        if (ang1 > 0) {
          ang1 -= 1;
          RS.write(ang1);
        }
        if (ang2 < 90) {
          ang2 += 1;
          LS.write(ang2);
        }
        delay(50);
      }
    }
    else if (in_data == '1') {//왼쪽만 펼침
      for (int i = 0; i < 100; i++) {
        if (ang1 < 90) {
          ang1 += 1;
          RS.write(ang1);
          delay(50);
        }
      }
    }
    else if (in_data == '2') {// 오른쪽만 펼침
      for (int i = 0; i < 100; i++) {
        if (ang2 > 0) {
          ang2 -= 1;
          LS.write(ang2);
        }
        delay(50);
      }
    }
    else if (in_data == '3') { // 양쪽 펼친 상태
      for (int i = 0; i < 100; i++) {
        if (ang1 < 90) {
          ang1 += 1;
          RS.write(ang1);
        }
        if (ang2 > 0) {
          ang2 -= 1;
          LS.write(ang2);
        }
        delay(50);
      }
    }

    // 릴레이 워터펌프
    if (in_data == 'r') {
      digitalWrite(relay_r, LOW);
      delay(1000);
      digitalWrite(relay_r, HIGH);
    }
    if (in_data == 'l') {
      digitalWrite(relay_l, LOW);
      delay(1000);
      digitalWrite(relay_l, HIGH);
    }
  }
}

void Motor1(int a)
{
  a = constrain(a , -255, 255);
  if (a > 0)
  {
    analogWrite(IN1Pin, a);
    analogWrite(IN2Pin, 0);
  } else
  {
    analogWrite(IN1Pin, 0);
    analogWrite(IN2Pin, -a);
  }
}

void Motor2(int a)
{
  a = constrain(a , -255, 255);
  if (a > 0)
  {
    analogWrite(IN3Pin, a);
    analogWrite(IN4Pin, 0);
  } else
  {
    analogWrite(IN3Pin, 0);
    analogWrite(IN4Pin, -a);
  }
}
