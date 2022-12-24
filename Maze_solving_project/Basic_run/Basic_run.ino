#define sensor1 A0  // Left most sensor
#define sensor2 A1  // Left sensor
#define sensor3 A2  // Middle Left sensor
#define sensor4 A3  // Middle Right sensor
#define sensor5 5   // Right sensor
#define sensor6 4   // Right most sensor
#define sensor7 3
#define sensor8 2

bool FOLLOWWALL = true;

// Initial Values of Sensors
int sensor[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int size_of_array = 8;
int B = 0, R = 0, L = 0, F = 0;
char path[40];int i = 0, j = 0;
// char path[100] = {"RLRRSR"};int i = 6, j = 0;
// Motor Pins
#define ENA 6  // Left motor control
#define right_forward 7
#define right_reverse 8
#define left_forward 10
#define left_reverse 9
#define ENB 11  // Right motor control

int left_motor_speed = 0;
int right_motor_speed = 0;

int spedr = 60;
int spedl = 60;
int spedf = 60;
int speedL = 0;
int speedR = 0;

int DelayCheck = 16;

int DelayForth = 180;
int DelayWhiteForth = 100;
int DelayEvery = 70;
int DelayReverse = DelayForth * 2;
int DelaySharp = 100;
int delaytest = 3000;
int DelayWhiteError = 100;
int GoodPosition = 2;

int counter1, counter2;
int t = 0;
int Time = 0;
int initial_motor_speed = 60;

// Output Pins for Led
//int ledPin1 = A3;
//int ledPin2 = A4;

// PID Constants
int Kp = 12;  // Will be tuned on track
int Ki = 0;   //these contants will differ in eee and
int Kd = 10;  //cse fest depending on track


// PID Variables
int error = 0, max_error = 5;
int P = 0, I = 0, D = 0, PID_value = 0;
int previous_error = 0, previous_I = 0;

int flag = 0;

// ERROR VARS
int LEFT = 100;
int RIGHT = 101;
int WHITE = 102;
int BLACK = 103;
int WALL = 105;

// Sonar Library
#include <NewPing.h>
NewPing sonar_left(A4, A5, 20);
// NewPing sonar_front(A4,A7,30);
NewPing sonar_right(A4, 12, 20);
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

  Serial.begin(9600);  //setting serial monitor at a default baund rate of 9600
  delay(500);
  Serial.println("Started !!");
  delay(3000);
}

void read_sensor_values();
bool read_black_values();
void forward();
void reverse();
void sharpLeftTurn();
void sharpRightTurn();
void stopBot();
void goSharpLeft();
void goSharpRight();
void goWhiteTurn();
void goBlackTurn();
void goPID();
bool escapeLine();
void motor_forward_test();
void calcSharpSpeed();
void sharpCalibrate();
bool conditionLeft();
bool conditionRight();
bool followWall();
void calcShortestPath();
void goShortestWay();

void loop() {
  read_sensor_values();
  if (error == WALL) followWall();
  else if (error == LEFT) goSharpLeft();
  else if (error == RIGHT) goSharpRight();
  else if (error == WHITE) goWhiteTurn();
  else if (error == BLACK) goBlackTurn();
  else goPID();
  // goShortestWay();
}

void goSharpLeft() {
  // if(error != LEFT) return;
  L++;
  counter1 = millis();
  while (true) {
    analogWrite(ENA, spedf);
    analogWrite(ENB, spedf);
    forward();
    counter2 = millis();
    if (counter2 - counter1 >= DelayForth) break;
  }
  read_sensor_values();
  if (error == WHITE) {
    while (GoodPosition < error) {
      calcSharpSpeed();
      sharpLeftTurn();
      read_sensor_values();
    }
    sharpCalibrate();
  } else if (read_black_values()) return goBlackTurn();
  else {
    counter1 = millis();
    while (true) {
      analogWrite(ENA, spedr);
      analogWrite(ENB, spedl);
      sharpLeftTurn();
      read_sensor_values();
      if (escapeLine(LEFT, counter1)) break;
    }
    // read_sensor_values();
    while (GoodPosition < error) {
      calcSharpSpeed();
      sharpLeftTurn();
      read_sensor_values();
    }
    sharpCalibrate();
  }
  path[i++] = 'L';
}

void goSharpRight() {
  counter1 = millis();
  while (true) {
    analogWrite(ENA, spedf);
    analogWrite(ENB, spedf);
    forward();
    delay(1);
    read_sensor_values();
    if (error == LEFT) return goSharpLeft();
    counter2 = millis();
    if (counter2 - counter1 >= DelayForth) break;
  }
  if (error == WHITE) {
    while (-GoodPosition > error || error > 99) {
      calcSharpSpeed();
      sharpRightTurn();
      read_sensor_values();
    }
    sharpCalibrate();
    path[i++] = 'R';
  } else if (read_black_values()) return goBlackTurn();
  else path[i++] = 'S';
}
void goWhiteTurn() {
  counter1 = millis();
  while (true) {
    analogWrite(ENA, spedf);
    analogWrite(ENB, spedf);
    forward();
    delay(1);
    read_sensor_values();
    if (error != WHITE) return;
    else if (error == WALL && followWall()) return;
    counter2 = millis();
    if (counter2 - counter1 >= DelayWhiteError) break;
  }
  // stopBot();
  // delay(5000);
  while (GoodPosition + 2 < error) {
    analogWrite(ENA, spedr + 20);
    analogWrite(ENB, spedl);
    sharpLeftTurn();
    read_sensor_values();
  }
  sharpCalibrate();
  path[i++] = 'B';
}

void goBlackTurn() {
  stopBot();
  delay(5000);
  Serial.print("\n");
  Serial.println(i);
  for (int ii = 0; ii < i; ii++) {
    Serial.print(path[ii]);
  }
  Serial.print("\n");
  Serial.print("\ncalcShortestPath\n");
  calcShortestPath();
  Serial.println(i);
  for (int ii = 0; ii < i; ii++) {
    Serial.print(path[ii]);
  }
  Serial.print("\n");
  goShortestWay();
  stopBot();
  delay(5000);
}

void calcShortestPath() {
  j = i;
  while (true) {
    int n = i;
    int flag = 0;
    for (i = 1; i < n - 1; i++) {
      if (path[i] == 'B') {
        flag = 1;
        if (path[i - 1] == 'L' && path[i + 1] == 'L') {  // LBL = S
          path[i - 1] = 'S';
          path[i] = 'X';
          path[i + 1] = 'X';
        } else if (path[i - 1] == 'L' && path[i + 1] == 'R') {  // LBR = B
          path[i - 1] = 'B';
          path[i] = 'X';
          path[i + 1] = 'X';
        } else if (path[i - 1] == 'L' && path[i + 1] == 'S') {  // LBS = R
          path[i - 1] = 'R';
          path[i] = 'X';
          path[i + 1] = 'X';
        } else if (path[i - 1] == 'R' && path[i + 1] == 'L') {  // RBL = B
          path[i - 1] = 'B';
          path[i] = 'X';
          path[i + 1] = 'X';
        } else if (path[i - 1] == 'R' && path[i + 1] == 'R') {  // RBR = S  // unnecessary
          path[i - 1] = 'S';
          path[i] = 'X';
          path[i + 1] = 'X';
        } else if (path[i - 1] == 'R' && path[i + 1] == 'S') {  // RBS = L  // unnecessary
          path[i - 1] = 'L';
          path[i] = 'X';
          path[i + 1] = 'X';
        } else if (path[i - 1] == 'S' && path[i + 1] == 'L') {  // SBL = R
          path[i - 1] = 'R';
          path[i] = 'X';
          path[i + 1] = 'X';
        } else if (path[i - 1] == 'S' && path[i + 1] == 'R') {  // SBR = L  // unnecessary
          path[i - 1] = 'L';
          path[i] = 'X';
          path[i + 1] = 'X';
        } else if (path[i - 1] == 'S' && path[i + 1] == 'S') {  // SBS = B
          path[i - 1] = 'B';
          path[i] = 'X';
          path[i + 1] = 'X';
        }
      }
    }
    i++;
    if (flag == 0) {
      break;
    }
    char temp[40];
    for (i = 0, j = 0; i < n; i++) {
      if (path[i] == 'X') {
        continue;
      } else {
        temp[j++] = path[i];
      }
    }
    for (i = 0; i < j; i++) {
      path[i] = temp[i];
    }
  }
  return;
}

void goShortestWay() {
  j = 0;
  while (true) {
    read_sensor_values();
    if (error == LEFT || error == RIGHT || error == BLACK) {
      if (i == j) {
        while (true) {
          analogWrite(ENA, spedf);
          analogWrite(ENB, spedf);
          forward();
          delay(1);
          counter2 = millis();
          if (counter2 - counter1 >= DelayForth) break;
        }
        read_sensor_values();
        if (error == LEFT || error == RIGHT || error == BLACK) {
          return;
        }
      } else if (path[j] == 'L') {
        counter1 = millis();
        while (true) {
          analogWrite(ENA, spedf);
          analogWrite(ENB, spedf);
          forward();
          delay(1);
          counter2 = millis();
          if (counter2 - counter1 >= DelayForth) break;
        }
        read_sensor_values();
        if (error != WHITE) {
          counter1 = millis();
          while (true) {
            analogWrite(ENA, spedr);
            analogWrite(ENB, spedl);
            sharpLeftTurn();
            read_sensor_values();
            if (escapeLine(LEFT, counter1)) break;
          }
        }
        // read_sensor_values();
        while (GoodPosition < error) {
          calcSharpSpeed();
          sharpLeftTurn();
          read_sensor_values();
        }
        sharpCalibrate();
      } else if (path[j] == 'R') {
        counter1 = millis();
        while (true) {
          analogWrite(ENA, spedf);
          analogWrite(ENB, spedf);
          forward();
          delay(1);
          counter2 = millis();
          if (counter2 - counter1 >= DelayForth) break;
        }
        read_sensor_values();
        if (error != WHITE) {
          counter1 = millis();
          while (true) {
            analogWrite(ENA, spedr);
            analogWrite(ENB, spedl);
            sharpRightTurn();
            read_sensor_values();
            if (escapeLine(RIGHT, counter1)) break;
          }
        }
        // read_sensor_values();
        while (-GoodPosition > error || error > 99) {
          calcSharpSpeed();
          sharpRightTurn();
          read_sensor_values();
        }
        sharpCalibrate();
      } else if (path[j] == 'S') {
        counter1 = millis();
        while (true) {
          analogWrite(ENA, spedf);
          analogWrite(ENB, spedf);
          forward();
          delay(1);
          counter2 = millis();
          if (counter2 - counter1 >= DelayForth) break;
        }
      }
      j++;
    } else if (error == WHITE) {  // Line White error
      error = 0;
      goPID();
    } else if (error == WALL) followWall();
    else goPID();
  }
}

bool followWall() {
  if (!FOLLOWWALL) return false;
  stopBot();
  delay(3000);
  counter1 = millis();
  for (int k = 0; k < 5; k++) {
    read_sensor_values();
    if (error != WALL && sLeft <= 8) return false;
  }
  counter2 = millis();
  Serial.println('\n');
  Serial.println(counter2 - counter1);
  while (true) {
    int pivot = 14;
    int diff = (pivot - sLeft) / 2;  // if left diff pos, if right diff neg
    int sKp = 7;
    left_motor_speed = initial_motor_speed + diff * sKp;
    right_motor_speed = initial_motor_speed - diff * sKp;

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

    read_sensor_values();
    if (error != WALL) {
      stopBot();
      delay(3000);
      return true;
    }
  }
}

bool escapeLine(int direction, int counter) {
  counter2 = millis();
  if (counter2 - counter < 150 && (direction == LEFT && (sensor[0] == 1 || sensor[1] == 1) || (direction == RIGHT && (sensor[6] == 1 || sensor[7] == 1)))) {
    analogWrite(ENA, spedr);
    analogWrite(ENB, spedl);
    if (direction == LEFT) sharpLeftTurn();
    else if (direction == RIGHT) sharpRightTurn();
    delay(DelaySharp);
    return true;
  }
  if (counter2 - counter1 >= 400) return true;
  return false;
}

bool IR_value_check(int a[], int b[]) {
  for (int i = 1; i < size_of_array - 1; i++) {
    if (a[i] == 2 || b[i] == 2) continue;
    if (a[i] != b[i]) return false;
  }
  return true;
}

int white[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int black[8] = { 2, 1, 1, 1, 1, 1, 1, 2 };
int err5[8] = { 2, 1, 0, 0, 0, 0, 0, 2 };
int err4[8] = { 2, 1, 1, 0, 0, 0, 0, 2 };
int err3[8] = { 2, 0, 1, 0, 0, 0, 0, 2 };
int err2[8] = { 2, 0, 1, 1, 0, 0, 0, 2 };
int err1[8] = { 2, 0, 0, 1, 0, 0, 0, 2 };
int err0[8] = { 2, 0, 0, 1, 1, 0, 0, 2 };
int err_1[8] = { 2, 0, 0, 0, 1, 0, 0, 2 };
int err_2[8] = { 2, 0, 0, 0, 1, 1, 0, 2 };
int err_3[8] = { 2, 0, 0, 0, 0, 1, 0, 2 };
int err_4[8] = { 2, 0, 0, 0, 0, 1, 1, 2 };
int err_5[8] = { 2, 0, 0, 0, 0, 0, 1, 2 };

int err33[8] = { 2, 1, 1, 1, 0, 0, 0, 2 };
int err11[8] = { 2, 0, 1, 1, 1, 0, 0, 2 };
int err_11[8] = { 2, 0, 0, 1, 1, 1, 0, 2 };
int err_33[8] = { 2, 0, 0, 0, 1, 1, 1, 2 };

void read_sensor_values() {
  sensor[0] = digitalRead(sensor1);
  sensor[1] = digitalRead(sensor2);
  sensor[2] = digitalRead(sensor3);
  sensor[3] = digitalRead(sensor4);
  sensor[4] = digitalRead(sensor5);
  sensor[5] = digitalRead(sensor6);
  sensor[6] = digitalRead(sensor7);
  sensor[7] = digitalRead(sensor8);
  if (FOLLOWWALL) {
    sLeft = sonar_left.ping_cm();
    // sRight = sonar_right.ping_cm();
  }

  if (sLeft != 0) error = WALL;
  else if (sensor[0] == 1) error = LEFT;
  else if (sensor[7] == 1) error = RIGHT;
  else if (IR_value_check(sensor, white)) error = WHITE;
  else if (IR_value_check(sensor, black)) error = BLACK;
  else if (IR_value_check(sensor, err5)) error = 5;
  else if (IR_value_check(sensor, err4)) error = 4;
  else if (IR_value_check(sensor, err3)) error = 3;
  else if (IR_value_check(sensor, err33)) error = 3;
  else if (IR_value_check(sensor, err2)) error = 2;
  else if (IR_value_check(sensor, err1)) error = 1;
  else if (IR_value_check(sensor, err11)) error = 1;
  else if (IR_value_check(sensor, err0)) error = 0;
  else if (IR_value_check(sensor, err_1)) error = -1;
  else if (IR_value_check(sensor, err_11)) error = -1;
  else if (IR_value_check(sensor, err_2)) error = -2;
  else if (IR_value_check(sensor, err_3)) error = -3;
  else if (IR_value_check(sensor, err_33)) error = -3;
  else if (IR_value_check(sensor, err_4)) error = -4;
  else if (IR_value_check(sensor, err_5)) error = -5;
  else error = 0;

  // int us = 3;
  // int ii = 4;
  // double dd = 2.0;
  Serial.print("\n");
  // Serial.print(us>ii);
  // Serial.print(dd>ii);
  // Serial.print(dd>us);
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

bool read_black_values() {
  if (digitalRead(sensor2) == 0) return false;
  if (digitalRead(sensor3) == 0) return false;
  if (digitalRead(sensor4) == 0) return false;
  if (digitalRead(sensor5) == 0) return false;
  if (digitalRead(sensor6) == 0) return false;
  if (digitalRead(sensor7) == 0) return false;
  return true;
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

void calcSharpSpeed() {
  analogWrite(ENA, spedr);
  analogWrite(ENB, spedl);
  return;
  if (abs(error) > 5) {
    analogWrite(ENA, spedr);
    analogWrite(ENB, spedl);
    return;
  }
  if (abs(error) <= 1) {
    analogWrite(ENA, spedr);
    analogWrite(ENB, spedl);
    return;
  }
  int speed = constrain(spedr - (5 - abs(error)) * 10, 30, 60);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void sharpCalibrate() {
  stopBot();
  delay(600);
  return;
  read_sensor_values();
  // while(abs(error) < 1){
  //   analogWrite(ENA, spedf);
  //   analogWrite(ENB, spedf);
  //   forward();
  //   delay(1);
  //   read_sensor_values();
  // }
  if (error > 0) {
    speedL = 0;
    speedR = 60;
  } else {
    speedL = 60;
    speedR = 0;
  }
  while (abs(error) > 2) {
    analogWrite(ENA, speedL);
    analogWrite(ENB, speedR);
    forward();
    delay(1);
    read_sensor_values();
  }
}

bool conditionLeft() {
  if (error >= -max_error && error <= GoodPosition) return false;
  return true;
}

bool conditionRight() {
  if (error <= max_error && error > -GoodPosition) return false;
  return true;
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
void stopBot() {
  digitalWrite(right_forward, LOW);
  digitalWrite(right_reverse, LOW);
  digitalWrite(left_forward, LOW);
  digitalWrite(left_reverse, LOW);
}