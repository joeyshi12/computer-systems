#include <stdlib.h>
#include <stdio.h>

#include "mymalloc.h"

/* Feel free to change/remove these variables and/or add your own. */

// Base address of the allocated heap.
static char *heap;
// Size of the complete heap.
static int heapsize;
// Next unallocated location in the heap.
static int top;

struct Node {
  int size;
  struct Node* next;
};

static struct Node* head;

/* Initialize your memory manager to manage the given heap. */
void mymalloc_init(char *_heap, int _heapsize) {
  heap = _heap;
  heapsize = _heapsize;
  top = 0;
  /* TODO: Any other initialization you want to do */
  /* NOTE! Each call to mymalloc_init should initialize a brand new heap
     (discarding the old one), as it will be called once per test.
     Therefore, be sure to *initialize all your variables* here! */
  head = NULL;
}



/* Allocate a block of memory of the given size, or NULL if unable.

Returns: a pointer aligned to 8 bytes, or NULL if allocation failed. */
void *mymalloc(int size) {
  /* This is a dumb implementation of malloc, adapted slightly from the slides.

  You will augment this solution to make a real malloc. */

  /* Round `size` up to a multiple of 8 bytes */
  /* TODO: Make room for any extra metadata you want to store */
  size = (size + 7) / 8 * 8 + 16;

  /* TODO: Implement and walk a free list before allocating off `top` as a last resort */
  struct Node* prev = NULL;
  for (struct Node* curr = head; curr != NULL; curr = curr->next) {
    if (curr->size >= size - 16) {
      if (prev != NULL) {
        prev->next = curr->next;
      } else {
        head = curr->next;
      }
      return (void*) (((int*) curr) + 4);
    }
    prev = curr;
  }

  if (size < 0 || size > heapsize || heapsize - size < top) {
    /* There is not enough room in the heap - fail */
    return NULL;
  }

  /* Allocate the memory from `top` to `top+size` and return it */
  void *res = &heap[top];
  top += size;
  struct Node* newBlock = (struct Node*) res;
  newBlock->size = size - 16;
  return (void*) ((int*) res + 4);
}

/* Free the given block of memory. */
void myfree(void *ptr) {
  /* TODO: Our dumb implementation does not allow for any freeing. Implement me! */
  struct Node* node = (struct Node*) ((int*) ptr - 4);
  struct Node* temp = head;
  head = node;
  head->next = temp;
}
