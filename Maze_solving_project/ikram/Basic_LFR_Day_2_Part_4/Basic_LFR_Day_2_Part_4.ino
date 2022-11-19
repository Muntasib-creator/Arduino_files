#include<EEPROM.h>
#define lmf 7
#define lmb 6
#define lms 5
#define rmf 8
#define rmb 9
#define rms 10
#define spl 20
#define spr 19
#define node_delay 40
#define tsp 120
#define turn_brake 40
#define u_turn_timer 150
char side = 'l';

byte path[20], counter = 0;
uint32_t m1, m2;
char turn = 's', cross = 's';
int s[6];
byte sensor, sum;
int mid[] = {150, 150, 150, 150, 150, 150};
int base[] = {32, 16, 8, 4, 2, 1};
void setup() {
  pinMode(lmf, OUTPUT);
  pinMode(lmb, OUTPUT);
  pinMode(rmf, OUTPUT);
  pinMode(rmb, OUTPUT);
  //  motor(10 * spl, 10 * spr);
  Serial.begin(9600);
  load_value();
  maze_scan();
}

void loop() {
  //  analog_value();
  //  digital_value();

}
