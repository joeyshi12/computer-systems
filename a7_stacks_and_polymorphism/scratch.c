#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void a() {printf("a\n");}
void b() {printf("b\n");}
void c() {printf("c\n");}

struct fptrlist {
  void (*x)();
  void (*y)();
  void (*z)();
};

struct fptrlist table = {a, b, c};

void run0() {
  table.y();
}





int main(int argc, char const *argv[]) {
  run0();
  return 0;
}
