#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "uthread.h"
#include "queue.h"
#include "disk.h"

queue_t pending_read_queue;
volatile unsigned int next_blockno;
int done;

void interrupt_service_routine() {
  // TODO
  void* val;
  void* countv;
  void (*callback)(void*,void*);
  queue_dequeue (pending_read_queue, &val, &countv, &callback);
  callback (val, countv);
}

void handleOtherReads(void *resultv, void *countv) {
  // TODO
  next_blockno = *(int*) resultv;
  int* count = (int*) countv;
  if (count > 0) {
    (*count)--;
    int result;
    queue_enqueue(pending_read_queue, &result, count, handleOtherReads);
    disk_schedule_read(&result, next_blockno);
  }
}

void handleFirstRead(void *resultv, void *countv) {
  // TODO
  next_blockno = *(int*) resultv;
  int count = *(int*) resultv;
  int result;
  queue_enqueue(pending_read_queue, &result, &count, handleOtherReads);
  disk_schedule_read(&result, next_blockno);
  while (count > 0);
  done = 1;
}

int main(int argc, char **argv) {
  // Command Line Arguments
  static char* usage = "usage: treasureHunt starting_block_number";
  int starting_block_number;
  char *endptr;
  if (argc == 2)
    starting_block_number = strtol (argv [1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf ("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init (1);
  disk_start (interrupt_service_routine);
  pending_read_queue = queue_create();

  next_blockno = starting_block_number;

  // Start the Hunt
  // TODO

  int result;
  queue_enqueue(pending_read_queue, &result, NULL, handleFirstRead);
  disk_schedule_read (&result, next_blockno);

  while (!done); // infinite loop so that main doesn't return before hunt completes
  printf("%d\n", next_blockno);
}
