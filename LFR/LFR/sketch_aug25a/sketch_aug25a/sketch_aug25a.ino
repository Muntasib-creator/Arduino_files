const int trigPin = 12;
const int echoPin = 13;
int ab=10;
// defines variables
long duration;
int distance;
void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
/*digitalWrite(11,HIGH);
delay(6000);
digitalWrite(11,LOW);
delay(0);*/
Serial.begin(9600); // Starts the serial communication
}
void loop() {
// Clears the trigPin

digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
/*if(distance<=15){digitalWrite(ab,HIGH);}
else{digitalWrite(ab,LOW);}*/
  
}
