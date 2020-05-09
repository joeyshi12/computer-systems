#include <stdlib.h>
#include <stdio.h>

int x[8] = {1, 2, 3, -1, -2, 0, 184, 340057058};
int y[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int f(int x) {
  int count = 0;
  for (int i = x; i != 0; i *= 2) {
    if (i < 0) {
      count++;
    }
  }
  return count;
}

int main(int argc, char const *argv[]) {
  int i = 8;
  while (i != 0) {
    i--;
    y[i] = f(x[i]);
  }
  for (int i = 0; i < 8; i++) {
    printf("%d\n", x[i]);
  }
  for (int i = 0; i < 8; i++) {
    printf("%d\n", y[i]);
  }
  return 0;
}
