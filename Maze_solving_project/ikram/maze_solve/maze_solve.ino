void maze_solve() {
  counter = 0;
  while (1) {
p:   reading();
    if (sum == 0) {
      if (turn != 's') {
        brake();
        (turn == 'l') ? motor(-tsp, tsp) : motor(tsp, -tsp);
        while (!s[2] || !s[3])  reading();
        (turn == 'l') ? motor(tsp, -tsp) : motor(-tsp, tsp);
        delay(turn_brake); turn = 's'; cross = 's';
      }
    }
    else if (sum == 1 || sum == 2) {
      if (cross != 's') {
        brake();
        (cross == 'l') ? motor(-tsp, tsp) : motor(tsp, -tsp);
        //        while (s[1] || s[2] || s[3] || s[4])  reading(); //bring it out of the line
        delay(1500);
        while (!s[2] && !s[3])  reading(); //bring it into the line
        (cross == 'l') ? motor(tsp, -tsp) : motor(-tsp, tsp);
        delay(turn_brake);  turn = 's'; cross = 's';
      }
      if (sensor == 0b001100) motor(10 * spl, 10 * spr);

      else if (sensor == 0b001000) motor(9 * spl, 10 * spr);
      else if (sensor == 0b011000) motor(6 * spl, 10 * spr);
      else if (sensor == 0b010000) motor(3 * spl, 10 * spr);
      else if (sensor == 0b110000) motor(0 * spl, 10 * spr);
      else if (sensor == 0b100000) motor(-3 * spl, 10 * spr);

      else if (sensor == 0b000100) motor(10 * spl, 9 * spr);
      else if (sensor == 0b000110) motor(10 * spl, 6 * spr);
      else if (sensor == 0b000010) motor(10 * spl, 3 * spr);
      else if (sensor == 0b000011) motor(10 * spl, 0 * spr);
      else if (sensor == 0b000001) motor(10 * spl, -3 * spr);
    }
    else if (sum == 3 || sum == 4 || sum == 5) {
      if (s[0] && !s[5] && (s[2] + s[3])) {
        turn = 'l';
        while (s[0] && !s[5]) reading();
        if (!s[0]) {
          delay(node_delay);
          if (sum > 0) {
            if (path[counter] == 1) cross = 'l';
            counter++;
          }
        }
      }
      else if (s[5] && !s[0] && (s[2] + s[3])) {
        turn = 'r';
        while (s[5] && !s[0]) reading();
        if (!s[5]) {
          delay(node_delay);
          if (sum > 0) {
            if (path[counter] == 1) cross = 'r';
            counter++;
          }
        }
      }
      m1 = millis();
    }
    else if (sum == 6) {
      m2 = millis();
      while (s[0] && s[5]) {
        reading();
        if (millis - m2 > 200) {
          motor(0, 0);
          return;
        }
      }
      delay(node_delay); reading();
      if (sum > 0) {
        if (path[counter] == 1) {
          (side == 'l') ? cross = 'l' : cross = 'r';
        }
        else if (path[counter] == 3) {
          (side == 'r') ? cross = 'l' : cross = 'r';
        }
      }
      if (path[counter] == 1) {
        (side == 'l') ? turn = 'l' : turn = 'r';
      }
      else if (path[counter] == 3) {
        (side == 'r') ? turn = 'l' : turn = 'r';
      }
      counter++;
      m1 = millis();
    }
    if (millis() - m1 > 200) turn = 's';
  }
}
