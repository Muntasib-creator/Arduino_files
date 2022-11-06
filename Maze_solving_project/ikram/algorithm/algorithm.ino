void algorithm() {
  int s = 0, a = 0;
  while (1) {
    if (path[a + 1] == 4) {
      a += 2; path[s] = path[s] + path[a];
    }
    if (path[s] == 4) {
      s--; a++; path[s] = path[s] + path[a];
    }
    else {
      s++; a++; path[s] = path[a];
    }
    if (path[s] == 5) break;
  }
}
