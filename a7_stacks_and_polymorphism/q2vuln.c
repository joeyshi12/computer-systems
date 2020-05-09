#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct string {
  int length;
  char* _str;
};

struct string str1 = {30, "Welcome! Please enter a name:"};
struct string str2 = {11, "Good luck, "};

void print(struct string* string) {
  write(1, string->_str, string->length);
}

int main(int argc, char const *argv[]) {
  char buf[128];
  print(&str1);
  register int size = read(0, buf, 256);
  struct string strb = {size, buf};
  print(&str2);
  print(&strb);
  return 128;
}
