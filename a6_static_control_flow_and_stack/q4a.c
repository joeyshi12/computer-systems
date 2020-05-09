#include <stdlib.h>
#include <stdio.h>

int a[10];

void bar(int arg0, int arg1) {
  a[arg1] = arg0 + a[arg1];
}

int main(int argc, char const *argv[]) {
  int x = 1;
  int y = 2;
  bar(3, 4);
  bar(x, y);
  for (int i = 0; i < 10; i++) {
    printf("%d\n", a[i]);
  }
  return 0;
}
