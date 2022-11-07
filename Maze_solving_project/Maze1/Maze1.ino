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
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);
  pinMode(sensor6, INPUT);
  pinMode(sensor7, INPUT);
  pinMode(sensor8, INPUT);

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
/***************************************************************************************  Void Loop  **************************************************************************************/

void loop()
{
  read_sensor_values();
  //  stop_bot();
  //  delay(5000);
  //  Go_right_way();
  //  stop_bot();
  //  delay(5000);
}
/***************************************************************************************  Int Main  **************************************************************************************/

int main() {
  read_sensor_values();
Line_white_error:
  /*************************************************************************************  SHARP_LEFT_DETECT  *******************************************************************************************************************************/

  if (error == 100) {               // Make left turn untill it detects straight path
LeftSharp:
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
      int l= main();
      if (l == 0) {
        path[i++] = 'S';
      }
      else {
        path[i++] = 'L';
      }
      //      stop_bot();
    }
  }

  /**************************************************************************************  SHARP_RIGHT_DETECT  *************************************************************************************** *************************************/

  else if (error == 101) {          // Make right turn untill it detects straight path....If straight path exists.....Go forward
RightSharp:
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

    else {
      //      analogWrite(ENA, spedf); //Right Motor Speed
      //      analogWrite(ENB, spedf); //Left Motor Speed
      //      forward();
      //      delay(DelayForth);
      int l= main();
      if (l == 0) {
        path[i++] = 'R';
        return 0;
      }
      else {
        path[i++] = 'S';
        return 1;
      }
    }
  }

  /********************************************************************  WHITE_DETECT  *************************************************************************************************************************/

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
    return 0;
  }

  /******************************************************************  BLACK_DETECT  ****************************************************************************************************************************/

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
      return 1;
    }
    else if (error == 102) {       /**** Move Left ****/
      do {
        analogWrite(ENA, spedr); //Right Motor Speed
        analogWrite(ENB, spedl); //Left Motor Speed
        sharpLeftTurn();
        read_sensor_values();
      } while (error < -GoodPosition || error > GoodPosition );
      int l = main();
      if (l == 0) {
        path[i++] = 'R';
        return 0;
      }
      else {
        path[i++] = 'L';
        return 1;
      }
    }
    else {
      analogWrite(ENA, spedr); //Right Motor Speed
      analogWrite(ENB, spedl); //Left Motor Speed
      sharpLeftTurn();
      delay(DelaySharp);
      read_sensor_values();
      do {
        analogWrite(ENA, spedr); //Right Motor Speed
        analogWrite(ENB, spedl); //Left Motor Speed
        sharpLeftTurn();
        read_sensor_values();
      } while (error < -GoodPosition || error > GoodPosition );
      int l = main();
      if (l == 0) {
        int l2=main();
        if (l2 == 0) {
          path[i++] = 'R';
          return 0;
        }
        else {
          path[i++] = 'S';
          return 1;
        }
      }
      else {
        path[i++] = 'L';
        return 1;
      }
    }
  }

  /*******************************************************************************************  PID_CONTROL  *******************************************************************************************************************************/

  else {
    calculate_pid();
    motor_control();
  }

}
/*********************************************************************************************  Go Right Way  **************************************************************************************/

void Go_right_way() {
  while (1) {
    read_sensor_values();
    if (error == 100 || error == 101 || error == 102) {
      if (j == i) {
        break;
      }
      analogWrite(ENA, spedf); //Right Motor Speed
      analogWrite(ENB, spedf); // Left Motor Speed
      forward();
      delay(DelayEvery);
      if (path[j++] == 'L') {
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
      }
      else if (path[j++] == 'R') {
        analogWrite(ENA, spedr); //Right Motor Speed
        analogWrite(ENB, spedl); //Left Motor Speed
        sharpRightTurn();
        delay(DelaySharp);
        //      read_sensor_values();
        do {
          analogWrite(ENA, spedr); //Right Motor Speed
          analogWrite(ENB, spedl); //Left Motor Speed
          sharpRightTurn();
          read_sensor_values();
        } while (error < -GoodPosition || error > GoodPosition);
      }
      else {
        j++; forward();
      }

    }
    else {
      calculate_pid();
      motor_control();
    }
  }
}
/***************************************************************************************  Read Sensor Values  **************************************************************************************/

void read_sensor_values() {
  Serial.println(3011);
  
}
/***************************************************************************************  Calculate PID  **************************************************************************************/

void calculate_pid() {
  P = error;
  I = I + previous_I;
  D = error - previous_error;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);

  previous_I = I;
  previous_error = error;
}
/***************************************************************************************  Motor Control  **************************************************************************************/

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
void sharpLeftTurn() {
  digitalWrite(motorInput1, LOW);//Right reverse rotation
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);//Left forward rotation
}
void sharpRightTurn() {
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
