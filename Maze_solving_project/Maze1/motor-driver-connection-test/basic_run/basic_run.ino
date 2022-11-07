#define ir_left_most A3
#define if_left A2
#define ir_left_least A1
#define ir_center_left A0
#define ir_center_right 2
#define ir_right_least 3
#define ir_right 4
#define ir_right_most 5

// Motor Pins
#define ENA 6
#define right_forward 7
#define right_reverse 8
#define left_forward 9
#define left_reverse 10
#define ENB 11

// error definations
#define sharp_left_found 100
#define sharp_right_found 101
#define all_white_found 102
#define all_black_found 103
#define left_deflection_7 7
#define left_deflection_6 6
#define left_deflection_5 5
#define left_deflection_4 4
#define left_deflection_3 3
#define left_deflection_2 2
#define left_deflection_1 1
#define middle_of_line_found 0
#define right_deflection_1 -1
#define right_deflection_2 -2
#define right_deflection_3 -3
#define right_deflection_4 -4
#define right_deflection_5 -5
#define right_deflection_6 -6
#define right_deflection_7 -7

int ir_sensor_readings[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int size_of_array = 8;
int left_motor_speed = 0;
int right_motor_speed = 0;
int initial_motor_speed = 60;
int sharp_turn_speed = 80;
int default_speed = 80;
// PID Constants
float Kp = 6.00;
float Ki = 0;
float Kd = 20.00;

// PID Variables
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;
int flag = 0;
/***************************************************************************************  Void Setup  **************************************************************************************/
void setup()
{
  pinMode(ir_left_most, INPUT);
  pinMode(if_left, INPUT);
  pinMode(ir_left_least, INPUT);
  pinMode(ir_center_left, INPUT);
  pinMode(ir_center_right, INPUT);
  pinMode(ir_right_least, INPUT);
  pinMode(ir_right, INPUT);
  pinMode(ir_right_most, INPUT);
  pinMode(right_forward, OUTPUT);
  pinMode(right_reverse, OUTPUT);
  pinMode(left_forward, OUTPUT);
  pinMode(left_reverse, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  Serial.begin(9600);
  delay(500);
  Serial.println("Started !!");
  delay(1000);
}

void read_sensor_values();
void forward(int left_speed = default_speed, int right_speed = default_speed);
void reverse(int left_speed = default_speed, int right_speed = default_speed);
void sharp_lef_turn(int left_speed = sharp_turn_speed, int right_speed = sharp_turn_speed);
void sharp_right_turn(int left_speed = sharp_turn_speed, int right_speed = sharp_turn_speed);
void stop_bot();
bool ir_value_check(int a[], int b[]);

void loop()
{
  read_ir_sensor_values();
  calculate_pid();
  motor_control();
}

bool ir_value_check(int a[], int b[])
{
  for (int i = 1; i < size_of_array - 1; i++)
  {
    if (a[i] != b[i])
    {
      return false;
    }
  }
  return true;
}

void read_ir_sensor_values()
{
  // IR reading..........
  ir_sensor_readings[0] = digitalRead(ir_left_most); //  analogRead digitalRead
  ir_sensor_readings[1] = digitalRead(if_left);
  ir_sensor_readings[2] = digitalRead(ir_left_least);
  ir_sensor_readings[3] = digitalRead(ir_center_left);
  ir_sensor_readings[4] = digitalRead(ir_center_right);
  ir_sensor_readings[5] = digitalRead(ir_right_least);
  ir_sensor_readings[6] = digitalRead(ir_right);
  ir_sensor_readings[7] = digitalRead(ir_right_most);
  Serial.print(ir_sensor_readings[0]);
  Serial.print("\t");
  Serial.print(ir_sensor_readings[1]);
  Serial.print("\t");
  Serial.print(ir_sensor_readings[2]);
  Serial.print("\t");
  Serial.print(ir_sensor_readings[3]);
  Serial.print("\t");
  Serial.print(ir_sensor_readings[4]);
  Serial.print("\t");
  Serial.print(ir_sensor_readings[5]);
  Serial.print("\t");
  Serial.print(ir_sensor_readings[6]);
  Serial.print("\t");
  Serial.print(ir_sensor_readings[7]);
  int white[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int black[8] = {0, 1, 1, 1, 1, 1, 1, 0};
  int err_l7[8] = {1, 0, 0, 0, 0, 0, 0, 0};
  int err_l6[8] = {1, 1, 0, 0, 0, 0, 0, 0};
  int err_l5[8] = {0, 1, 0, 0, 0, 0, 0, 0};
  int err_l4[8] = {0, 1, 1, 0, 0, 0, 0, 0};
  int err_l3[8] = {0, 0, 1, 0, 0, 0, 0, 0};
  int err_l2[8] = {0, 0, 1, 1, 0, 0, 0, 0};
  int err_l1[8] = {0, 0, 0, 1, 0, 0, 0, 0};
  int err_c0[8] = {0, 0, 0, 1, 1, 0, 0, 0};
  int err_r1[8] = {0, 0, 0, 0, 1, 0, 0, 0};
  int err_r2[8] = {0, 0, 0, 0, 1, 1, 0, 0};
  int err_r3[8] = {0, 0, 0, 0, 0, 1, 0, 0};
  int err_r4[8] = {0, 0, 0, 0, 0, 1, 1, 0};
  int err_r5[8] = {0, 0, 0, 0, 0, 0, 1, 0};
  int err_r6[8] = {0, 0, 0, 0, 0, 0, 1, 1};
  int err_r7[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  error = middle_of_line_found;

  if (ir_value_check(ir_sensor_readings, white))
  {
    error = all_white_found;
  }
  if (ir_value_check(ir_sensor_readings, black))
  {
    error = all_black_found;
  }
  if (ir_value_check(ir_sensor_readings, err_l7))
  {
    error = left_deflection_7;
  }
  if (ir_value_check(ir_sensor_readings, err_l6))
  {
    error = left_deflection_6;
  }
  if (ir_value_check(ir_sensor_readings, err_l5))
  {
    error = left_deflection_5;
  }
  if (ir_value_check(ir_sensor_readings, err_l4))
  {
    error = left_deflection_4;
  }
  if (ir_value_check(ir_sensor_readings, err_l3))
  {
    error = left_deflection_3;
  }
  if (ir_value_check(ir_sensor_readings, err_l2))
  {
    error = left_deflection_2;
  }
  if (ir_value_check(ir_sensor_readings, err_l1))
  {
    error = left_deflection_1;
  }
  if (ir_value_check(ir_sensor_readings, err_c0))
  {
    error = middle_of_line_found;
  }
  if (ir_value_check(ir_sensor_readings, err_r1))
  {
    error = right_deflection_1;
  }
  if (ir_value_check(ir_sensor_readings, err_r2))
  {
    error = right_deflection_2;
  }
  if (ir_value_check(ir_sensor_readings, err_r3))
  {
    error = right_deflection_3;
  }
  if (ir_value_check(ir_sensor_readings, err_r4))
  {
    error = right_deflection_4;
  }
  if (ir_value_check(ir_sensor_readings, err_r5))
  {
    error = right_deflection_5;
  }
  if (ir_value_check(ir_sensor_readings, err_r6))
  {
    error = right_deflection_6;
  }
  if (ir_value_check(ir_sensor_readings, err_r7))
  {
    error = right_deflection_7;
  }
}

void calculate_pid()
{
  if (error > 10)
    return;
  Serial.print(" Error : ");
  Serial.print(error);
  Serial.print(" ");
  P = error;
  I = I + previous_I;
  D = error - previous_error;
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  Serial.print("PID: ");
  Serial.print(PID_value);
  previous_I = I;
  previous_error = error;
}

void forward(int left_speed = default_speed, int right_speed = default_speed)
{
  Serial.print(" ");
  Serial.print("Left spd: ");
  Serial.print(left_speed);
  Serial.print(" ");
  Serial.print(" Right spd: ");
  Serial.print(right_speed);
  Serial.print("\n");
  analogWrite(ENA, left_speed);
  analogWrite(ENB, right_speed);
  digitalWrite(left_forward, HIGH);
  digitalWrite(left_reverse, LOW);
  digitalWrite(right_forward, HIGH);
  digitalWrite(right_reverse, LOW);
}
void reverse(int left_speed = default_speed, int right_speed = default_speed)
{
  analogWrite(ENA, left_speed);
  analogWrite(ENB, right_speed);
  digitalWrite(left_forward, LOW);
  digitalWrite(left_reverse, HIGH);
  digitalWrite(right_forward, LOW);
  digitalWrite(right_reverse, HIGH);
}
void sharp_lef_turn(int left_speed = sharp_turn_speed, int right_speed = sharp_turn_speed)
{
  analogWrite(ENA, left_speed);
  analogWrite(ENB, right_speed);
  digitalWrite(left_forward, HIGH);
  digitalWrite(left_reverse, LOW);
  digitalWrite(right_forward, LOW);
  digitalWrite(right_reverse, HIGH);
}
void sharp_right_turn(int left_speed = sharp_turn_speed, int right_speed = sharp_turn_speed)
{
  analogWrite(ENA, left_speed);
  analogWrite(ENB, right_speed);
  digitalWrite(left_forward, LOW);
  digitalWrite(left_reverse, HIGH);
  digitalWrite(right_forward, HIGH);
  digitalWrite(right_reverse, LOW);
}
void stop_bot()
{
  digitalWrite(left_forward, LOW);
  digitalWrite(left_reverse, LOW);
  digitalWrite(right_forward, LOW);
  digitalWrite(right_reverse, LOW);
}
void motor_control()
{
  // Calculating the effective motor speed:
  int left_motor_speed = initial_motor_speed - PID_value;
  int right_motor_speed = initial_motor_speed + PID_value;
  // The motor speed should not exceed the max PWM value
  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);
  forward(left_motor_speed, right_motor_speed);
}