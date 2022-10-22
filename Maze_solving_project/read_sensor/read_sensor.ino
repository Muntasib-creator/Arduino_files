#define sensor1 A3      // Left most sensor
#define sensor2 A2      // Left sensor
#define sensor3 A1      // Middle Left sensor
#define sensor4 A0      // Middle Right sensor
#define sensor5 2       // Right sensor
#define sensor6 3       // Right most sensor
#define sensor7 4
#define sensor8 5

// Initial Values of Sensors
int sensor[8] = { 0, 0, 0, 0, 0, 0 , 0, 0 };
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
/***************************************************************************************  Void Setup  **************************************************************************************/
void setup() {
  Serial.begin(9600);                     //setting serial monitor at a default baund rate of 9600
  delay(500);
  Serial.println("Started !!");
  delay(1000);
}
/***************************************************************************************  Void Loop  **************************************************************************************/

void loop()
{
  delay(100);
  sensor[0] = digitalRead(sensor1);   //  analogRead digitalRead
  sensor[1] = digitalRead(sensor2);
  sensor[2] = digitalRead(sensor3);
  sensor[3] = digitalRead(sensor4);
  sensor[4] = digitalRead(sensor5);
  sensor[5] = digitalRead(sensor6);
  sensor[6] = digitalRead(sensor7);
  sensor[7] = digitalRead(sensor8);


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
  Serial.print(sensor[5]);
  Serial.print("\t");
  Serial.print(sensor[6]);
  Serial.print("\t");
  Serial.print(sensor[7]);
  Serial.print("\n");
}
/***************************************************************************************  Calculate PID  **************************************************************************************/
