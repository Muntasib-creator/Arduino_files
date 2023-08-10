/* IR sensor pins */
#define sensor1 A0  
#define sensor2 A1  
#define sensor3 A2  
#define sensor4 A3  
#define sensor5 A4 

// Initial Values of Sensors
int sensor[5] = { 0, 0, 0, 0, 0 };
int size_of_array = 5;
// Motor Pins
#define ENA 6  // Left motor control
#define right_forward 7
#define right_reverse 8
#define left_forward 9
#define left_reverse 10
#define ENB 11  // Right motor control

int left_motor_speed = 0;
int right_motor_speed = 0;

int initial_motor_speed = 80;
int right_motor_calibrate = 0;
int left_motor_calibrate = 0;

// PID Constants
int Kp = 11;  // Will be tuned on track
int Ki = 0;   //these contants will differ in eee and
int Kd = 0;  //cse fest depending on track

int spedr = 90;
int spedl = 90;
int spedf = initial_motor_speed;
int GoodPosition = 3;
int DelayWhiteError = 100;

int counter1, counter2;
int t = 0;
int Time = 0;

// PID Variables
int error = 0, max_error = 4;
int P = 0, I = 0, D = 0, PID_value = 0;
int previous_error = 0, previous_I = 0;

int flag = 0;

// ERROR VARS
int LEFT = 100;
int RIGHT = 101;
int WHITE = 102;
int BLACK = 103;

void setup() {
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);

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

// Initialize Functions
void read_sensor_values();
void forward();
void reverse();
void stopBot();
void goWhiteTurn();
void goBlackTurn();
void goPID();

void loop() {
  read_sensor_values();
  if (error == WHITE) goWhiteTurn();
  else if (error == BLACK) goBlackTurn();
  else goPID();
}

void goWhiteTurn() {
  counter1 = millis();
  bool goLeft;
  if (previous_error >= 0) goLeft = false;
  else goLeft = true;
  while (true) {
    analogWrite(ENA, spedf);
    analogWrite(ENB, spedf);
    forward();
    delay(1);
    read_sensor_values();
    if (error != WHITE) return;
    counter2 = millis();
    if (counter2 - counter1 >= DelayWhiteError) break;
  }
  stopBot();
  delay(500);
  while (error > GoodPosition || error < -GoodPosition) {
    analogWrite(ENA, spedr);
    analogWrite(ENB, spedl);
    if (goLeft) sharpLeftTurn();
    else sharpRightTurn();
    read_sensor_values();
  }
  stopBot();
  delay(500);
}

void goBlackTurn() {
  return;
  stopBot();
  delay(5000);
}

bool IR_value_check(int a[], int b[]) {
  for (int i = 0; i < size_of_array; i++) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

int black[5] = { 0, 0, 0, 0, 0};
int white[5] = { 1, 1, 1, 1, 1};

int err4[5] =  { 0, 1, 1, 1, 1 };
int err3[5] =  { 0, 0, 1, 1, 1 };
int err2[5] =  { 1, 0, 1, 1, 1 };
int err1[5] =  { 1, 0, 0, 1, 1 };
int err0[5] =  { 1, 1, 0, 1, 1 };
int err_1[5] = { 1, 1, 0, 0, 1 };
int err_2[5] = { 1, 1, 1, 0, 1 };
int err_3[5] = { 1, 1, 1, 0, 0 };
int err_4[5] = { 1, 1, 1, 1, 0 };

void read_sensor_values() {
  sensor[0] = digitalRead(sensor1);
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
  Serial.print(error);
  Serial.print("\n");
}

void goPID() {
  P = error;
  I = I + previous_I;
  D = error - previous_error;
  I = 0;
  D = 0;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);

  previous_I = I;
  previous_error = error;

  // Calculating the effective motor speed:
  left_motor_speed = initial_motor_speed + PID_value + left_motor_calibrate;
  right_motor_speed = initial_motor_speed - PID_value + right_motor_calibrate;

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

bool conditionLeft() {
  if (error >= -max_error && error <= GoodPosition) return false;
  return true;
}

bool conditionRight() {
  if (error <= max_error && error > -GoodPosition) return false;
  return true;
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