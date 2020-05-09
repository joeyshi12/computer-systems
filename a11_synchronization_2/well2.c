#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

#define MAX_OCCUPANCY      3
#define NUM_ITERATIONS     100
#define NUM_PEOPLE         20
#define FAIR_WAITING_COUNT 4

/**
 * You might find these declarations useful.
 */
enum Endianness {LITTLE = 0, BIG = 1};
const static enum Endianness oppositeEnd [] = {BIG, LITTLE};

struct Well {
  // TODO
  uthread_mutex_t mutex;
  uthread_cond_t littleCanEnter;
  uthread_cond_t bigCanEnter;
};

struct Well* createWell() {
  struct Well* well = malloc (sizeof (struct Well));
  // TODO
  well->mutex = uthread_mutex_create();
  well->littleCanEnter = uthread_cond_create(well->mutex);
  well->bigCanEnter = uthread_cond_create(well->mutex);
  return well;
}

struct Well* well;

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram         [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_mutex_t waitingHistogrammutex;
int             occupancyHistogram       [2] [MAX_OCCUPANCY + 1];

int littleEndianUsingWell = 0;
int bigEndianUsingWell = 0;
int wellPopulation = 0;

void recordWaitingTime (int waitingTime) {
  uthread_mutex_lock (waitingHistogrammutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_mutex_unlock (waitingHistogrammutex);
}

void leaveWell() {
  // TODO
  // uthread_mutex_lock (well->mutex);

  wellPopulation--;
  if (wellPopulation == 0) {
    // printf("%s\n", "reached zero");
    // if (rand() % 2 == 1) {
      bigEndianUsingWell = !bigEndianUsingWell;
      littleEndianUsingWell = !littleEndianUsingWell;
    // }
  }
  printf("%d\n", littleEndianUsingWell);
  printf("%d\n", bigEndianUsingWell);
  if (littleEndianUsingWell) {
    uthread_cond_signal (well->littleCanEnter);
  } else {
    uthread_cond_signal (well->bigCanEnter);
  }
  // uthread_mutex_unlock (well->mutex);

  for (int i = 0; i < NUM_PEOPLE; i++) {
    uthread_yield();
  }
  // uthread_mutex_unlock (well->mutex);
}

void enterWell (enum Endianness g, int start) {
  // TODO
  // uthread_mutex_lock (well->mutex);
  recordWaitingTime(entryTicker - start);
  entryTicker++;
  wellPopulation++;
  occupancyHistogram[g][wellPopulation]++;

  for (int i = 0; i < NUM_PEOPLE; i++) {
    uthread_yield();
  }
  leaveWell();
  // uthread_mutex_unlock (well->mutex);
}

//
// TODO
// You will probably need to create some additional procedures etc.
//

void* littleEndian(void* pv) {
    int start = entryTicker;
    // uthread_mutex_lock (well->mutex);
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      uthread_mutex_lock (well->mutex);

      while (bigEndianUsingWell || wellPopulation >= MAX_OCCUPANCY) {
        // printf("Little %d waiting\n", i);
        uthread_cond_wait(well->littleCanEnter);
      }
      // printf("%s\n", "littleEndian entering well");
      littleEndianUsingWell = 1;
      // uthread_mutex_unlock (well->mutex);

      enterWell(LITTLE, start);
      uthread_mutex_unlock (well->mutex);
    }
    // uthread_mutex_unlock (well->mutex);
  return NULL;
}

void* bigEndian(void* pv) {
  int start = entryTicker;
  // uthread_mutex_lock (well->mutex);
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      uthread_mutex_lock (well->mutex);
      while (littleEndianUsingWell || wellPopulation >= MAX_OCCUPANCY) {
        // printf("Big %d waiting\n", i);
        uthread_cond_wait(well->bigCanEnter);
      }

      // printf("Big %d entering well\n", i);
      bigEndianUsingWell = 1;
      // uthread_mutex_unlock (well->mutex);

      enterWell(BIG, start);
      uthread_mutex_unlock (well->mutex);
    }
  // uthread_mutex_unlock (well->mutex);
  return NULL;
}

void* person(void* ev) {
  int e = *(int*) ev;
  int start = entryTicker;
  if (e) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      // uthread_mutex_lock (well->mutex);
      uthread_mutex_lock (well->mutex);

      while (littleEndianUsingWell || wellPopulation >= MAX_OCCUPANCY) {
        // printf("Big %d waiting\n", i);
        uthread_cond_wait(well->bigCanEnter);
      }
      // printf("Big %d entering well\n", i);
      bigEndianUsingWell = 1;
      uthread_mutex_unlock (well->mutex);

      enterWell(BIG, start);
      // uthread_mutex_unlock (well->mutex);

    }
  } else {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      // uthread_mutex_lock (well->mutex);
      uthread_mutex_lock (well->mutex);

      while (bigEndianUsingWell || wellPopulation >= MAX_OCCUPANCY) {
        // printf("Little %d waiting\n", i);
        uthread_cond_wait(well->littleCanEnter);
      }
      // printf("%s\n", "littleEndian entering well");
      littleEndianUsingWell = 1;
      uthread_mutex_unlock (well->mutex);

      enterWell(LITTLE, start);
      // uthread_mutex_unlock (well->mutex);

    }
  }
  return NULL;
}

int main (int argc, char** argv) {
  uthread_init (1);
  well = createWell();
  uthread_t pt [NUM_PEOPLE];
  waitingHistogrammutex = uthread_mutex_create ();

  // TODO
  for (int i = 0; i < NUM_PEOPLE; i++) {
    if (rand() % 2) {
      pt[i] = uthread_create (bigEndian, NULL);
    } else {
      pt[i] = uthread_create (littleEndian, NULL);
    }
  }
  // int* e[NUM_PEOPLE];
  // for (int i = 0; i < NUM_PEOPLE; i++) {
  //   e[i] = malloc(sizeof(int));
  //   *(e[i]) = random() % 2;
  //   pt[i] = uthread_create (person, e[i]);
  // }
  for (int i = 0; i < NUM_PEOPLE; i++) {
    uthread_join (pt[i], 0);
  }
  printf ("Times with 1 little endian %d\n", occupancyHistogram [LITTLE]   [1]);
  printf ("Times with 2 little endian %d\n", occupancyHistogram [LITTLE]   [2]);
  printf ("Times with 3 little endian %d\n", occupancyHistogram [LITTLE]   [3]);
  printf ("Times with 1 big endian    %d\n", occupancyHistogram [BIG] [1]);
  printf ("Times with 2 big endian    %d\n", occupancyHistogram [BIG] [2]);
  printf ("Times with 3 big endian    %d\n", occupancyHistogram [BIG] [3]);
  printf ("Waiting Histogram\n");
  for (int i=0; i<WAITING_HISTOGRAM_SIZE; i++)
    if (waitingHistogram [i])
      printf ("  Number of times people waited for %d %s to enter: %d\n", i, i==1?"person":"people", waitingHistogram [i]);
  if (waitingHistogramOverflow)
    printf ("  Number of times people waited more than %d entries: %d\n", WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);

  // for (int i = 0; i < NUM_PEOPLE; i++) {
  //   free(e[i]);
  // }
}
