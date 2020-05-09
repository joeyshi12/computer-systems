#include <stdlib.h>
#include <stdio.h>

int a[] = {0, 0, 0, 0};

int q2(int arg0, int arg1, int arg2) {
  switch (arg0) {
    case 10:
      arg2 = arg1 + arg2;
      break;
    case 12:
      arg2 = arg1 - arg2;
      break;
    case 14:
      arg2 = arg1 > arg2;
      break;
    case 16:
      arg2 = arg1 < arg2;
      break;
    case 18:
      arg2 = arg1 == arg2;
      break;
    default:
      arg2 = 0;
      break;
  }
  return arg2;
}

int main(int argc, char const *argv[]) {
  a[3] = q2(a[0], a[1], a[2]);
  return 0;
}
