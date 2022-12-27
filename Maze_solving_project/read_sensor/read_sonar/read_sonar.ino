#include <NewPing.h>

NewPing sonar_left(A4,A5,30);
NewPing sonar_front(A4,A7,30);
NewPing sonar_right(A4,12,30);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(50);
}

void loop() {
  sonar();
  
}
void sonar(){
  int counter1 = millis();
  for(int k=0; k<10; k++){
    Serial.print("left dist : ");
    Serial.print(sonar_left.ping_cm());
    Serial.print("\t");
    Serial.print("front dist : ");
    Serial.print(sonar_front.ping_cm());
    Serial.print("\t");
    Serial.print("right dist : ");
    Serial.println(sonar_right.ping_cm());
  }
  int counter2 = millis();
  Serial.println(counter2-counter1);
  // exit(0);
}
