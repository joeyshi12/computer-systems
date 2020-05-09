#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "list.h"
#include "list.c"


void printStr (element_t ev) {
  char** e = (char**) ev;
  if (*e == NULL) {
    printf("NULL\n");
  } else {
    printf ("%s\n", *e);
  }
}

void printInt (element_t ev) {
  int* e = (int*) ev;
  printf ("%d\n", *e);
}

void strToInt (element_t* rv, element_t av) {
  char** a = (char**) av;
  int** r = (int**) rv;
  if (*r == NULL) {
    *r = malloc(sizeof(int));
  }
  char* endp;
  int value = strtol(*a, &endp, 10);
  if(*a[0] != '\0' && endp[0] == '\0') {
    **r = value;
  } else {
    **r = -1;
  }
}

void replaceIntWithNULL (element_t* rv, element_t av, element_t bv) {
  char** a = (char**) av;
  int* b = (int*) bv;
  char*** r = (char***) rv;
  printf("a: %s\n", *a);
  printf("b: %d\n", *b);
  if (*b == -1) {
    printf("%p\n", *r);
    if (**r == NULL) {
      printf("%s\n", "pass if statement");
      **r = malloc(strlen(*a) + 1);
      printf("%s\n", "successful malloc");
    }
    char* str = **r;
    printf("str: %s\n", str);
    printf("%s\n", "asd");
    strcpy(str, *a);
    printf("%s\n", "copied");
  } else {
    *r = NULL;
  }
}

int notNegative (element_t av) {
  return *(int*) av >= 0;
}

int notNull (element_t av) {
  return av != NULL;
}

void substring (element_t* rv, element_t av, element_t bv) {
  int* a = (int*) av;
  char** b = (char**) bv;
  char*** r = (char***) rv;
  if (*r == NULL) {
    *r = malloc(*a + 1);
  }
  strncpy(**r, *b, *a);
}

void joinStr (element_t* rv, element_t av, element_t bv) {
  char** a = (char**) av;
  char** b = (char**) bv;
  char*** r = (char***) rv;
  strcpy(**r, *a);
  strcat(**r, *b);
  strcat(**r, " ");
}

void add (element_t* rv, element_t av, element_t bv) {
  int* a = (int*) av;
  int* b = (int*) bv;
  int** r = (int**) rv;
  if (*r == NULL) {
     *r = malloc(sizeof(int));
  }
  **r = *a + *b;
}

void max (element_t* rv, element_t av, element_t bv) {
  int* a = (int*) av;
  int* b = (int*) bv;
  int** r = (int**) rv;
  **r = *a > *b ? *a : *b;
}

int main(int argc, char const *argv[]) {
  struct list* list = list_create();
  for (int i=0; i<argc-1; i++) {
    list_append(list, (element_t) &argv[i+1]);
  }
  printf("%s\n", "----------- list ----------");
  list_foreach(printStr, list);
  printf("--------------------------\n");

  struct list* intList0 = list_create();
  list_map1(strToInt, intList0, list);
  printf("%s\n", "-------- intList0 --------");
  list_foreach(printInt, intList0);
  printf("---------------------------\n");

  struct list* strList0 = list_create();
  list_map2(replaceIntWithNULL, strList0, list, intList0);
  printf("%s\n", "--------- strList0 --------");
  list_foreach(printStr, strList0);
  printf("----------------------------\n");

  struct list* strList1 = list_create();
  list_filter(notNull, strList1, strList0);
  // printf("%s\n", "--------- strList1 --------");
  // list_foreach(printStr, strList1);
  // printf("----------------------------\n");

  struct list* intList1 = list_create();
  list_filter(notNegative, intList1, intList0);
  // printf("%s\n", "--------- intList1 ---------");
  // list_foreach(printInt, intList1);
  // printf("----------------------------\n");

  struct list* truncList = list_create();
  list_map2(substring, truncList, intList1, strList1);
  list_foreach(printStr, truncList);

  int sum = 0;
  int* sump = &sum;
  list_foldl(add, (element_t*) &sump, intList1);

  char* s = malloc(sum + truncList->len + 1);
  list_foldl(joinStr, (element_t*) &s, truncList);
  printf("%s\n", s);

  int m = 0;
  int* mp = &m;
  list_foldl(max, (element_t*) &mp, intList1);
  printf("%d\n", m);

  list_foreach(free, intList0);
  list_foreach(free, strList0);
  list_foreach(free, truncList);
  free(s);
  list_destroy(list);
  list_destroy(intList0);
  list_destroy(strList0);
  list_destroy(strList1);
  list_destroy(intList1);
  list_destroy(truncList);
  return 0;
}
