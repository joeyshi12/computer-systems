#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "list.h"

void printStr (element_t ev) {
  char* e = (char*) ev;
  if (e == NULL) {
    printf("%s\n", "NULL");
  } else {
    printf ("%s\n", e);
  }
}

void printInt (element_t ev) {
  int* e = (int*) ev;
  printf ("%d\n", *e);
}

void strToInt (element_t* rv, element_t av) {
  char* a = (char*) av;
  int** r = (int**) rv;
  if (*r == NULL) {
    *r = malloc(sizeof(int));
  }
  char* endp;
  **r = strtol(a, &endp, 0);
  if(*endp) {
    **r = -1;
  }
}

void replaceIntWithNULL (element_t* rv, element_t av, element_t bv) {
  char* a = (char*) av;
  int* b = (int*) bv;
  char** r = (char**) rv;
  if (*b == -1) {
    if (*r == NULL) {
      *r = malloc(strlen(a) + 1);
    }
    strcpy(*r, a);
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
  char* b = (char*) bv;
  char** r = (char**) rv;
  *r = strdup(b);
  if (*a < strlen(*r)) {
    (*r)[*a] = 0;
  }
}

void joinStr (element_t* rv, element_t av, element_t bv) {
  char* b = (char*) bv;
  char** r = (char**) rv;
  *r = realloc(*r, strlen(*r) + strlen(b) + 2);
  if (strlen(*r)) {
    strcat(*r, " ");
  }
  strcat(*r, b);
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
  for (int i=1; i<argc; i++) {
    list_append(list, (element_t) argv[i]);
  }

  struct list* intList0 = list_create();
  list_map1(strToInt, intList0, list);

  struct list* strList0 = list_create();
  list_map2(replaceIntWithNULL, strList0, list, intList0);

  struct list* strList1 = list_create();
  list_filter(notNull, strList1, strList0);

  struct list* intList1 = list_create();
  list_filter(notNegative, intList1, intList0);

  struct list* truncList = list_create();
  list_map2(substring, truncList, intList1, strList1);
  list_foreach(printStr, truncList);

  char* s = malloc(1);
  *s = 0;
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
