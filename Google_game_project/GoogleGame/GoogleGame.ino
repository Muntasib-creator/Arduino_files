#include <Servo.h>
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
int d1=1000;
int d2=100;
void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}
void loop() {
  myservo.write(80);             
  delay(d1);
  myservo.write(64);              
  delay(d2);                                                       
}
