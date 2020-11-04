#define In1 4
#define In2 5
#define In3 6
#define In4 7
int state = 0;
void setup() {
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
  digitalWrite(In1, HIGH);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, HIGH);
  Serial.begin(9600); // Default communication rate of the Bluetooth module
}
void loop() {
  if (Serial.available() > 0) { // Checks whether data is comming from the serial port
    state = Serial.read(); // Reads the data from the serial port
  }
  if (state == '0') {
    digitalWrite(In1, LOW);
    Serial.println("Tube light ON\nBy-Muntasib");
    state = 0;
  }
  else if (state == '1') {
    digitalWrite(In1, HIGH);
    Serial.println("Tube light OFF\nBy-Muntasib");
    state = 0;
  }
  else if (state == '2') {
    digitalWrite(In2, LOW);
    Serial.println("Fan ON\nBy-Muntasib");
    state = 0;
  }
  else if (state == '3') {
    digitalWrite(In2, HIGH);
    Serial.println("Fan OFF\nBy-Muntasib");
    state = 0;
  }
  else if (state == '6') {
    digitalWrite(In3, LOW);
    Serial.println("Other ON\nBy-Muntasib");
    state = 0;
  }
  else if (state == '7') {
    digitalWrite(In3, HIGH);
    Serial.println("Other OFF\nBy-Muntasib");
    state = 0;
  }
  else if (state == '4') {
    digitalWrite(In4, LOW);
    Serial.println("Dim Light OFF\nBy-Muntasib");
    state = 0;
  }
  else if (state == '5') {
    digitalWrite(In4, HIGH);
    Serial.println("Dim Light OFF\nBy-Muntasib");
    state = 0;
  }
}
