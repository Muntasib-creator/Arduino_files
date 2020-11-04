int sensor[]={4,7,8,10,11,12};
int motor[]={6,9,3,5};
int lfa,rfa;
const int trigPin = 12;
const int echoPin = 11;
long duration;
int distance;

void setup() {
  // put your setup code here, to run once:
  for(int i=0;i<6;i++){
    pinMode(sensor[i],INPUT);
  }
  for(int i=0;i<4;i++){
    pinMode(motor[i],OUTPUT);
  }
  Serial.begin(9600);

}

void loop() {
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
  if(distance<=17){
    analogWrite(motor[0],0);
    analogWrite(motor[1],0);
    analogWrite(motor[2],0);
    analogWrite(motor[3],0);
}    

    else {
    analogWrite(motor[0],200);
    analogWrite(motor[1],0);
    analogWrite(motor[2],200);
    analogWrite(motor[3],0); 
      }
  }
