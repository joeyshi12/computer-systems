#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "str_element.h"
#include "refcount.h"

/* TODO: Implement all public str_element functions, including element interface functions.

You may add your own private functions here too. */

struct str_element {
  struct str_element_class *class;
  char* value;
  int type;
};

struct str_element_class {
  void (*print)(struct element *);
  int (*compare)(struct element *, struct element *);
  char* (*str_element_get_value)(struct str_element*);
  int (*is_str_element)(struct element *);
};

/* Print this element (without any trailing newline) */
void str_print(struct element* thisv) {
  struct str_element* e = (struct str_element*) thisv;
  printf("%s", e->class->str_element_get_value(e));
}

/* Compare two elements. int_element should always compare LESS than str_element.
 * The compare function should return:
 *   a number < 0 if the first element is less than the second element,
 *   0 if the elements compare equal,
 *   a number > 0 otherwise.
 */
int str_compare(struct element* thisv, struct element* other) {
  if (!is_str_element(other)) {
    return 1;
  }
  struct str_element* se1 = (struct str_element*) thisv;
  struct str_element* se2 = (struct str_element*) other;
  char* a = se1->value;
  char* b = se2->value;
  return strcmp(a, b);
}

/* Static function that obtains the value held in an int_element. */
char *str_element_get_value(struct str_element* thisv) {
  return thisv->value;
}

/* Static function that determines whether this is an int_element. */
int is_str_element(struct element* thisv) {
  struct str_element* se = (struct str_element*) thisv;
  return se->type == 1;
}

struct str_element_class str_element_class_table = {
  str_print,
  str_compare,
  str_element_get_value,
  is_str_element
};

void str_element_finalizer(void* thisv) {
  struct str_element* se = (struct str_element*) thisv;
  free(se->value);
}

/* Static constructor that creates new integer elements. */
struct str_element *str_element_new(char* value) {
  struct str_element *thisv = rc_malloc(sizeof(*thisv) + 1, str_element_finalizer);
  thisv->class = &str_element_class_table;
  thisv->value = malloc(strlen(value) + 1);
  strcpy(thisv->value, value);
  thisv->type = 1;
  return thisv;
}
