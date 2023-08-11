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
#define ENA 11  // Left motor control
#define left_reverse 10   // IN1 = left_reverse
#define left_forward 9    // IN2 = left_forward
#define right_reverse 8   // IN3 = right_reverse
#define right_forward 7   // IN4 = right_forward
#define ENB 6  // Right motor control

// LED pins
#define left_LED 12
#define mid_LED 4
#define right_LED 3

// Motor speed variables
int left_motor_speed = 0;
int right_motor_speed = 0;

int initial_motor_speed = 80;
int right_motor_calibrate = 0;
int left_motor_calibrate = 0;

int spedr = 100;
int spedl = 100;
int spedf = initial_motor_speed;
int GoodPosition = 3;

// Delay Variables
int DelayWhiteError = 100;
int DelayBlackError = 150;

// PID Variables
int Kp = 11;  // Will be tuned on track
int Ki = 0;   //these contants will differ in eee and
int Kd = 0;  //cse fest depending on track

int error = 0, max_error = 4;
int P = 0, I = 0, D = 0, PID_value = 0;
int previous_error = 0, previous_I = 0;

// Timer variables
int counter1, counter2;
int t = 0;
int Time = 0;

// ERROR variables
int WHITE = 102;
int BLACK = 103;

// Initialize Functions
void read_sensor_values();
void ledControl();
void forward();
void reverse();
void stopBot();
void goWhiteTurn();
bool goBlackTurn();
void goPID();


void setup() {
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);

  pinMode(right_forward, OUTPUT);
  pinMode(right_reverse, OUTPUT);
  pinMode(left_forward, OUTPUT);
  pinMode(left_reverse, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(left_LED, OUTPUT);
  pinMode(mid_LED, OUTPUT);
  pinMode(right_LED, OUTPUT);


  Serial.begin(9600);  //setting serial monitor at a default baund rate of 9600
  delay(500);
  Serial.println("Started !!");
  delay(3000);
}

void loop() {
  read_sensor_values();
  if (error == WHITE) goWhiteTurn();
  else if (error == BLACK) goBlackTurn();
  else goPID();
}

void goWhiteTurn() {
  if (previous_error > 0)   ledControl(1,0,0);
  else if (previous_error == 0)   ledControl(0,1,0);
  else   ledControl(0,0,1);
  // stopBot();
  // delay(1000);

  bool goLeft;
  if (previous_error > 0) goLeft = true;
  else goLeft = false;

  counter1 = millis();
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
  // stopBot();
  // delay(500);
  
  while (error > GoodPosition || error < -GoodPosition) {
    analogWrite(ENA, spedl);
    analogWrite(ENB, spedr);
    if (goLeft) sharpLeftTurn();
    else sharpRightTurn();
    read_sensor_values();
  }
  ledControl(0,0,0);
  // stopBot();
  // delay(500);
}

bool goBlackTurn() {
  counter1 = millis();
  ledControl(1,0,1);
  while (true) {
    analogWrite(ENA, spedf);
    analogWrite(ENB, spedf);
    forward();
    delay(1);
    counter2 = millis();
    if (counter2 - counter1 >= DelayBlackError) break;
  }
  read_sensor_values();
  if (error == BLACK) {
    ledControl(1,1,1);
    stopBot();
    delay(15000);
  }
  ledControl(0,0,0);
  return false;
}

void ledControl(int l, int m, int r){
  digitalWrite(left_LED, l);
  digitalWrite(mid_LED, m);
  digitalWrite(right_LED, r);
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
  else{
    int l2 = (sensor[0] == 1) ? 0 : 1;
    int l1 = (sensor[1] == 1) ? 0 : 1;
    int r1 = (sensor[3] == 1) ? 0 : 1;
    int r2 = (sensor[4] == 1) ? 0 : 1;
    error = l2 * 2 + l1 - r1 - r2 * 2;
  }

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
  Serial.print("\t error = ");
  Serial.print(error);
  Serial.print("\n");
}

void goPID() {
  P = error;
  I = I + previous_I;
  D = error - previous_error;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);

  previous_I = I;
  previous_error = error;

  // Calculating the effective motor speed:
  left_motor_speed = initial_motor_speed - PID_value + left_motor_calibrate;
  right_motor_speed = initial_motor_speed + PID_value + right_motor_calibrate;

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
void sharpRightTurn() {
  digitalWrite(right_forward, LOW);
  digitalWrite(right_reverse, HIGH);
  digitalWrite(left_forward, HIGH);
  digitalWrite(left_reverse, LOW);
}
void sharpLeftTurn() {
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