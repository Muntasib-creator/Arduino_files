#include<NewPing.h>

const int trigPin = A4;
const int echoPin_left = 12;
const int echoPin_front = A5;
const int echoPin_right = 13;       // left A3   front A4   right A5
int ab = 10;
// defines variables
//long duration_left;
//long duration_front;
long duration_right;

int distance_left;
int distance_front;
int distance_right;

int max_dis = 100;

NewPing sonar_left(trigPin , echoPin_left, max_dis);
NewPing sonar_right(trigPin, echoPin_right, max_dis);
NewPing sonar_front(trigPin, echoPin_front, max_dis);


void setup() {



  Serial.begin(9600);                     //setting serial monitor at a default baund rate of 9600
  delay(500);
  Serial.println("Started !!");
  delay(1000);
}

void loop() {

  distance_left = sonar_left.ping_cm();
  distance_right = sonar_right.ping_cm();
  distance_front = sonar_front.ping_cm();

//  digitalWrite(trigPin, LOW);
//  delayMicroseconds(2);
//  // Sets the trigPin on HIGH state for 10 micro seconds
//  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trigPin, LOW);
//  delayMicroseconds(2);
//  // Reads the echoPin, returns the sound wave travel time in microseconds
//
//  duration_right = pulseIn(echoPin_right, HIGH, 7000);
//  distance_right= duration_right*0.017;

  Serial.print("\nDistance Left: ");
  Serial.println(distance_left);
  Serial.print("    Distance Front: ");
  Serial.println(distance_front);
  Serial.print("    Distance Right: ");
  Serial.println(distance_right);

  delay(500);
}
