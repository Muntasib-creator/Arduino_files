void motor(int a, int b) {
  if (a >= 0) {
    digitalWrite(lmf, 1);
    digitalWrite(lmb, 0);
  }
  else {
    a = -a;
    digitalWrite(lmf, 0);
    digitalWrite(lmb, 1);
  }

  if (b > 0) {
    digitalWrite(rmf, 1);
    digitalWrite(rmb, 0);
  }
  else {
    b = -b;
    digitalWrite(rmf, 0);
    digitalWrite(rmb, 1);
  }
  analogWrite(lms, a);
  analogWrite(rms, b);
}

void brake(){
  motor(-10*spl,10*spr); delay(20);
  motor(0,0); delay(200);
}
