// IR Sensor Pins
#define sensor1 A3      // Left most sensor
#define sensor2 A2    // Left sensor
#define sensor3 A1     // Middle Left sensor
#define sensor4 A0      // Middle Right sensor
#define sensor5 2      // Right sensor
#define sensor6 3      // Right most sensor
#define sensor7 4
#define sensor8 5

// Motor Pins
#define ENA 6
#define motorInput1 7
#define motorInput2 8
#define motorInput3 9
#define motorInput4 10
#define ENB 11

// Output Pins for Led

#define delayStraight 200
#define delay_reverse 200
#define delayTurn 200
#define delay_wall 400
#define good_position 2
#define turning_speed 90
#define forward_speed 90
#define max_distance 100
#define tune 25


//Speeds of Motor online and between wall
#define initial_motor_speed 150
#define wall_base_speed 100

//pid constants
#define Kp 5
#define Kd 0
#define Ki 0


// Initial Values of Sensors
int sensor[8] = { 0, 0, 0, 0, 0, 0, 0, 0};

int flag = 0;

void setup()
{
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


  Serial.begin(9600);                     //setting serial monitor at a default baund rate of 9600
  delay(500);
  Serial.println("Started !!");
  delay(1000);
}

void loop()
{
  if (flag == 0) {
    //Reading the track...
    while (1) {
      
      follow_line();
      int found_left = 0;
      int found_right = 0;
      int found_straight = 0;

      int check = read_sensors();

      if (check == 100)
        found_left = 1;
      if (check == 101)
        found_right = 1;

      go_straight();
      delay(delayStraight);
      check = read_sensors();

      if (check >= -2 && check <= 2)
        found_straight = 1 ;
      if (check == 103)
        break;

      char Direction = select_turn(found_left, found_right, found_straight);
      follow(Direction);
    }
  }
  stop_bot();
  flag = 1;
}


void follow_line()
{
  int I = 0;
  int previous_Position = 0;
  while (1)
  {
    int Position = read_sensors();

    if (Position == 100 || Position == 101 || Position == 102)
      return ;

    int P = Position;
    int D = Position - previous_Position;
    I +=  P;

    previous_Position = Position;

    int PID_value = (Kp * P) + (Ki * I) + (Kd * D);



    // Calculating effective motor speed:
    int left_motor_speed = initial_motor_speed + PID_value;
    int right_motor_speed = initial_motor_speed - PID_value;

    // The motor speed should not exceed the max PWM value
    left_motor_speed = constrain(left_motor_speed, 0, 255);
    right_motor_speed = constrain(right_motor_speed, 0, 255);

    /*Serial.print(PID_value);
      Serial.print("\t");
      Serial.print(left_motor_speed);
      Serial.print("\t");
      Serial.println(right_motor_speed);*/

    motor_control(right_motor_speed, left_motor_speed);
  }
}

int read_sensors()
{
  int error;

  //IR reading..........

  sensor[0] = digitalRead(sensor1);   //  analogRead digitalRead
  sensor[1] = digitalRead(sensor2);
  sensor[2] = digitalRead(sensor3);
  sensor[3] = digitalRead(sensor4);
  sensor[4] = digitalRead(sensor5);
  sensor[5] = digitalRead(sensor6);
  sensor[6] = digitalRead(sensor7);
  sensor[7] = digitalRead(sensor8);


  Serial.print("sen1     ");
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
  Serial.print("    sen8  \n");

  if ((sensor[0] == 1)) // found left
    error = 100;
  else if ((sensor[7] == 1)) // found right
    error = 101;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0) && (sensor[7] == 0)) // Make U turn
    error = 102;
  else if ((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1) && (sensor[6] == 1)) // stop
    error = 103;
  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
    error = 5;
  else if ((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
    error = 4;
  else if ((sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
    error = 3;
  else if ((sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
    error = 2;
  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
    error = 1;
  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 0) && (sensor[6] == 0))
    error = 0;
  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1) && (sensor[5] == 0) && (sensor[6] == 0))
    error = -1;
  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1) && (sensor[5] == 1) && (sensor[6] == 0))
    error = -2;
  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 1) && (sensor[6] == 0))
    error = -3;
  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 1) && (sensor[6] == 1))
    error = -4;
  else if ((sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 1))
    error = -5;
  else error = 0;

  return error;
}

char select_turn(int found_left, int found_right , int found_straight)
{
  if (found_left) return 'L';
  else if (found_straight) return 'S';
  else if (found_right) return 'R';
  else return 'B';
}

void follow(char Direction)
{
  switch (Direction)
  {
    case 'L': turn_left();
      break;
    case 'R': turn_right();
      break;
    case 'S': go_straight();
      break;
    case 'B': 
        turn_left();
      break;
  }
}

void balance()
{
  int Position;
  Position = read_sensors();
  if (Position < 0 && Position >= -5)
    move_right();
  else
    move_left();
}



void turn_right()
{
  analogWrite(ENA, turning_speed-tune);
  analogWrite(ENB, turning_speed);
  digitalWrite(motorInput1, LOW);// right forward direction...
  digitalWrite(motorInput2, HIGH);// right reverse direction...
  digitalWrite(motorInput3, HIGH);// left forward direction...
  digitalWrite(motorInput4, LOW);// left reverse direction...
  delay(delayTurn);
  int Position;
  do {
    analogWrite(ENA, turning_speed-tune);
    analogWrite(ENB, turning_speed);
    digitalWrite(motorInput1, LOW);// right forward direction...
    digitalWrite(motorInput2, HIGH);// right reverse direction...
    digitalWrite(motorInput3, HIGH);// left forward direction...
    digitalWrite(motorInput4, LOW);// left reverse direction...
    Position = read_sensors();
  } while (Position <= -good_position || Position >= good_position);
  balance();
}
void turn_left()
{
  analogWrite(ENA, turning_speed-tune);
  analogWrite(ENB, turning_speed);
  digitalWrite(motorInput1, HIGH);// right forward direction...
  digitalWrite(motorInput2, LOW);// right reverse direction...
  digitalWrite(motorInput3, LOW);// left forward direction...
  digitalWrite(motorInput4, HIGH);// left reverse direction...
  delay(delayTurn);
  int Position;
  do {
    analogWrite(ENA, turning_speed-tune);
    analogWrite(ENB, turning_speed);
    digitalWrite(motorInput1, HIGH);// right forward direction...
    digitalWrite(motorInput2, LOW);// right reverse direction...
    digitalWrite(motorInput3, LOW);// left forward direction...
    digitalWrite(motorInput4, HIGH);// left reverse direction...
    Position = read_sensors();
  } while (Position <= -good_position || Position >= good_position);
  balance();

}
void go_straight()
{
  analogWrite(ENA, forward_speed-tune);
  analogWrite(ENB, forward_speed);
  digitalWrite(motorInput1, HIGH);// right forward direction...
  digitalWrite(motorInput2, LOW);// right reverse direction...
  digitalWrite(motorInput3, HIGH);// left forward direction...
  digitalWrite(motorInput4, LOW);// left reverse direction...
}
void move_left()
{
  int Position;
  do {
    analogWrite(ENA, turning_speed-tune);
    analogWrite(ENB, turning_speed);
    digitalWrite(motorInput1, HIGH);// right forward direction...
    digitalWrite(motorInput2, LOW);// right reverse direction...
    digitalWrite(motorInput3, LOW);// left forward direction...
    digitalWrite(motorInput4, HIGH);// left reverse direction...
    Position = read_sensors();
  } while (Position != 0);
}
void move_right()
{
  int Position;
  do {
    analogWrite(ENA, turning_speed-tune);
    analogWrite(ENB, turning_speed);
    digitalWrite(motorInput1, LOW);// right forward direction...
    digitalWrite(motorInput2, HIGH);// right reverse direction...
    digitalWrite(motorInput3, HIGH);// left forward direction...
    digitalWrite(motorInput4, LOW);// left reverse direction...
    Position = read_sensors();
  } while (Position != 0);
}

void reverse()
{
  analogWrite(ENA, forward_speed-tune);
  analogWrite(ENB, forward_speed);
  digitalWrite(motorInput1, LOW);// right forward direction...
  digitalWrite(motorInput2, HIGH);// right reverse direction...
  digitalWrite(motorInput3, LOW);// left forward direction...
  digitalWrite(motorInput4, HIGH);// left reverse direction...
}

void stop_bot()
{
  digitalWrite(motorInput1, LOW);// right forward direction...
  digitalWrite(motorInput2, LOW);// right reverse direction...
  digitalWrite(motorInput3, LOW);// left forward direction...
  digitalWrite(motorInput4, LOW);// left reverse direction...
}
int motor_control(int right_motor_speed, int left_motor_speed)
{
  analogWrite(ENA, right_motor_speed-tune);  //Left Motor Speed
  analogWrite(ENB, left_motor_speed); //Right Motor Speed
  digitalWrite(motorInput1, HIGH);// right forward direction...
  digitalWrite(motorInput2, LOW);// right reverse direction...
  digitalWrite(motorInput3, HIGH);// left forward direction...
  digitalWrite(motorInput4, LOW);// left reverse direction...
}
