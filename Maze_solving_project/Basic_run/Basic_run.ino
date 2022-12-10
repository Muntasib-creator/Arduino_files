#define sensor1 A0      // Left most sensor
#define sensor2 A1      // Left sensor
#define sensor3 A2      // Middle Left sensor
#define sensor4 A3      // Middle Right sensor
#define sensor5 5       // Right sensor
#define sensor6 4       // Right most sensor
#define sensor7 3
#define sensor8 2

// Initial Values of Sensors
unsigned short sensor[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned short size_of_array = 8;
unsigned short B = 0, R = 0, L = 0, F = 0;
char path[20];
unsigned short  i = 0, j = 0;
// Motor Pins
#define ENA 6
#define right_forward 7
#define right_reverse 8
#define left_forward 9
#define left_reverse 10
#define ENB 11

int left_motor_speed = 0;
int right_motor_speed = 0;

unsigned short spedr = 60;
unsigned short spedl = 60;
unsigned short spedf = 60;


unsigned short DelayCheck = 16;

unsigned short DelayForth = 70;
unsigned short DelayEvery = 70;
unsigned short DelayReverse = DelayForth*2;
unsigned short DelaySharp = DelayForth*2;
unsigned short delaytest = 3000;
unsigned short DelayWhiteError = 30;
unsigned short GoodPosition = 3;

unsigned short t = 0;
unsigned short Time = 0;
unsigned short initial_motor_speed = 60;

// Output Pins for Led
//int ledPin1 = A3;
//int ledPin2 = A4;

// PID Constants
int Kp = 7; // Will be tuned on track
int Ki = 0; //these contants will differ in eee and
int Kd = 10 ;//cse fest depending on track


// PID Variables
int error = 0;
int P = 0, I = 0, D = 0, PID_value = 0;
int previous_error = 0, previous_I = 0;

unsigned short flag = 0;

// ERROR VARS
unsigned short LEFT = 100;
unsigned short RIGHT = 101;
unsigned short WHITE = 102;
unsigned short BLACK = 103;
unsigned short WALL = 105;

// Sonar Library
#include <NewPing.h>
NewPing sonar_left(A4,A5,24);
// NewPing sonar_front(A4,A7,30);
NewPing sonar_right(A4,12,24);
int sLeft, sRight;

void setup() {
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);
  pinMode(sensor6, INPUT);
  pinMode(sensor7, INPUT);
  pinMode(sensor8, INPUT);

  pinMode(right_forward, OUTPUT);
  pinMode(right_reverse, OUTPUT);
  pinMode(left_forward, OUTPUT);
  pinMode(left_reverse, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  /*pinMode(ledPin1, OUTPUT);
    digitalWrite(ledPin1, LOW);*/

  Serial.begin(9600);   //setting serial monitor at a default baund rate of 9600
  delay(500);
  Serial.println("Started !!");
  delay(3000);
}

void read_sensor_values();
void forward();
void sharpLeftTurn();
void sharpRightTurn();
void stop_bot();
void goSharpLeft();
void goSharpRight();
void goWhiteTurn();
void goBlackTurn();
void followWall();

void loop(){
  read_sensor_values();
  if      (error == WALL)  followWall();
  else if (error == LEFT)  goSharpLeft();
  else if (error == RIGHT) goSharpRight();
  else if (error == WHITE) goWhiteTurn();
  else if (error == BLACK) goBlackTurn();
  else                     goPID();
}

void goSharpLeft(){
  // if(error != LEFT) return;
  L++;
  Time = 0;
  while (Time <= DelayForth) {
    forward();
    delay(1);
    Time++;
    // read_sensor_values();
    // if (error == BLACK) {
    //   goto Black;
    // }
  }
  Time = 0;
  read_sensor_values();
  if (error == WHITE) {
    path[i++] = 'L';
    while (error < -GoodPosition || error > GoodPosition) {
      analogWrite(ENA, spedr); //Right Motor Speed
      analogWrite(ENB, spedl); //Left Motor Speed
      sharpLeftTurn();
      read_sensor_values();
    }
  }
  else {  
    while(true){
      analogWrite(ENA, spedr); //Right Motor Speed
      analogWrite(ENB, spedl); //Left Motor Speed
      sharpLeftTurn();
      read_sensor_values();
      if (sensor[7]==1||sensor[6]==1)break;
    }
    analogWrite(ENA, spedr); //Right Motor Speed
    analogWrite(ENB, spedl); //Left Motor Speed
    sharpLeftTurn();
    delay(DelaySharp);
    // read_sensor_values();
    while (error < -GoodPosition || error > GoodPosition) {
      analogWrite(ENA, spedr); //Right Motor Speed
      analogWrite(ENB, spedl); //Left Motor Speed
      sharpLeftTurn();
      read_sensor_values();
    }
  }
}
void goSharpRight(){
  Time = 0;
  do {
    forward();
    delay(1);
    Time++;
    // read_sensor_values();
    // if (error == BLACK) {
    //   goto Black;
    // }
  } while (Time <= DelayForth);
  Time = 0;
  read_sensor_values();
  if (error == WHITE) {
    path[i++] = 'R';
    while (error < -GoodPosition || error > GoodPosition) {
      analogWrite(ENA, spedr);
      analogWrite(ENB, spedl);
      sharpRightTurn();
      read_sensor_values();
    }
  }

}
void goWhiteTurn(){
  Time = 0;
  while (Time <= DelayWhiteError) {
    forward();
    Time++;
    read_sensor_values();
    if (error != WHITE)return;
  }
  Time = 0;
  while (error < -GoodPosition || error > GoodPosition) {
    Serial.print("XX=");
    Serial.print(error);
    Serial.print("\t");

    analogWrite(ENA, spedr);
    analogWrite(ENB, spedl);
    sharpRightTurn();
    read_sensor_values();
  }
}

void goBlackTurn(){
  analogWrite(ENA, spedf);
  analogWrite(ENB, spedf);
  forward();
  delay(DelayEvery);
  read_sensor_values();
  if (error == BLACK) {     /**** Finish End Reached, Stop! ****/
    stop_bot();
    delay(5000);
  }
  else if (error == WHITE) {       /**** Move Left ****/
    while (error < -GoodPosition || error > GoodPosition ) {
      analogWrite(ENA, spedr); 
      analogWrite(ENB, spedl);
      sharpLeftTurn();
      read_sensor_values();
    }

  }
  else {
    analogWrite(ENA, spedr);
    analogWrite(ENB, spedl);
    sharpLeftTurn();
    delay(DelaySharp);
    // read_sensor_values();
    while (error < -GoodPosition || error > GoodPosition) {
      analogWrite(ENA, spedr);
      analogWrite(ENB, spedl);
      sharpLeftTurn();
      read_sensor_values();
    }
  }
}

void followWall(){
  int pivot = 16;
  int diff = (pivot-sLeft)/2;  // if left diff pos, if right diff neg
  unsigned short sKp = 4;
  // unsigned short sKd = 0;
  // unsigned short sKi = 10;
  left_motor_speed = initial_motor_speed-20 + diff*sKp;
  right_motor_speed = initial_motor_speed-20 - diff*sKp;

  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);

  Serial.print("diff: ");
  Serial.print(diff);
  Serial.print("\t");
  Serial.print("L: ");
  Serial.print(left_motor_speed);
  Serial.print("\t");
  Serial.print("R: ");
  Serial.print(right_motor_speed);
  Serial.print("\t");
  
  analogWrite(ENA, left_motor_speed);
  analogWrite(ENB, right_motor_speed);
  forward();
  return;
}

bool IR_value_check(int a[], int b[]){
  for (int i = 1;  i < size_of_array-1; i++){
    if(a[i] == 2 || b[i] == 2) continue;
    if(a[i] != b[i]) return false;
  }
  return true;
}
void read_sensor_values() {

  sensor[0] = digitalRead(sensor1);
  sensor[1] = digitalRead(sensor2);
  sensor[2] = digitalRead(sensor3);
  sensor[3] = digitalRead(sensor4);
  sensor[4] = digitalRead(sensor5);
  sensor[5] = digitalRead(sensor6);
  sensor[6] = digitalRead(sensor7);
  sensor[7] = digitalRead(sensor8);
  sLeft = sonar_left.ping_cm();
  sRight = sonar_right.ping_cm();

  unsigned short white[8] = {0,0,0,0,0,0,0,0};
  unsigned short black[8] = {2,1,1,1,1,1,1,2};
  unsigned short err5[8] = {2,1,0,0,0,0,0,2};
  unsigned short err4[8] = {2,1,1,0,0,0,0,2};
  unsigned short err3[8] = {2,0,1,0,0,0,0,2};
  unsigned short err2[8] = {2,0,1,1,0,0,0,2};
  unsigned short err1[8] = {2,0,0,1,0,0,0,2};
  unsigned short err0[8] = {2,0,0,1,1,0,0,2};
  unsigned short err_1[8] = {2,0,0,0,1,0,0,2};
  unsigned short err_2[8] = {2,0,0,1,1,0,0,2};
  unsigned short err_3[8] = {2,0,0,0,0,1,0,2};
  unsigned short err_4[8] = {2,0,0,0,0,1,1,2};
  unsigned short err_5[8] = {2,0,0,0,0,0,1,2};
  
  if (sLeft != 0) error = WALL;
  else if (sensor[0] == 1) error = LEFT; // found left
  else if (sensor[7] == 1) error = RIGHT;  // found right
  else if (IR_value_check(sensor, white)){error = WHITE;}//found white
  else if (IR_value_check(sensor, black)){error = BLACK;}//found Black
  else if (IR_value_check(sensor, err5)){error = 5;}
  else if (IR_value_check(sensor, err4)){error = 4;}
  else if (IR_value_check(sensor, err3)){error = 3;}
  else if (IR_value_check(sensor, err2)){error = 2;}
  else if (IR_value_check(sensor, err1)){error = 1;}
  else if (IR_value_check(sensor, err0)){error = 0;} 
  else if (IR_value_check(sensor, err_1)){error = -1;} 
  else if (IR_value_check(sensor, err_2)){error = -2;} 
  else if (IR_value_check(sensor, err_3)){error = -3;} 
  else if (IR_value_check(sensor, err_4)){error = -4;} 
  else if (IR_value_check(sensor, err_5)){error = -5;}
  else {error = 0;}

  Serial.print("\n");
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
  Serial.print("\t");
  Serial.print(error);
  Serial.print("\t");
  Serial.print("left: ");
  Serial.print(sLeft);
  Serial.print("\t");
  Serial.print("right: ");
  Serial.print(sRight);
  Serial.print("\t");

}

void goPID() {
  P = error;
  I = I + previous_I;
  D = error - previous_error;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);

  previous_I = I;
  previous_error = error;

  // Calculating the effective motor speed:
  left_motor_speed = initial_motor_speed - PID_value;
  right_motor_speed = initial_motor_speed + PID_value;

  // The motor speed should not exceed the max PWM value
  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);

  /*Serial.print(PID_value);
    Serial.print("\t");
    Serial.print(left_motor_speed);
    Serial.print("\t");
    Serial.println(right_motor_speed);*/

  analogWrite(ENA, left_motor_speed);
  analogWrite(ENB, right_motor_speed);
  forward();
}
void motor_forward_test() {
  analogWrite(ENA, 80);
  analogWrite(ENB, 80);
  forward();
}

void forward() {
  digitalWrite(right_forward, HIGH);
  digitalWrite(right_reverse, LOW);
  digitalWrite(left_forward, HIGH);
  digitalWrite(left_reverse, LOW);
}
void reverse() {
  digitalWrite(right_forward, LOW);
  digitalWrite(right_reverse, HIGH);
  digitalWrite(left_forward, LOW);
  digitalWrite(left_reverse, HIGH);
}
void sharpLeftTurn() {
  digitalWrite(right_forward, LOW);
  digitalWrite(right_reverse, HIGH);
  digitalWrite(left_forward, HIGH);
  digitalWrite(left_reverse, LOW);
}
void sharpRightTurn() {
  digitalWrite(right_forward, HIGH);
  digitalWrite(right_reverse, LOW);
  digitalWrite(left_forward, LOW);
  digitalWrite(left_reverse, HIGH);
}
void stop_bot() {
  //All motors at ease
  digitalWrite(right_forward, LOW);
  digitalWrite(right_reverse, LOW);
  digitalWrite(left_forward, LOW);
  digitalWrite(left_reverse, LOW);
}