#include <stdlib.h>
#include <stdio.h>
#include "int_element.h"
#include "refcount.h"

/* TODO: Implement all public int_element functions, including element interface functions.

You may add your own private functions here too. */

struct int_element {
  struct int_element_class *class;
  int* value;
  int type;   // 0 if int element, 1 if str element
};

struct int_element_class {
  void (*print)(struct element *);
  int (*compare)(struct element *, struct element *);
  int (*int_element_get_value)(struct int_element*);
  int (*is_int_element)(struct element *);
};

/* Print this element (without any trailing newline) */
void int_print(struct element* thisv) {
  struct int_element* e = (struct int_element*) thisv;
  printf("%d", e->class->int_element_get_value(e));
}

/* Compare two elements. int_element should always compare LESS than str_element.
 * The compare function should return:
 *   a number < 0 if the first element is less than the second element,
 *   0 if the elements compare equal,
 *   a number > 0 otherwise.
 */
int int_compare(struct element* thisv, struct element* other) {
  if (!is_int_element(other)) {
    return -1;
  }
  struct int_element* ie1 = (struct int_element*) thisv;
  struct int_element* ie2 = (struct int_element*) other;
  int* a = ie1->value;
  int* b = ie2->value;
  return *a<*b? -1: *a==*b? 0 : 1;
}

/* Static function that obtains the value held in an int_element. */
int int_element_get_value(struct int_element* thisv) {
  return *thisv->value;
}

/* Static function that determines whether this is an int_element. */
int is_int_element(struct element* thisv) {
  struct int_element* ie = (struct int_element*) thisv;
  return ie->type == 0;
}



struct int_element_class int_element_class_table = {
  int_print,
  int_compare,
  int_element_get_value,
  is_int_element
};

void int_element_finalizer(void* thisv) {
  struct int_element* ie = (struct int_element*) thisv;
  free(ie->value);
}

/* Static constructor that creates new integer elements. */
struct int_element *int_element_new(int value) {
  struct int_element *thisv = rc_malloc(sizeof(*thisv), int_element_finalizer);
  thisv->class = &int_element_class_table;
  thisv->value = malloc(sizeof(int));
  *thisv->value = value;
  thisv->type = 0;
  return thisv;
}
