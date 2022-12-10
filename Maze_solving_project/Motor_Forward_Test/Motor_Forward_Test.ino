#define ENA 6
#define right_forward 7
#define right_reverse 8
#define left_forward 9
#define left_reverse 10
#define ENB 11

#define sensor1 A0      // Left most sensor
#define sensor2 A1      // Left sensor
#define sensor3 A2      // Middle Left sensor
#define sensor4 A3      // Middle Right sensor
#define sensor5 5       // Right sensor
#define sensor6 4       // Right most sensor
#define sensor7 3
#define sensor8 2

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
  analogWrite(ENA, 60); //Left Motor Speed
  analogWrite(ENB, 60); //Right Motor Speed
  // forward();
  forward();
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
void sharpLeftTurn() {
  digitalWrite(right_forward, LOW);
  digitalWrite(right_reverse, HIGH);
  digitalWrite(left_forward, HIGH);
  digitalWrite(left_reverse, LOW);
}
void sharpRightTurn() {
  digitalWrite(right_forward, HIGH);
  digitalWrite(right_reverse, LOW);
  digitalWrite(left_forward, LOW);
  digitalWrite(left_reverse, HIGH);
}
void stop_bot() {
  //All motors at ease
  digitalWrite(right_forward, LOW);
  digitalWrite(right_reverse, LOW);
  digitalWrite(left_forward, LOW);
  digitalWrite(left_reverse, LOW);
}