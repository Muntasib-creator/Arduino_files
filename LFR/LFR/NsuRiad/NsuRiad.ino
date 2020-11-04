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
int surfaceflag = 0;

// Motor Pins
#define ENA 6
#define motorInput1 7
#define motorInput2 8
#define motorInput3 9
#define motorInput4 10
#define ENB 11

int left_motor_speed = 0;
int right_motor_speed = 0;

int spedr = 100;
int spedl = 100;
int spedf = 100;


int DelayCheck = 15;

int DelayForth = 100;
int DelayEvery = 100;
int DelayReverse = 200;
int DelaySharp = 400;
int delaytest = 3000;
int GoodPosition = 2;

int t = 0;

int initial_motor_speed = 100;

// Output Pins for Led
//int ledPin1 = A3;
//int ledPin2 = A4;

// PID Constants
float Kp = 20.00; // Will be tuned on track
float Ki = 0; //these contants will differ in eee and
float Kd = 25.00 ;//cse fest depending on track

// PID Variables
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;

int flag = 0;

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
  read_sensor_values(surfaceflag);

  /*************************************************************************************  SHARP_LEFT_DETECT  *******************************************************************************************************************************/

  if (error == 100) {               // Make left turn untill it detects straight path

    //    stop_bot();
    //    delay(delaytest);
    analogWrite(ENA, spedf); //Right Motor Speed
    analogWrite(ENB, spedf); // Left Motor Speed
    forward();
    delay(DelayEvery);
    read_sensor_values(surfaceflag);
    if ( error == 103 ) {
      //stop_bot();
      //delay(5000);
    } else {
      analogWrite(ENA, spedr); //Right Motor Speed
      analogWrite(ENB, spedl); //Left Motor Speed
      sharpLeftTurn();
      delay(DelaySharp);
      read_sensor_values(surfaceflag);
      do {
        analogWrite(ENA, spedr); //Right Motor Speed
        analogWrite(ENB, spedl); //Left Motor Speed
        sharpLeftTurn();
        read_sensor_values(surfaceflag);
      } while (error < -GoodPosition || error > GoodPosition);
      //      stop_bot();
    }
  }

  /**************************************************************************************  SHARP_RIGHT_DETECT  *************************************************************************************** *************************************/

  else if (error == 101) {          // Make right turn untill it detects straight path....If straight path exists.....Go forward

    analogWrite(ENA, spedf); //Right Motor Speed
    analogWrite(ENB, spedf); // Left Motor Speed
    forward();
    delay(DelayEvery);
    read_sensor_values(surfaceflag);

    if ( error == 103 ) {
      //stop_bot();
      //delay(5000);
    } else if (error == 102) {
      do {
        analogWrite(ENA, spedr); //Right Motor Speed
        analogWrite(ENB, spedl); //Left Motor Speed
        sharpRightTurn();
        read_sensor_values(surfaceflag);
      } while (error < -GoodPosition || error > GoodPosition );

    } else {
      analogWrite(ENA, spedf); //Right Motor Speed
      analogWrite(ENB, spedf); //Left Motor Speed
      forward();
      delay(DelayForth);
    }
  }

  /********************************************************************  WHITE_DETECT  *************************************************************************************************************************/

  else if (error == 102) {
    //    Serial.println("IM in 102");
    //    delay(6000);
    t = 0;
    do {
      analogWrite(ENA, spedf); //Right Motor Speed
      analogWrite(ENB, spedf); //Left Motor Speed

      //      analogWrite(ENA, left_motor_speed); //Left Motor Speed
      //      analogWrite(ENB, right_motor_speed); //Right Motor Speed

      forward();
      delayMicroseconds(1000);
      t = t + 1;
      //      Serial.print("t=");
      //      Serial.println(t);
      read_sensor_values(surfaceflag);
      if (t == DelayCheck) {
        //        Serial.println("IM in condition");
        //        delay(6000);
        //        Serial.println("IM out of loop");
        //        delay(6000);

        break;
      }
      //      Serial.println("IM out of condition");
    } while (error == 102);

    //    Serial.println("IM out of do while 102");
    //    delay(6000);

    if (error == 102) {
      analogWrite(ENA, 100); //Right Motor Speed
      analogWrite(ENB, 100); //Left Motor Speed
      stop_bot();
      delay(50);
      reverse();
      delay(DelayReverse);
      do {
        analogWrite(ENA, 100); //Right Motor Speed
        analogWrite(ENB, 100); //Left Motor Speed

        sharpLeftTurn();

        read_sensor_values(surfaceflag);
      } while (error < -GoodPosition || error > GoodPosition );
    }
  }

  /******************************************************************  BLACK_DETECT  ****************************************************************************************************************************/

  else if (error == 103) {

    analogWrite(ENA, spedf); //Right Motor Speed
    analogWrite(ENB, spedf); // Left Motor Speed


    forward();
    delay(DelayEvery);
    read_sensor_values(surfaceflag);
    if (error == 103) {     /**** Finish End Reached, Stop! ****/
      stop_bot();
      // digitalWrite(ledPin1, HIGH);
      // delay(5000);
    } else if (error == 102) {       /**** Move Left ****/
      analogWrite(ENA, spedr); //Right Motor Speed
      analogWrite(ENB, spedl); //Left Motor Speed
      sharpLeftTurn();
      delay(DelaySharp);
      do {
        //Serial.print("\t");
        //Serial.println("Left Here");

        analogWrite(ENA, spedr); //Right Motor Speed
        analogWrite(ENB, spedl); //Left Motor Speed
        sharpLeftTurn();
        read_sensor_values(surfaceflag);
      } while (error < -GoodPosition || error > GoodPosition );

    } else {
      analogWrite(ENA, spedf); //Right Motor Speed
      analogWrite(ENB, spedf); // Left Motor Speed
      forward();
      delay(DelayForth);

    }

  }

  /*******************************************************************************************  PID_CONTROL  *******************************************************************************************************************************/

  else if(error > -5 && error <5) {
    calculate_pid();
    motor_control();
  }
  else 
       surfaceflag = checksurface();
}

int checksurface()
{

int i = 0;
int total =0;
while(i<5)
{
  sensor[0] = digitalRead(sensor1);   //  analogRead digitalRead
  sensor[1] = digitalRead(sensor2);
  sensor[2] = digitalRead(sensor3);
  sensor[3] = digitalRead(sensor4);
  sensor[4] = digitalRead(sensor5);
  sensor[5] = digitalRead(sensor6);
  sensor[6] = digitalRead(sensor7);
  sensor[7] = digitalRead(sensor8);
  total+= (sensor[0]+sensor[1] + sensor[2] +  sensor[3] +  sensor[4] + sensor[5] +  sensor[6] +  sensor[7] );
   i++;
}
  if(total>=20)
     return 1;
   else return 0;

}
void read_sensor_values(int surfaceflag)
{
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

  if (((sensor[0] == 1)&& !surfaceflag)||((sensor[0] == 0)&& surfaceflag))
    error = 100;              // found left
  else if (((sensor[7] == 1)&&!surfaceflag)||((sensor[7] == 0)&& surfaceflag))
    error = 101;              // found right
  else if (((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0) && !surfaceflag)||((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1)&& (sensor[5] == 1) && (sensor[6] == 1) &&surfaceflag)) // Make U turn
    error = 102;              //found white
  else if (((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1) && (sensor[6] == 1)&&!surfaceflag)||((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0)&& (sensor[5] == 0) && (sensor[6] == 0) &&surfaceflag))// stop
    error = 103;              // found black
  else if (((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0)&&!surfaceflag)||((sensor[1]==0)&&(sensor[2]==1)&&(sensor[3]==1)&&(sensor[4]==1)&&(sensor[5]==1)&&(sensor[6]==1)&&surfaceflag))
    error = 5;
  else if (((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0)&&!surfaceflag)||((sensor[1]==0)&&(sensor[2]==0)&&(sensor[3]==1)&&(sensor[4]==1)&&(sensor[5]==1)&&(sensor[6]==1)&&surfaceflag))
    error = 4;
  else if (((sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0)&&!surfaceflag)||((sensor[1]==1)&&(sensor[2]==0)&&(sensor[3]==1)&&(sensor[4]==1)&&(sensor[5]==1)&&(sensor[6]==1)&&surfaceflag))
    error = 3;
  else if (((sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0)&&!surfaceflag)||((sensor[1]==1)&&(sensor[2]==1)&&(sensor[3]==1)&&(sensor[4]==1)&&(sensor[5]==1)&&(sensor[6]==1)&&surfaceflag))
    error = 2;
  else if (((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0) && !surfaceflag) || ((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 1) && (sensor[5] == 1) && (sensor[6] == 1) && surfaceflag))
    error = 1;
  else if (((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 0) && (sensor[6] == 0)&&!surfaceflag)||((sensor[1]==1)&&(sensor[2]==1)&&(sensor[3]==0)&&(sensor[4]==0)&&(sensor[5]==1)&&(sensor[6]==1)&&surfaceflag))
    error = 0;
  else if (((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1) && (sensor[5] == 0) && (sensor[6] == 0)&&!surfaceflag)||((sensor[1]==1)&&(sensor[2]==1)&&(sensor[3]==1)&&(sensor[4]==0)&&(sensor[5]==1)&&(sensor[6]==1)&&surfaceflag))
    error = -1;
  else if (((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1) && (sensor[5] == 1) && (sensor[6] == 0)&&!surfaceflag)||((sensor[1]==1)&&(sensor[2]==1)&&(sensor[3]==1)&&(sensor[4]==1)&&(sensor[5]==1)&&(sensor[6]==1)&&surfaceflag))
    error = -2;
  else if (((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 1) && (sensor[6] == 0)&&!surfaceflag)||((sensor[1]==1)&&(sensor[2]==1)&&(sensor[3]==1)&&(sensor[4]==1)&&(sensor[5]==0)&&(sensor[6]==1)&&surfaceflag))
    error = -3;
  else if (((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 1) && (sensor[6] == 1)&&!surfaceflag)||((sensor[1]==1)&&(sensor[2]==1)&&(sensor[3]==1)&&(sensor[4]==1)&&(sensor[5]==0)&&(sensor[6]==0)&&surfaceflag))
    error = -4;
  else if (((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 1)&&!surfaceflag)||((sensor[1]==1)&&(sensor[2]==1)&&(sensor[3]==1)&&(sensor[4]==1)&&(sensor[5]==1)&&(sensor[6]==0)&&surfaceflag))
    error = -5;

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
