#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/errno.h>
#include <assert.h>
#include "queue.h"
#include "disk.h"
#include "uthread.h"

queue_t      pending_read_queue;
unsigned int sum = 0;

void interrupt_service_routine () {
  void* val;
  queue_dequeue (pending_read_queue, &val, NULL, NULL);
  uthread_unblock(*(uthread_t*) val);
}

void *read_block (void *arg) {
  // TODO schedule read and the update (correctly)
  int* buf = (int*) arg;
  int result;
  disk_schedule_read(&result, *buf);
  uthread_block();
  sum += result;
  free(buf);
  return NULL;
}

int main (int argc, char** argv) {

  // Command Line Arguments
  static char* usage = "usage: tRead num_blocks";
  int num_blocks;
  char *endptr;
  if (argc == 2)
    num_blocks = strtol (argv [1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf ("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init (1);
  disk_start (interrupt_service_routine);
  pending_read_queue = queue_create();

  // Sum Blocks
  // TODO

  uthread_t threads[num_blocks];

  for (int blockno = 0; blockno < num_blocks; blockno++) {
    int* buf = malloc(4);
    *buf = blockno;
    threads[blockno] = uthread_create(read_block, (void*) buf);
    queue_enqueue(pending_read_queue, (void*) &threads[blockno], NULL, NULL);
  }

  for (int blockno = 0; blockno < num_blocks; blockno++) {
    uthread_join(threads[blockno], NULL);
  }
  printf("%d\n", sum);
  return 0;
}
