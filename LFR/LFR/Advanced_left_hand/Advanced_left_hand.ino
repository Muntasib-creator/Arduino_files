void setup()
{

}

void loop()
{
  if (flag == 0) {
    //Reading the track...
    while (1) {
      follow_line();
      unsigned char found_left = 0;
      unsigned char found_right = 0;
      unsigned char found_straight = 0;

      int check = read_sensors();

      if (check == 100)7
        found_left = 1;
      if (check == 101)
        found_right = 1;

      go_straight();
      check = read_sensors();

      if (check >= -2 && check <= 2)
        found_straight = 1 ;
      if (check == 103)
        break;

      char Driection = select_turn(found_left, found_right, found_straight);
      turn(Direction);

    }
    //Going through shortest path...
    while (2) {

    }
  }
  stop_bot();
  flag = 1;
}


void follow_line()
{
  int Position = read_sensors();

  if (Position == 100 || Position == 101 || Position == 102)
    return ;

  int P = Position;
  int D = Position - previous_Position;
  int I +=  P;

  previous_Position = Position;

  int PID_value = (Kp * P) + (Ki * I) + (Kd * D);



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

  motor_control(right_motor_speed, left_motor_speed);
}

int read_sensors()
{
  //sonar reading.......

  //  left_dist = sonar_left.ping_cm();
  //  right_dist = sonar_right.ping_cm();
  //  front_dist = sonar_front.ping_cm();

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

  if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
    error = 5;
  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
    error = 4;
  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0)) //(sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) might occure this transition fault
    error = 3;
  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
    error = 2;
  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
    error = 1;
  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
    error = 0;
  else if (((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 0) && (sensor[5] == 0)) || ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 0)))
    error = -1;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 0))
    error = -2;
  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0)) //(sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1) && (sensor[5] == 1) might occure this transition fault
    error = -3;
  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
    error = -4;
  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0))
    error = -5;
  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0)) // Turn robot left side
    error = 100;
  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0)) // Turn robot right side
    error = 101;
  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0)) // Make U turn
    error = 102;
  else if ((sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0) && (sensor[5] == 0) && (sensor[6] == 0)) // Turn left side or stop
    error = 103;

  return error;
}

char  select_turn(unsigned char found_left, unsigned char found_right , unsigned char found_straight)
{
  if (found_left) return 'L';
  else if (found_straight) return 'S';
  else if (found_right) return 'R';
  else return 'B';
}

void simplify_path()
{
  if (path_length < 3 || path_length != 'B')
    return ;
  int total = 0 ;
  int i = 0;
  for (i = 1 ; i <= 3; i++) {
    switch (path[path_length - i])
    {
      case 'L': total += 270;
        break;
      case 'R' : total += 90;
        break;
      case 'B': total += 180;
        break;
    }
  }
  total = total % 360 ;
  switch (total)
  {
    case 90: path[path_length - 3] = 'R';
      break;
    case 180 : path[path_length - 3] = 'B';
      break;
    case 270: path[path_length - 3] = 'L';
      break;
    case 0: path[path_length - 3 ] = 'S';
      break;
  }
  path_length -= 2;
}
void turn(char Direction)
{
  switch (Direction)
  {
    case 'L': turn_left();
      break;
    case 'R': turn_right();
      break;
    case 'S': go_straight();
      break;
    case 'B': turn_left();
      break;
  }
}


void balance();
{
  if () {
    do {
      turn_right();
    } while ();
  } else {
    do {
      turn_left();
    } while ();
  }
}

void turn_right()
{
  go_straight();
  analogWrite(ENA, turning_speed);
  analogWrite(ENB, turning_speed);
  digitalWrite(motorInput1, LOW);// right forward direction...
  digitalWrite(motorInput2, HIGH);// right reverse direction...
  digitalWrite(motorInput3, HIGH);// left forward direction...
  digitalWrite(motorInput4, LOW);// left reverse direction...
  delay(delayTRN);
  do {
    analogWrite(ENA, turning_speed);
    analogWrite(ENB, turning_speed);
    digitalWrite(motorInput1, LOW);// right forward direction...
    digitalWrite(motorInput2, HIGH);// right reverse direction...
    digitalWrite(motorInput3, HIGH);// left forward direction...
    digitalWrite(motorInput4, LOW);// left reverse direction...
  } while ();
  balance();
}
void turn_left()
{
  go_straight();
  analogWrite(ENA, turning_speed);
  analogWrite(ENB, turning_speed);
  digitalWrite(motorInput1, HIGH);// right forward direction...
  digitalWrite(motorInput2, LOW);// right reverse direction...
  digitalWrite(motorInput3, LOW);// left forward direction...
  digitalWrite(motorInput4, HIGH);// left reverse direction...
  delay(delayTRN);
  do {
    analogWrite(ENA, turning_speed);
    analogWrite(ENB, turning_speed);
    digitalWrite(motorInput1, HIGH);// right forward direction...
    digitalWrite(motorInput2, LOW);// right reverse direction...
    digitalWrite(motorInput3, LOW);// left forward direction...
    digitalWrite(motorInput4, HIGH);// left reverse direction...
  } while ();
  balance();
}
void go_straight()
{
  analogWrite(ENA, forward_speed);
  analogWrite(ENB, forward_speed);
  digitalWrite(motorInput1, HIGH);// right forward direction...
  digitalWrite(motorInput2, LOW);// right reverse direction...
  digitalWrite(motorInput3, HIGH);// left forward direction...
  digitalWrite(motorInput4, LOW);// left reverse direction...
  delay(delayFWD);
}
void reverse()
{
  balance();
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
  analogWrite(ENA, right_motor_speed);  //Left Motor Speed
  analogWrite(ENB, left_motor_speed); //Right Motor Speed
  digitalWrite(motorInput1, HIGH);// right forward direction...
  digitalWrite(motorInput2, LOW);// right reverse direction...
  digitalWrite(motorInput3, HIGH);// left forward direction...
  digitalWrite(motorInput4, LOW);// left reverse direction...
  delay(delayFWD);
}

