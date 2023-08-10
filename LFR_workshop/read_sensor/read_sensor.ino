// #define sensor1 A3      // Left most sensor
// #define sensor2 A2      // Left sensor
// #define sensor3 A1      // Middle Left sensor
// #define sensor4 A0      // Middle Right sensor
// #define sensor5 2       // Right sensor
// #define sensor6 3       // Right most sensor
// #define sensor7 4
// #define sensor8 5

#define sensor1 A0      // Left most sensor
#define sensor2 A1      // Left sensor
#define sensor3 A2      // Middle Left sensor
#define sensor4 A3      // Middle Right sensor
#define sensor5 A4       // Right sensor

// Initial Values of Sensors
int sensor[8] = { 0, 0, 0, 0, 0 };
int size_of_array = 8;
int B = 0, R = 0, L = 0, F = 0;
char path[10];
int i = 0, j = 0;
// Motor Pins
#define ENA 6
#define motorInput1 7
#define motorInput2 8
#define motorInput3 9
#define motorInput4 10
#define ENB 11

int left_motor_speed = 0;
int right_motor_speed = 0;

int spedr = 60;
int spedl = 60;
int spedf = 60;

// ERROR VARS
int LEFT = 100;
int RIGHT = 101;
int WHITE = 102;
int BLACK = 103;
int DelayCheck = 16;

int DelayForth = 100;
int DelayEvery = 100;
int DelayReverse = 200;
int DelaySharp = 200;
int delaytest = 3000;
int DelayWhiteError = 50;
int GoodPosition = 3;

int t = 0;
int Time = 0;

int initial_motor_speed = 60;

// Output Pins for Led
//int ledPin1 = A3;
//int ledPin2 = A4;

// PID Constants
float Kp = 10.00; // Will be tuned on track
float Ki = 0; //these contants will differ in eee and
float Kd = 10.00 ;//cse fest depending on track

// PID Variables
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;

int flag = 0;

#include <NewPing.h>
NewPing sonar_left(A4,A5,30);
// NewPing sonar_front(A4,A7,30);
NewPing sonar_right(A4,12,30);
/***************************************************************************************  Void Setup  **************************************************************************************/
void setup() {
  Serial.begin(9600);                     //setting serial monitor at a default baund rate of 9600
  delay(500);
  Serial.println("Started !!");
  delay(1000);
}
/***************************************************************************************  Void Loop  **************************************************************************************/

bool IR_value_check(int a[], int b[]){
  for (int i = 1;  i < size_of_array-1; i++){
    if( a[i] != b[i]){
      return false;
    }
  }
  return true;
}

void loop()
{
  read_sensor_values();
}

int black[8] = { 0, 0, 0, 0, 0};
int white[8] = { 1, 1, 1, 1, 1};

int err4[8] =  { 0, 1, 1, 1, 1 };
int err3[8] =  { 0, 0, 1, 1, 1 };
int err2[8] =  { 1, 0, 1, 1, 1 };
int err1[8] =  { 1, 0, 0, 1, 1 };
int err0[8] =  { 1, 1, 0, 1, 1 };
int err_1[8] = { 1, 1, 0, 0, 1 };
int err_2[8] = { 1, 1, 1, 0, 1 };
int err_3[8] = { 1, 1, 1, 0, 0 };
int err_4[8] = { 1, 1, 1, 1, 0 };

void read_sensor_values(){
  sensor[0] = digitalRead(sensor1);   //  analogRead digitalRead
  sensor[1] = digitalRead(sensor2);
  sensor[2] = digitalRead(sensor3);
  sensor[3] = digitalRead(sensor4);
  sensor[4] = digitalRead(sensor5);

  if (IR_value_check(sensor, white)) error = WHITE;
  else if (IR_value_check(sensor, black)) error = BLACK;
  else if (IR_value_check(sensor, err4)) error = 4;
  else if (IR_value_check(sensor, err3)) error = 3;
  else if (IR_value_check(sensor, err2)) error = 2;
  else if (IR_value_check(sensor, err1)) error = 1;
  else if (IR_value_check(sensor, err0)) error = 0;
  else if (IR_value_check(sensor, err_1)) error = -1;
  else if (IR_value_check(sensor, err_2)) error = -2;
  else if (IR_value_check(sensor, err_3)) error = -3;
  else if (IR_value_check(sensor, err_4)) error = -4;
  else error = 0;


  Serial.print(sensor[0]);
  Serial.print("\t");
  Serial.print(sensor[1]);
  Serial.print("\t");
  Serial.print(sensor[2]);
  Serial.print("\t");
  Serial.print(sensor[3]);
  Serial.print("\t");
  Serial.print(sensor[4]);
  Serial.print("\t");
  Serial.print(error);
  Serial.println("");
  
}
