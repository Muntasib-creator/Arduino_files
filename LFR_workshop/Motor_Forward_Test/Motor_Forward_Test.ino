/* IR sensor pins */
#define sensor1 A0  
#define sensor2 A1  
#define sensor3 A2  
#define sensor4 A3  
#define sensor5 A4 

// Motor Pins
#define ENA 11  // Left motor control
#define left_reverse 10   // IN1 = left_reverse
#define left_forward 9    // IN2 = left_forward
#define right_reverse 8   // IN3 = right_reverse
#define right_forward 7   // IN4 = right_forward
#define ENB 6  // Right motor control

int speed;
void setup() {

  pinMode(right_forward, OUTPUT);
  pinMode(right_reverse, OUTPUT);
  pinMode(left_forward, OUTPUT);
  pinMode(left_reverse, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  /*pinMode(ledPin1, OUTPUT);
    digitalWrite(ledPin1, LOW);*/


  Serial.begin(9600);                     //setting serial monitor at a default baund rate of 9600
  delay(500);
  Serial.println("Started !!");
  delay(1000);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  motor_forward_test();
}

void motor_forward_test() {
  forward();

  speed = 100;
  analogWrite(ENA, speed); //Left Motor Speed
  analogWrite(ENB, speed); //Right Motor Speed
  delay(2000);
  speed = 90;
  analogWrite(ENA, speed); //Left Motor Speed
  analogWrite(ENB, speed); //Right Motor Speed
  delay(2000);
  speed = 80;
  analogWrite(ENA, speed); //Left Motor Speed
  analogWrite(ENB, speed); //Right Motor Speed
  delay(2000);
  speed = 70;
  analogWrite(ENA, speed); //Left Motor Speed
  analogWrite(ENB, speed); //Right Motor Speed
  delay(2000);
  speed = 60;
  analogWrite(ENA, speed); //Left Motor Speed
  analogWrite(ENB, speed); //Right Motor Speed
  delay(2000);
  speed = 50;
  analogWrite(ENA, speed); //Left Motor Speed
  analogWrite(ENB, speed); //Right Motor Speed
  delay(2000);
  speed = 40;
  analogWrite(ENA, speed); //Left Motor Speed
  analogWrite(ENB, speed); //Right Motor Speed
  delay(2000);
}


void forward() {
  digitalWrite(right_forward, HIGH);
  digitalWrite(right_reverse, LOW);
  digitalWrite(left_forward, HIGH);
  digitalWrite(left_reverse, LOW);
}
void reverse() {
  digitalWrite(right_forward, LOW);
  digitalWrite(right_reverse, HIGH);
  digitalWrite(left_forward, LOW);
  digitalWrite(left_reverse, HIGH);
}
void sharpRightTurn() {
  digitalWrite(right_forward, LOW);
  digitalWrite(right_reverse, HIGH);
  digitalWrite(left_forward, HIGH);
  digitalWrite(left_reverse, LOW);
}
void sharpLeftTurn() {
  digitalWrite(right_forward, HIGH);
  digitalWrite(right_reverse, LOW);
  digitalWrite(left_forward, LOW);
  digitalWrite(left_reverse, HIGH);
}
void stopBot() {
  digitalWrite(right_forward, LOW);
  digitalWrite(right_reverse, LOW);
  digitalWrite(left_forward, LOW);
  digitalWrite(left_reverse, LOW);
}