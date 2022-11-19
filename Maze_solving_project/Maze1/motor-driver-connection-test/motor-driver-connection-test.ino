#define ENA 6
#define right_motor_forward 7
#define right_motor_reverse 8
#define left_motor_forward 9
#define left_motor_reverse 10
#define ENB 11
int left_motor_speed = 100;
int right_motor_speed = 100;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(right_motor_forward, OUTPUT);
  pinMode(right_motor_reverse, OUTPUT);
  pinMode(left_motor_forward, OUTPUT);
  pinMode(left_motor_reverse, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  Serial.begin(9600);                     //setting serial monitor at a default baund rate of 9600
  delay(500);
  Serial.println("Started !!");
  delay(1000);
}

// the loop function runs over and over again forever
void loop() {
  analogWrite(ENA, left_motor_speed);
  analogWrite(ENB, right_motor_speed);
  digitalWrite(right_motor_forward, HIGH);
  digitalWrite(right_motor_reverse, LOW);
  digitalWrite(left_motor_forward, HIGH);
  digitalWrite(left_motor_reverse, LOW);
}
