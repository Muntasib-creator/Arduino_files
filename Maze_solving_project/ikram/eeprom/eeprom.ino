void save_value() {
  byte i = 0;
  while (1) {
    EEPROM.write(i + 50, path[i]); delay(1);
    if (path[i] == 5) break;
    i++;
  }
}
void load_value() {
  byte i = 0;
  while (1) {
    path[i] = EEPROM.read(i + 50);
    Serial.println(path[i]);
    if (path[i] == 5 || i == 20 || path[i] == 0) break;
    i++;
  }
}
