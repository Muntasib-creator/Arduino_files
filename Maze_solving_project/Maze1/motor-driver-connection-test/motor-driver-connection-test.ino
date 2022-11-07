#define ENA 6
#define motorInput1 7
#define motorInput2 8
#define motorInput3 9
#define motorInput4 10
#define ENB 11
int left_motor_speed = 100;
int right_motor_speed = 100;

// the setup function runs once when you press reset or power the board
void setup() {
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

// the loop function runs over and over again forever
void loop() {
  analogWrite(ENA, left_motor_speed);
  analogWrite(ENB, right_motor_speed);
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
  delay(2000);  
}
