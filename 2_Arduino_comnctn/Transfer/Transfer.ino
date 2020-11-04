//#define In1 4
//#define In2 5
//#define In3 6
//#define In4 7
int button = 0;
int button2 = 0;
int Prebutton = 0;
int flag = 0;
void setup() {
  pinMode(2, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, OUTPUT);
  //  pinMode(In3, OUTPUT);
  //  pinMode(In4, OUTPUT);
  analogWrite(A5, 255);
  //  digitalWrite(In2, HIGH);
  //  digitalWrite(In3, HIGH);
  //  digitalWrite(In4, HIGH);
  Serial.begin(9600); // Default communication rate of the Bluetooth module
}
void loop() {
  button = digitalRead(2);
//  button2 = digitalRead(A4);
//  Serial.print(button);
//  Serial.println(button2);
  if (button == 1 && Prebutton == 0 && flag == 0) {
    Serial.print("button=");
    Serial.print(button);
    Serial.print("\t");
    Serial.println("1");
    delay(250);
    flag = 1;
  }
  else if (button == 1 && Prebutton == 1 && flag == 1) {
    Serial.print("button=");
    Serial.print(button);
    Serial.print("\t");
    Serial.println("0");

    delay(250);
    flag = 0;
  }
  
//  if (button2 == 1) {
//    Serial.println("2");
//    delay(250);
//  }


  Prebutton = button;
}
