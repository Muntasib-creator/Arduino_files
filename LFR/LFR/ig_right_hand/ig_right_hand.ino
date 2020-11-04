#include<NewPing.h>

#define trig_pin_left A3
#define echo_pin_left A4
#define trig_pin_right A0
#define echo_pin_right A1
#define trig_pin_front A5
#define echo_pin_front A6
#define base_dist 8

// IR Sensors
int sensor1 = 13;      // Left most sensor
int sensor2 = 12;      // Left sensor
int sensor3 = 10;      // Middle Left sensor
int sensor4 = 9;       // Middle Right sensor
int sensor5 = 8;       // Right sensor
int sensor6 = 2;       // Right most sensor

// Initial Values of Sensors
int sensor[6] = { 0, 0, 0, 0, 0, 0 };

// Motor Variables
int ENA = 11;
int motorInput1 = 7;
int motorInput2 = 6;
int motorInput3 = 5;
int motorInput4 = 4;
int ENB = 3;

int sped11 = 90;
int sped22 = 90;

int DelayCheck = 90;

int DelayForth = 400;
int DelayEvery = 50;
int DelayReverse = 200;
int DelaySharp = 200;

int t = 0;
int Triangle = 170;

//Initial Speed of Motor
int initial_motor_speed = 160;

// Output Pins for Led
//int ledPin1 = A3;
//int ledPin2 = A4;

// PID Constants
float Kp = 15.00;     // Will be tuned on track
float Ki = 0;         // these contants will differ in
float Kd = 30.00 ;    // 'IGNITION' fest depending on track

// PID Variables
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;

int flag = 0;

int max_distance = 150;

int front_dist;
int left_dist;
int right_dist;
int base_speed = 80;


// Pid for wall...

float proportionate = 0, derivative = 0;
float prev_error = 0, pid = 0;
float Sp = 7;
float Sd = 20;

NewPing sonar_left(trig_pin_left , echo_pin_left, max_distance);
NewPing sonar_right(trig_pin_right, echo_pin_right, max_distance);
NewPing sonar_front(trig_pin_front, echo_pin_front, max_distance);



void setup() {
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);
  pinMode(sensor6, INPUT);

  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
  pinMode(motorInput3, OUTPUT);
  pinMode(motorInput4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  /*pinMode(ledPin1, OUTPUT);
    digitalWrite(ledPin1, LOW);*/


  Serial.begin(9600);                     //setting serial monitor at a default baund rate of 9600
  delay(500);
  Serial.println("Started !!");
  delay(1000);
}

void loop()
{


  read_sensor_values();





  /*************************************************************************************  SHARP_LEFT_DETECT  *******************************************************************************************************************************/





  if (error == 100) {               // Make left turn untill it detects straight path...If straight path exists.....Go forward

    analogWrite(ENA, 90); //Right Motor Speed
    analogWrite(ENB, 90); // Left Motor Speed
    forward();
    delay(DelayEvery);
    read_sensor_values();

    if ( error == 103 ) {
      analogWrite(ENA, 90); //Right Motor Speed
      analogWrite(ENB, 90); // Left Motor Speed
      forward();
      delay(400);
      stop_bot();
      delay(5000);         // Being stopped
    } else if (error == 102) {
      do {
        analogWrite(ENA, sped11); //Right Motor Speed
        analogWrite(ENB, sped22); //Left Motor Speed
        sharpLeftTurn();
        read_sensor_values();
      } while (error < -2 || error > 2 );
      stop_bot();
      delay(50);
    } else {
      analogWrite(ENA, 100); //Right Motor Speed
      analogWrite(ENB, 100); //Left Motor Speed
      forward();
      delay(DelayEvery);
    }
  }





  /**************************************************************************************  SHARP_RIGHT_DETECT  *************************************************************************************** *************************************/





  else if (error == 101) {          // Make right turn untill it detects straight path....

    analogWrite(ENA, 90); //Right Motor Speed
    analogWrite(ENB, 90); // Left Motor Speed
    forward();
    delay(DelayEvery);
    read_sensor_values();

    if ( error == 103 ) {
      analogWrite(ENA, 90); //Right Motor Speed
      analogWrite(ENB, 90); // Left Motor Speed
      forward();
      delay(400);
      stop_bot();
      delay(5000);         // Being stopped
    } else {
      analogWrite(ENA, sped22); //Right Motor Speed
      analogWrite(ENB, sped11); //Left Motor Speed
      sharpRightTurn();
      delay(DelaySharp);
      read_sensor_values();
      do {
        analogWrite(ENA, sped22); //Right Motor Speed
        analogWrite(ENB, sped11); //Left Motor Speed
        sharpRightTurn();
        read_sensor_values();
      } while (error < -2 || error > 2 );
      stop_bot();
      delay(50);
    }

  }






  /************************************************************************************************  WHITE_DETECT  *************************************************************************************************************************/






  else if (error == 102) {
    //This will be a U turn
    //Serial.print("\t");
    //Serial.println("Sharp Left Turn");


    if (previous_error == 5)
    {
      error = 6;
      previous_error = 7;
      calculate_pid();
      do {
        motor_control();
      } while ( error == 102 );
    } else if (previous_error == -5) {
      error = -6;
      previous_error = -7;
      calculate_pid();
      do {
        motor_control();
      } while ( error == 102 );
    } else
    {
      stop_bot();
      delay(50);
      t = 0;
      do {
        analogWrite(ENA, 90); //Right Motor Speed
        analogWrite(ENB, 90); //Left Motor Speed
        forward();
        delay(10);
        t = t + 10;
        read_sensor_values();
        if (error != 102) {
          analogWrite(ENA, 120); //Right Motor Speed
          analogWrite(ENB, 120); //Left Motor Speed
          forward();
          delay(DelayForth);
          break;
        }
      } while (t <= DelayCheck);

      if (error == 102) {      // Make Right turn untill it detects straight path
        analogWrite(ENA, 80); //Right Motor Speed
        analogWrite(ENB, 80); //Left Motor Speed
        reverse();
        delay (100);
        do {
          analogWrite(ENA, sped22); //Right Motor Speed
          analogWrite(ENB, sped11); //Left Motor Speed
          sharpRightTurn();
          read_sensor_values();
        } while (error < -2 || error > 2);
        stop_bot();
        delay(50);
      }
    }
  }







  /*********************************************************************************************  BLACK_DETECT  ****************************************************************************************************************************/







  else if (error == 103) {
    if (flag == 0) {

      analogWrite(ENA, 90); //Right Motor Speed
      analogWrite(ENB, 90); // Left Motor Speed
      forward();
      delay(DelayEvery);
      read_sensor_values();

      if (error == 103) {     /**** Finish End Reached, Stop! ****/
        stop_bot();
        // digitalWrite(ledPin1, HIGH);
        delay(10000);
        flag = 1;
      } else if (error == 102) {       /**** Move Left ****/
        analogWrite(ENA, sped11); //Right Motor Speed
        analogWrite(ENB, sped22); //Left Motor Speed
        sharpRightTurn();
        delay(DelaySharp);
        do {
          //Serial.print("\t");
          //Serial.println("Left Here");
          read_sensor_values();
          analogWrite(ENA, sped11); //Right Motor Speed
          analogWrite(ENB, sped22); //Left Motor Speed
          sharpRightTurn();
        } while (error < -2 || error > 2);
        stop_bot();
      } else {
        analogWrite(ENA, 90); //Right Motor Speed
        analogWrite(ENB, 90); // Left Motor Speed
        forward();
        delay(200);
      }
    }
  }






  /*******************************************************************************************  PID_CONTROL  *******************************************************************************************************************************/





  else {
    calculate_pid();
    motor_control();
  }

}




/********************************************************************************************Void_Loop_End************************************************************************************************************************************/
/********************************************************************************************Sonar_functions************************************************************************************************************************************/






void read_error_sonar_left()
{
  if (left_dist <= 30) error = left_dist - base_dist;
}

void read_error_sonar_right()
{
  if (right_dist <= 30) error = right_dist - base_dist;
}



void read_error_front()
{
  if ( front_dist <= 15 )
  {
    do {
      analogWrite(ENA, sped11); //Right Motor Speed
      analogWrite(ENB, sped22); //Left Motor Speed
      sharpLeftTurn();
      read_sensor_values();
    } while (error != 102 );
    stop_bot();
    delay(50);
    int i = 90; // some value
    do {
      analogWrite(ENA, initial_motor_speed);
      analogWrite(ENB, initial_motor_speed - i );
      forward();
      delay(50);
      i+-= 2 ; // this should be tuned....
    } while ( error == 102 );
  }
}

/********************************************************************************************Sonar_functions_End************************************************************************************************************************************/
/********************************************************************************************Sensor_functiond_start************************************************************************************************************************************/





void read_sensor_values()
{

  //sonar reading.......

  left_dist = sonar_left.ping_cm();
  right_dist = sonar_right.ping_cm();
  front_dist = sonar_front.ping_cm();

  //IR reading..........

  sensor[0] = digitalRead(sensor1);
  sensor[1] = digitalRead(sensor2);
  sensor[2] = digitalRead(sensor3);
  sensor[3] = digitalRead(sensor4);
  sensor[4] = digitalRead(sensor5);
  sensor[5] = digitalRead(sensor6);


  //  Serial.print("left_dist=");
  //  Serial.print("");
  //  Serial.print(left_dist);
  //  Serial.print("\t");
  //  Serial.print("right_dist=");
  //  Serial.print("");
  //  Serial.print(right_dist);
  //  Serial.print("\n\n");


  //  Serial.print(sensor[0]);
  //  Serial.print("\t");
  //  Serial.print(sensor[1]);
  //  Serial.print("\t");
  //  Serial.print(sensor[2]);
  //  Serial.print("\t");
  //  Serial.print(sensor[3]);
  //  Serial.print("\t");
  //  Serial.print(sensor[4]);
  //  Serial.print("\t");
  //  Serial.print(sensor[5]);
  //  Serial.print("\n");


  if (((sensor[1] == 1) && (sensor[4] == 1)) || ((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1))) // Turn Right side or stop
    error = 103;
  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0))
    error = 5;
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0))
    error = 4;
  else if (((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0)) || ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0))) //(sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) might occure this transition fault
    error = 3;
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0))
    error = 2;
  else if (((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0)) || ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 0)))
    error = 1;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 0))
    error = 0;
  else if (((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 0)) || ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 0)))
    error = -1;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 0))
    error = -2;
  else if (((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1) && (sensor[5] == 0)) || ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1))) //(sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1) might occure this transition fault
    error = -3;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1) && (sensor[5] == 1))
    error = -4;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 1))
    error = -5;
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 0)) // Turn robot left side
    error = 100;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1)) // Turn robot right side
    error = 101;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0)) // Make U turn
    error = 102;

}

void calculate_pid()
{

  P = error;
  I = I + previous_I;
  D = error - previous_error;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);

  previous_I = I;
  previous_error = error;
}


// '-' PID  means right motor should slow down...
// "+" PID  means left motor should slow down...


void motor_control()
{
  // Calculating the effective motor speed:

  int left_motor_speed;
  int right_motor_speed;
  if (PID_value < 0) {
    left_motor_speed = initial_motor_speed;
    right_motor_speed = initial_motor_speed + PID_value ;//......right decrease
  }
  if (PID_value > 0) {
    left_motor_speed = initial_motor_speed - PID_value;//......left decrease
    right_motor_speed = initial_motor_speed;
  }
  if (PID_value == 0)
  {
    left_motor_speed = initial_motor_speed;//........both stay same
    right_motor_speed = initial_motor_speed;//........both stay same
  }

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

void forward() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);//.............................................................Right forward rotation
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);//.............................................................Left forward rotation
}
void reverse() {
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}
/*void right(){
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
  }
  void left(){
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
  }*/
void sharpRightTurn() {
  digitalWrite(motorInput1, HIGH);//..............................................................Right reverse rotation
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);//..............................................................Left forward rotation
}
void sharpLeftTurn() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);//..............................................................Right forward rotation
  digitalWrite(motorInput3, HIGH);//..............................................................Left reverse rotation
  digitalWrite(motorInput4, LOW);
}
void stop_bot() {
  //All motors at ease
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
}
