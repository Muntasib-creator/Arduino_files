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

// ERROR VARS
int error = 0;
int WHITE = 102;
int BLACK = 103;

/***************************************************************************************  Void Setup  **************************************************************************************/
void setup() {
  Serial.begin(9600);                     //setting serial monitor at a default baund rate of 9600
  delay(500);
  Serial.println("Started !!");
  delay(1000);
}
/***************************************************************************************  Void Loop  **************************************************************************************/

bool IR_value_check(int a[], int b[]){
  for (int i = 0;  i < size_of_array; i++){
    if( a[i] != b[i]){
      return false;
    }
  }
  return true;
}

void loop()
{
  read_sensor_values();
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

void read_sensor_values(){
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
