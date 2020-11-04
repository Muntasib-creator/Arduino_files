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
char path[50];
//char path[30]="SBLLSLBRLLLSRSBLBLLLRBLLRS";
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
int initial_motor_speed = 60;

int DelayCheck = 16;

int DelayForth = 9;
int DelayEvery = 100;
int DelayReverse = 200;
int DelaySharp = 400;
int delaytest = 3000;
int DelayStop = 4000;
int DelayWhiteError = 4;
int GoodPosition = 2;

int t = 0;
int Time = 0;



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
//  i=21+5;
//  Go_right_way();
  read_sensor_values();
Line_white_error:
  /*************************************************************************************  SHARP_LEFT_DETECT  *******************************************************************************************************************************/

  if (error == 100) {               // Make left turn untill it detects straight path
LeftSharp:
    L++;
    Time = 0;
    do {
      analogWrite(ENA, spedf); //Right Motor Speed
      analogWrite(ENB, spedf); //Left Motor Speed
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
      Serial.print("i= ");
      Serial.println(i);
      for (j = 0; j < i; j++) {
        Serial.print(path[j]);
      }
      Serial.print("\n\n\n\n\n\n\n\nascheeeeeeee\n\n\n\n\n\n\n\n");
      do {
        analogWrite(ENA, spedr); //Right Motor Speed
        analogWrite(ENB, spedl); //Left Motor Speed
        sharpLeftTurn();
        read_sensor_values();
      } while (error < -GoodPosition || error > GoodPosition);
    }
    else {
      path[i++] = 'L';
      Serial.print("i= ");
      Serial.println(i);
      for (j = 0; j < i; j++) {
        Serial.print(path[j]);
      }
      Serial.print("\n\n\n\n\n\n\n\nascheeeeeeee\n\n\n\n\n\n\n\n");
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
  }

  /**************************************************************************************  SHARP_RIGHT_DETECT  *************************************************************************************** *************************************/

  else if (error == 101) {          // Make right turn untill it detects straight path....If straight path exists.....Go forward
RightSharp:
    //    stop_bot();
    //    delay(delaytest);
    Time = 0;
    do {
      analogWrite(ENA, spedf); //Right Motor Speed
      analogWrite(ENB, spedf); //Left Motor Speed
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
      Serial.print("i= ");
      Serial.println(i);
      for (j = 0; j < i; j++) {
        Serial.print(path[j]);
      }
      Serial.print("\n\n\n\n\n\n\n\nascheeeeeeee\n\n\n\n\n\n\n\n");
      do {
        analogWrite(ENA, spedr); //Right Motor Speed
        analogWrite(ENB, spedl); //Left Motor Speed
        sharpRightTurn();
        read_sensor_values();
      } while (error < -GoodPosition || error > GoodPosition);
    }

    else {
      path[i++] = 'S';
      Serial.print("i= ");
      Serial.println(i);
      for (j = 0; j < i; j++) {
        Serial.print(path[j]);
      }
      Serial.print("\n\n\n\n\n\n\n\nascheeeeeeee\n\n\n\n\n\n\n\n");
      analogWrite(ENA, spedf); //Right Motor Speed
      analogWrite(ENB, spedf); //Left Motor Speed
      forward();
      delay(DelayEvery);

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
        Serial.print("\n\n\n\n\nwhite error\n\n\n\n\n");
        goto Line_white_error;
      }
    } while (Time <= DelayWhiteError);
    path[i++] = 'B';
    Serial.print("i= ");
    Serial.println(i);
    for (j = 0; j < i; j++) {
      Serial.print(path[j]);
    }
    Serial.print("\n\n\n\n\n\n\n\nascheeeeeeee\n\n\n\n\n\n\n\n");
    Time = 0;
    do {
      analogWrite(ENA, spedr - 15); //Right Motor Speed
      analogWrite(ENB, spedl + 15); //Left Motor Speed
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
    delay(DelayEvery + 30);
    read_sensor_values();
    if (error == 103) {     /**** Finish End Reached, Stop! ****/
      Go_right_way();
    }
    else if (error == 102) {       /**** Move Left ****/
      path[i++] = 'L';
      Serial.print("i= ");
      Serial.println(i);
      for (j = 0; j < i; j++) {
        Serial.print(path[j]);
      }
      Serial.print("\n\n\n\n\n\n\n\nascheeeeeeee\n\n\n\n\n\n\n\n");
      do {
        analogWrite(ENA, spedr); //Right Motor Speed
        analogWrite(ENB, spedl); //Left Motor Speed
        sharpLeftTurn();
        read_sensor_values();
      } while (error < -GoodPosition || error > GoodPosition );
    }
    else {
      path[i++] = 'L';
      Serial.print("i= ");
      Serial.println(i);
      for (j = 0; j < i; j++) {
        Serial.print(path[j]);
      }
      Serial.print("\n\n\n\n\n\n\n\nascheeeeeeee\n\n\n\n\n\n\n\n");
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
  stop_bot();
  delay(DelayStop*2);
  GoodPosition=4;
  Serial.print("i= ");
  Serial.println(i);
  for (j = 0; j < i; j++) {
    Serial.print(path[j]);
  }
  Serial.print("\nasche 1\n");
  int n = i;
  while (true) {
    n = i;
    flag = 0;
    for (i = 0; i < n; i++) {
      if (path[i] == 'B') {
        flag = 1;
        if (path[i - 1] == 'L' && path[i + 1] == 'L') {
          path[i - 1] = 'S';
          path[i] = 'X';
          path[i + 1] = 'X';
        }
        else if (path[i - 1] == 'L' && path[i + 1] == 'R') {
          path[i - 1] = 'B';
          path[i] = 'X';
          path[i + 1] = 'X';
        }
        else if (path[i - 1] == 'L' && path[i + 1] == 'S') {
          path[i - 1] = 'R';
          path[i] = 'X';
          path[i + 1] = 'X';
        }
        else if (path[i - 1] == 'R' && path[i + 1] == 'L') {
          path[i - 1] = 'B';
          path[i] = 'X';
          path[i + 1] = 'X';
        }
        else if (path[i - 1] == 'S' && path[i + 1] == 'L') {
          path[i - 1] = 'R';
          path[i] = 'X';
          path[i + 1] = 'X';
        }
        else if (path[i - 1] == 'S' && path[i + 1] == 'S') {
          path[i - 1] = 'B';
          path[i] = 'X';
          path[i + 1] = 'X';
        }
      }
    }
    if (flag == 0) {
      break;
    }
    char temp[100];
    for (i = 0, j = 0; i < n; i++) {
      if (path[i] == 'X') {
        continue;
      }
      else {
        temp[j++] = path[i];
      }
    }
    for (i = 0; i < j; i++) {
      path[i] = temp[i];
    }
  }
  Serial.print("i= ");
  Serial.println(i);
  for (j = 0; j < i; j++) {
    Serial.print(path[j]);
  }
  Serial.print("\nasche 2\n");
  //  delay(1000000);

  j = 0;
  //  stop_bot();
  //  delay(DelayStop);

Right_Way_Loop:
  Serial.print("\n\n\n Check1 \n\n\n");
  read_sensor_values();
  Serial.print("\n\n\n Check2 \n\n\n");
  if (error == 100 || error == 101 || error == 103) {
    Serial.print("\n\n\n Check3 \n\n\n");
    if (j == i) {
      Serial.print("\n\n\n Check STOP!!!!!!!!!!!!!!!! \n\n\n");
      stop_bot();
      delay(DelayStop * 2);
    }
    analogWrite(ENA, spedf); //Right Motor Speed
    analogWrite(ENB, spedf); // Left Motor Speed
    forward();
    delay(DelayEvery);
    if (path[j] == 'L') {
      Serial.print("\n\n\n Check Left \n\n\n j & path[j]= ");
      Serial.print(j);
      Serial.print('\t');
      Serial.println(path[j]);
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
      j++;
    }
    else if (path[j] == 'R') {
      Serial.print("\n\n\nCheck Right \n\n\n j & path[j]= ");
      Serial.print(j);
      Serial.print('\t');
      Serial.println(path[j]);
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
      j++;
    }
    else {
      Serial.print("\n\n\n Check Forward \n\n\n j & path[j]= ");
      Serial.print(j);
      Serial.print('\t');
      Serial.println(path[j]);
      j++; forward();
    }

  }
  else {
    Serial.print("\n\n\n Check PID\n\n\n ");
    calculate_pid();
    motor_control();
  }
  goto Right_Way_Loop;
}
/***************************************************************************************  Read Sensor Values  **************************************************************************************/

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

  //  Serial.print("left_dist=");
  //  Serial.print("");
  //  Serial.print(left_dist);
  //  Serial.print("\t");
  //  Serial.print("right_dist=");
  //  Serial.print("");
  //  Serial.print(right_dist);
  //  Serial.print("\n\n");

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

  //  if ((sensor[0] == 1))
  //    error = 100;              // found left
  //  else if ((sensor[7] == 1))
  //    error = 101;              // found right
  //  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0)) // Make U turn
  //    error = 102;              //found white
  //  if ((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1) && (sensor[6] == 1)) // stop
  //    error = 103;              // found black
  //  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
  //    error = 5;
  //  else if ((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
  //    error = 4;
  //  else if ((sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
  //    error = 3;
  //  else if ((sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
  //    error = 2;
  //  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
  //    error = 1;
  //  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 0) && (sensor[6] == 0))
  //    error = 0;
  //  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1) && (sensor[5] == 0) && (sensor[6] == 0))
  //    error = -1;
  //  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1) && (sensor[5] == 1) && (sensor[6] == 0))
  //    error = -2;
  //  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 1) && (sensor[6] == 0))
  //    error = -3;
  //  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 1) && (sensor[6] == 1))
  //    error = -4;
  //  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 1))
  //    error = -5;
  /****************************************  Black Surface  *************************************************/
  if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0)) // stop
    error = 103;              // found black
  else  if ((sensor[0] == 0))
    error = 100;              // found left
  else if ((sensor[7] == 0))
    error = 101;              // found right
  else if ((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1) && (sensor[6] == 1)) // Make U turn
    error = 102;              //found white
  else if ((sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1) && (sensor[6] == 1))
    error = 5;
  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1) && (sensor[6] == 1))
    error = 4;
  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1) && (sensor[6] == 1))
    error = 3;
  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1) && (sensor[5] == 1) && (sensor[6] == 1))
    error = 2;
  else if ((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 1) && (sensor[5] == 1) && (sensor[6] == 1))
    error = 1;
  else if ((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 1) && (sensor[6] == 1))
    error = 1;
  else if ((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 1) && (sensor[6] == 1))
    error = -1;
  else if ((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 1))
    error = -2;
  else if ((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 0) && (sensor[6] == 1))
    error = -3;
  else if ((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 0) && (sensor[6] == 0))
    error = -4;
  else if ((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1) && (sensor[6] == 0))
    error = -5;

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
