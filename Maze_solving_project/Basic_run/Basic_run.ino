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
int size_of_array = 8;
int B = 0, R = 0, L = 0, F = 0;
char path[50];
int i = 0, j = 0;
// Motor Pins
#define ENA 6
#define right_forward 7
#define right_reverse 8
#define left_forward 9
#define left_reverse 10
#define ENB 11

int left_motor_speed = 0;
int right_motor_speed = 0;

int spedr = 60;
int spedl = 60;
int spedf = 60;


int DelayCheck = 16;

int DelayForth = 70;
int DelayEvery = 70;
int DelayReverse = DelayForth*2;
int DelaySharp = DelayForth*2;
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


  Serial.begin(9600);                     //setting serial monitor at a default baund rate of 9600
  delay(500);
  Serial.println("Started !!");
  delay(1000);
}
/***************************************************************************************  Void Loop  **************************************************************************************/

void read_sensor_values();
void forward();
void sharpLeftTurn();
void sharpRightTurn();
void stop_bot();

void loop(){
  // motor_forward_test();
  // read_sensor_values();
  // int ll =  main();
  //  stop_bot();
  //  delay(5000);
  //  Go_right_way();
  //  stop_bot();
  //  delay(5000);

  read_sensor_values();
  Line_white_error:
  /*******************  SHARP_LEFT_DETECT  *********************/
  if (error == 100) {
    L++;
    Time = 0;
    do {
      forward();
      delay(1);
      Time++;
      read_sensor_values();
      if (error == 103) {
        goto Black;
      }
    } while (Time <= DelayForth);
    Time = 0;
    read_sensor_values();
    if (error == 102) {
      path[i++] = 'L';
      do {
        analogWrite(ENA, spedr); //Right Motor Speed
        analogWrite(ENB, spedl); //Left Motor Speed
        sharpLeftTurn();
        read_sensor_values();
      } while (error < -GoodPosition || error > GoodPosition);
    }
    else {
      analogWrite(ENA, spedr); //Right Motor Speed
      analogWrite(ENB, spedl); //Left Motor Speed
      sharpLeftTurn();
      delay(DelaySharp);
      //      read_sensor_values();
      do {
        analogWrite(ENA, spedr); //Right Motor Speed
        analogWrite(ENB, spedl); //Left Motor Speed
        sharpLeftTurn();
        read_sensor_values();
      } while (error < -GoodPosition || error > GoodPosition);
      // int l= main();
      // if (l == 0) {
      //   path[i++] = 'S';
      // }
      // else {
      //   path[i++] = 'L';
      // }
      //      stop_bot();
    }
  }

  /*****************  SHARP_RIGHT_DETECT  ***************/

  else if (error == 101) {          // Make right turn untill it detects 
    //    stop_bot();
    //    delay(delaytest);
    Time = 0;
    do {
      forward();
      delay(1);
      Time++;
      read_sensor_values();
      if (error == 103) {
        goto Black;
      }
    } while (Time <= DelayForth);
    Time = 0;
    read_sensor_values();
    if (error == 102) {
      path[i++] = 'R';
      do {
        analogWrite(ENA, spedr); //Right Motor Speed
        analogWrite(ENB, spedl); //Left Motor Speed
        sharpRightTurn();
        read_sensor_values();
      } while (error < -GoodPosition || error > GoodPosition);
    }

    // else {
      //      analogWrite(ENA, spedf); //Right Motor Speed
      //      analogWrite(ENB, spedf); //Left Motor Speed
      //      forward();
      //      delay(DelayForth);
      // int l= main();
      // if (l == 0) {
      //   path[i++] = 'R';
      //   return 0;
      // }
      // else {
      //   path[i++] = 'S';
      //   return 1;
      // }
    // }
  }

  /**************** WHITE_DETECT  ******************/

  else if (error == 102) {
    Time = 0;
    do {
      forward();
      delay(1);
      Time++;
      read_sensor_values();
      if (error != 102) {
        goto Line_white_error;
      }
    } while (Time <= DelayWhiteError);
    Time = 0;
    do {
      analogWrite(ENA, spedr); //Right Motor Speed
      analogWrite(ENB, spedl); //Left Motor Speed
      sharpRightTurn();
      read_sensor_values();
    } while (error < -GoodPosition || error > GoodPosition);
    // return 0;
  }

  /********************* BLACK_DETECT  ***********************/

  else if (error == 103) {
  Black:
    analogWrite(ENA, spedf); //Right Motor Speed
    analogWrite(ENB, spedf); // Left Motor Speed
    forward();
    delay(DelayEvery);
    read_sensor_values();
    if (error == 103) {     /**** Finish End Reached, Stop! ****/
      stop_bot();
      delay(5000);
      // return 1;
    }
    else if (error == 102) {       /**** Move Left ****/
      do {
        analogWrite(ENA, spedr); //Right Motor Speed
        analogWrite(ENB, spedl); //Left Motor Speed
        sharpLeftTurn();
        read_sensor_values();
      } while (error < -GoodPosition || error > GoodPosition );
      // int l = main();
      // if (l == 0) {
      //   path[i++] = 'R';
      //   return 0;
      // }
      // else {
      //   path[i++] = 'L';
      //   return 1;
      // }
    }
    else {
      analogWrite(ENA, spedr); //Right Motor Speed
      analogWrite(ENB, spedl); //Left Motor Speed
      sharpLeftTurn();
      delay(DelaySharp);
      // read_sensor_values();
      do {
        analogWrite(ENA, spedr); //Right Motor Speed
        analogWrite(ENB, spedl); //Left Motor Speed
        sharpLeftTurn();
        read_sensor_values();
      } while (error < -GoodPosition || error > GoodPosition );
      // int l = main();
      // if (l == 0) {
      //   int l2=main();
      //   if (l2 == 0) {
      //     path[i++] = 'R';
      //     return 0;
      //   }
      //   else {
      //     path[i++] = 'S';
      //     return 1;
      //   }
      // }
      // else {
      //   path[i++] = 'L';
      //   return 1;
      // }
    }
  }

  else {
    calculate_pid();
    motor_control();
  }
}

bool IR_value_check(int a[], int b[]){
  for (int i = 1;  i < size_of_array-1; i++){
    if(a[i] == 2 || b[i] == 2) continue;
    if(a[i] != b[i]) return false;
  }
  return true;
}

void read_sensor_values() {
  //IR reading..........

  sensor[0] = digitalRead(sensor1);   //  analogRead digitalRead
  sensor[1] = digitalRead(sensor2);
  sensor[2] = digitalRead(sensor3);
  sensor[3] = digitalRead(sensor4);
  sensor[4] = digitalRead(sensor5);
  sensor[5] = digitalRead(sensor6);
  sensor[6] = digitalRead(sensor7);
  sensor[7] = digitalRead(sensor8);

  int white[8] = {0,0,0,0,0,0,0,0};
  int black[8] = {2,1,1,1,1,1,1,2};
  int err5[8] = {2,1,0,0,0,0,0,2};
  int err4[8] = {2,1,1,0,0,0,0,2};
  int err3[8] = {2,0,1,0,0,0,0,2};
  int err2[8] = {2,0,1,1,0,0,0,2};
  int err1[8] = {2,0,0,1,0,0,0,2};
  int err0[8] = {2,0,0,1,1,0,0,2};
  int err_1[8] = {2,0,0,0,1,0,0,2};
  int err_2[8] = {2,0,0,1,1,0,0,2};
  int err_3[8] = {2,0,0,0,0,1,0,2};
  int err_4[8] = {2,0,0,0,0,1,1,2};
  int err_5[8] = {2,0,0,0,0,0,1,2};
  if      (sensor[0] == 1) error = 100; // found left
  else if (sensor[7] == 1) error = 101;  // found right
  else if (IR_value_check(sensor, white)){error = 102;}//found white
  else if (IR_value_check(sensor, black)){error = 103;}//found Black
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

void calculate_pid() {
  P = error;
  I = I + previous_I;
  D = error - previous_error;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);

  previous_I = I;
  previous_error = error;
}

void motor_control() {
  // Calculating the effective motor speed:
  int left_motor_speed = initial_motor_speed - PID_value;
  int right_motor_speed = initial_motor_speed + PID_value;

  // The motor speed should not exceed the max PWM value
  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);

  /*Serial.print(PID_value);
    Serial.print("\t");
    Serial.print(left_motor_speed);
    Serial.print("\t");
    Serial.println(right_motor_speed);*/

  analogWrite(ENA, left_motor_speed); //Left Motor Speed
  analogWrite(ENB, right_motor_speed); //Right Motor Speed

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