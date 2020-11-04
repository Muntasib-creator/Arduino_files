int rcv = 0;
int Prebutton = 0;
int v = 0;
int LED = 11;
int fade = 20;
void setup() {
  pinMode(2, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(A5, OUTPUT); //  pinMode(In3, OUTPUT);
  //  pinMode(In4, OUTPUT);
  analogWrite(A5, 255);
  //  digitalWrite(In2, HIGH);
  //  digitalWrite(In3, HIGH);
  //  digitalWrite(In4, HIGH);
  Serial.begin(9600); // Default communication rate of the Bluetooth module
}
void loop() {
  if (Serial.available() > 0) {
    rcv = Serial.read();
//    if (rcv == '1') {
//      //      Serial.println(rcv);
//      digitalWrite(12, HIGH);
//      //      delay(500);
//    }
//    else if (rcv == '0') {
//      //      Serial.println(rcv);
//      digitalWrite(12, LOW);
//      //      delay(500);
//    }
    if (rcv == '1') {
      //      Serial.println(rcv);
      analogWrite(LED, v);
//      Serial.println(v);
      v = v + fade;

      if (v < 5 || v > 100) {
        fade = -fade;
      }
    }
  }
}
