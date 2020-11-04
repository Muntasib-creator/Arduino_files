const int trigPin = A4;
const int echoPin_left = 12;
const int echoPin_front = A5;
const int echoPin_right = 13;       // left A3   front A4   right A5
int ab=10;
// defines variables
long duration_left;
long duration_front;
long duration_right;

int distance_left=220;
int distance_front=320;
int distance_right=420;

void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin_left, INPUT);
pinMode(echoPin_front, INPUT);
pinMode(echoPin_right, INPUT);// Sets the echoPin as an Input
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
delayMicroseconds(2);
// Reads the echoPin, returns the sound wave travel time in microseconds
  
duration_right = pulseIn(echoPin_right, HIGH,7000);

digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

duration_front = pulseIn(echoPin_front, HIGH,7000);

digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

duration_left = pulseIn(echoPin_left, HIGH, 7000);     

distance_left= duration_left*0.017;
distance_front= duration_front*0.017;
distance_right= duration_right*0.017;


// Prints the distance on the Serial Monitor

//Serial.print("\nDuration Left: ");
//Serial.println(duration_left);


Serial.print("    Distance Left: ");
Serial.println(distance_left);
Serial.print("    Distance Front: ");
Serial.println(distance_front);
Serial.print("    Distance Right: ");
Serial.println(distance_right);
/*if(distance<=15){digitalWrite(ab,HIGH);}
else{digitalWrite(ab,LOW);}*/

delay(500);
  
}
