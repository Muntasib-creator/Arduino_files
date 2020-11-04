#include<NewPing.h>

#define trig_pin_left A3
#define echo_pin_left A4
#define trig_pin_right A0
#define echo_pin_right A1
#define trig_pin_front A5
#define echo_pin_front A6

// IR Sensors
int sensor1 = 13;      // Left most sensor
int sensor2 = 12;      // Left sensor
int sensor3 = 10;      // Middle Left sensor
int sensor4 = 9;       // Middle Right sensor
int sensor5 = 8;       // Right sensor
int sensor6 = 2;       // Right most sensor

// Initial Values of Sensors
int sensor[6] = {0, 0, 0, 0, 0,0};

// Motor Variables
int ENA = 3;
int motorInput1 = 4;
int motorInput2 = 5;
int motorInput3 = 6;
int motorInput4 = 7;
int ENB = 11;

int sped11 = 75;
int sped22 = 75;

int sped11rev = 0;
int sped22rev = 0;

int DelayCheck = 90;


int DelayForth = 400;
int DelayEvery = 150;
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
float Kp = 11.00; // Will be tuned on track
float Ki = 0; //these contants will differ in eee and
float Kd = 20.00 ;//cse fest depending on track

// PID Variables
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;

int flag = 0;


int max_distance = 150;

//int cur_dist;
//int base_dist = 8;
//int base_speed = 50;

int left_dist;
int right_dist;
int base_speed = 80;
int front_dist;

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

  pinMode(trig_pin_left, INPUT);
  pinMode(echo_pin_left, INPUT);
  pinMode(trig_pin_right, INPUT);
  pinMode(echo_pin_right, INPUT);

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





  if (error == 100) {               // Make left turn untill it detects straight path
    t = 0;
    do {
      analogWrite(ENA, 80); //Right Motor Speed
      analogWrite(ENB, 80); // Left Motor Speed
      forward();
      delay(10);
      t = t + 10;
      read_sensor_values();
      //        if (error == 150) {
      //          do {
      //            analogWrite(ENA, sped11); //Right Motor Speed
      //            analogWrite(ENB, sped22); //Left Motor Speed
      //            sharpLeftTurn();
      //            read_sensor_values();
      //          } while (error < -2 || error > 2);
      //          break;
      //        }
    } while ( t <= DelayEvery );

    read_sensor_values();
    if ( error == 103 ) {
      stop_bot();
      delay(5000);
    } else {
      analogWrite(ENA, sped22); //Right Motor Speed
      analogWrite(ENB, sped11); //Left Motor Speed
      sharpLeftTurn();
      delay(DelaySharp);
      read_sensor_values();
      do {
        analogWrite(ENA, sped22); //Right Motor Speed
        analogWrite(ENB, sped11); //Left Motor Speed
        sharpLeftTurn();
        read_sensor_values();
      } while (error < -2 || error > 2 );
      stop_bot();
    }
  }





  /**************************************************************************************  SHARP_RIGHT_DETECT  *************************************************************************************** *************************************/





  else if (error == 101) {          // Make right turn untill it detects straight path....If straight path exists.....Go forward
    t = 0;
    do {
      analogWrite(ENA, 80); //Right Motor Speed
      analogWrite(ENB, 80); // Left Motor Speed
      forward();
      delay(10);
      t = t + 10;
      read_sensor_values();
      //        if (error == 150) {
      //          do {
      //            analogWrite(ENA, sped11); //Right Motor Speed
      //            analogWrite(ENB, sped22); //Left Motor Speed
      //            sharpLeftTurn();
      //            read_sensor_values();
      //          } while (error < -2 || error > 2);
      //          break;
      //        }
    } while ( t <= DelayEvery );

    read_sensor_values();
    if ( error == 103 ) {
      stop_bot();
      delay(5000);
    } else if (error == 102) {
      do {
        analogWrite(ENA, sped11); //Right Motor Speed
        analogWrite(ENB, sped22); //Left Motor Speed
        sharpRightTurn();
        read_sensor_values();
      } while (error < -2 || error > 2 );
      stop_bot();
    } else {
      analogWrite(ENA, 100); //Right Motor Speed
      analogWrite(ENB, 100); //Left Motor Speed
      forward();
      delay(70);
    }
  }






  /************************************************************************************************  WHITE_DETECT  *************************************************************************************************************************/






  else if (error == 102) {
    //This will be a U turn
    //Serial.print("\t");
    //Serial.println("Sharp Left Turn");
    stop_bot();
    t = 0;
    do {
      analogWrite(ENA, 90); //Right Motor Speed
      analogWrite(ENB, 90); //Left Motor Speed
      forward();
      delay(10);
      t = t + 10;
      read_sensor_values();
      if (error != 102) {
        analogWrite(ENA, 100); //Right Motor Speed
        analogWrite(ENB, 100); //Left Motor Speed
        forward();
        delay(DelayForth);
        break;
      }
    } while (t <= DelayCheck);

    if (error == 102) {      // Make left turn untill it detects straight path
      //Serial.print("\t");
      //Serial.println("Right");
      analogWrite(ENA, 80); //Right Motor Speed
      analogWrite(ENB, 80); //Left Motor Speed
      reverse();
      delay (100);
      do {
        analogWrite(ENA, sped22); //Right Motor Speed
        analogWrite(ENB, sped11); //Left Motor Speed
        sharpLeftTurn();
        read_sensor_values();
      } while (error < -2 || error > 2);
      stop_bot();
    }
  }







  /*********************************************************************************************  BLACK_DETECT  ****************************************************************************************************************************/







  else if (error == 103) {
    if (flag == 0) {
      t = 0;
      do {
        analogWrite(ENA, 80); //Right Motor Speed
        analogWrite(ENB, 80); // Left Motor Speed
        forward();
        delay(10);
        t = t + 10;
        read_sensor_values();
        //          if (error == 150) {
        //            do {
        //              analogWrite(ENA, sped11); //Right Motor Speed
        //              analogWrite(ENB, sped22); //Left Motor Speed
        //              sharpLeftTurn();
        //              read_sensor_values();
        //            } while (error < -2 || error > 2);
        //            break;
        //          }
      } while ( t <= DelayEvery );
      read_sensor_values();
      if (error == 103) {     /**** Finish End Reached, Stop! ****/
        stop_bot();
        // digitalWrite(ledPin1, HIGH);
        delay(10000);
        flag = 1;
      } else if (error == 102) {       /**** Move Left ****/
        analogWrite(ENA, sped11); //Right Motor Speed
        analogWrite(ENB, sped22); //Left Motor Speed
        sharpLeftTurn();
        delay(DelaySharp);
        do {
          //Serial.print("\t");
          //Serial.println("Left Here");
          read_sensor_values();
          analogWrite(ENA, sped11); //Right Motor Speed
          analogWrite(ENB, sped22); //Left Motor Speed
          sharpLeftTurn();
        } while (error < -2 || error > 2);
        stop_bot();
      } else {
        analogWrite(ENA, 80); //Right Motor Speed
        analogWrite(ENB, 80); // Left Motor Speed
        forward();
        delay(250);
        stop_bot();
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



//
//void read_sonar_values()
//{
//  left_dist = sonar_left.ping_cm();
//  right_dist = sonar_right.ping_cm();
//
//  
//
//}
//
//void read_sonar_left()
//{
//
//
//  if (left_dist >= 23 && left_dist <= 40)
//    error = -7;
//  else if (left_dist == 22)
//    error = -6;
//  else if (left_dist == 21)
//    error = -5;
//  else if (left_dist == 20)
//    error = -4;
//  else if (left_dist == 19)
//    error = -3;
//  else if (left_dist == 18)
//    error = -2;
//  else if (left_dist == 17)
//    error = -1;
//
//
//  else   if (left_dist == 16)
//    error = 0;
//
//  else if (left_dist == 15)
//    error = 1;
//  else if (left_dist == 14)
//    error = 2;
//  else if (left_dist == 13)
//    error = 3;
//  else if (left_dist == 12)
//    error = 4;
//  else if (left_dist == 11)
//    error = 5;
//  else if (left_dist == 10)
//    error = 6;
//  else if (left_dist <= 9 && left_dist >= 0)
//    error = 7;
//
//}
//
//void read_sonar_right()
//{
//  left_dist = sonar_left.ping_cm();
//  right_dist = sonar_right.ping_cm();
//
//  if (right_dist <= 9 && right_dist >= 0)
//    error = -7;
//  else if (right_dist == 10)
//    error = -6;
//  else if (right_dist == 11)
//    error = -5;
//  else if (right_dist == 12)
//    error = -4;
//  else if (right_dist == 13)
//    error = -3;
//  else if (right_dist == 14)
//    error = -2;
//  else if (right_dist == 15)
//    error = -1;
//
//
//  else   if (right_dist == 16)
//    error = 0;
//
//  else if (right_dist == 17)
//    error = 1;
//  else if (right_dist == 18)
//    error = 2;
//  else if (right_dist == 19)
//    error = 3;
//  else if (right_dist == 20)
//    error = 4;
//  else if (right_dist == 21)
//    error = 5;
//  else if (right_dist == 22)
//    error = 6;
//  else if (right_dist >= 23 && right_dist <= 40)
//    error = 7;
//
//}
//
//
//
//void wall_pid()
//{
//  proportionate = error ;
//  derivative = error - prev_error ;
//  pid = error * Sp + derivative * Sd;
//  prev_error = error;
//
//  int left_motor_speed = base_speed + pid ;
//  int right_motor_speed = base_speed - pid ;
//
//  left_motor_speed = constrain(left_motor_speed, 0, 255);
//  right_motor_speed = constrain(right_motor_speed, 0, 255);
//
//  analogWrite(ENA, right_motor_speed); //Left Motor Speed
//  analogWrite(ENB, left_motor_speed); //Right Motor Speed
//  forward();
//}




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


  Serial.print("left_dist=");
  Serial.print("");
  Serial.print(left_dist);
  Serial.print("\t");
  Serial.print("right_dist=");
  Serial.print("");
  Serial.print(right_dist);
  Serial.print("\n\n");
  

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
  Serial.print("\n");

  if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0))
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
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1)) // Turn left side or stop
    error = 103;
//  else if (((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 0)) || ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 1))) // hexagon turn left
//    error = 104;
//  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 0)) // Triangle go right
//    error = 105;
//  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 1)) // Triangle go left
//    error = 106;


  //hexagon_errors

//  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 1))
//    error = 150;
//  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 1))
//    error = 150;
//  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1))
//    error = 150;
//  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1))
//    error = 150;
//  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 1) && (sensor[5] == 1))
//    error = 150;
//  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1) && (sensor[5] == 1))
//    error = 150;
//  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1))
//    error = 150;
//  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 1))
//    error = 150;
//  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1) && (sensor[5] == 1))
//    error = 150;
//  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 1))
//    error = 150;


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

void motor_control()
{
  // Calculating the effective motor speed:
  int left_motor_speed = initial_motor_speed + PID_value;
  int right_motor_speed = initial_motor_speed - PID_value ;

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
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);//Right forward rotation
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);//Left forward rotation
}
void reverse() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}
void sharpRightTurn() {
  digitalWrite(motorInput1, LOW);//Right reverse rotation
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);//Left forward rotation
}
void sharpLeftTurn() {
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);//Right forward rotation
  digitalWrite(motorInput3, LOW);//Left reverse rotation
  digitalWrite(motorInput4, HIGH);
}
void stop_bot() {
  //All motors at ease
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
}
