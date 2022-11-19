void reading(){
  sensor = 0;
  sum = 0;
  for(int i=0; i<6; i++){
    s[i] = analogRead(i); // anaog part
    (s[i] < mid[i])? s[i] = 0 : s[i] = 1; //digital conversion
    sensor += s[i] * base [i];
    sum += s[i];
  }
}

void analog_value(){
  for(int i=0; i<6; i++){
    s[i] = analogRead(i);
    Serial.print(s[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void digital_value(){
  for(int i=0; i<6; i++){
    s[i] = analogRead(i);
    (s[i] < mid[i])? s[i] = 0 : s[i] = 1; //digital conversion
    Serial.print(s[i]);
    Serial.print(" ");
  }
  Serial.println();
}
