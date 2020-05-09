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
  uthread_mutex_t mutex;
  uthread_cond_t littleLine;
  uthread_cond_t bigLine;
  int population;
  int endianness;
  int littleLineLength;
  int bigLineLength;
};

struct Well* createWell() {
  struct Well* well = malloc (sizeof (struct Well));
  well->mutex = uthread_mutex_create();
  well->littleLine = uthread_cond_create(well->mutex);
  well->bigLine = uthread_cond_create(well->mutex);
  well->population = 0;
  well->endianness = 0;
  well->littleLineLength = 0;
  well->bigLineLength = 0;
  return well;
}

struct Well* well;

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram         [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_mutex_t waitingHistogrammutex;
int             occupancyHistogram       [2] [MAX_OCCUPANCY + 1];

void recordWaitingTime (int waitingTime) {
  uthread_mutex_lock (waitingHistogrammutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_mutex_unlock (waitingHistogrammutex);
}

void enterWell (enum Endianness g, int start) {
    assert(well->population <= MAX_OCCUPANCY);
    assert(well->endianness == g);

    recordWaitingTime(entryTicker - start);
    entryTicker++;
    well->population++;
    occupancyHistogram[g][well->population]++;
}

void leaveWell() {
  uthread_mutex_lock (well->mutex);
  well->population--;
  if (well->population == 0) {
    if (well->endianness == 0) {
      if (well->bigLineLength > FAIR_WAITING_COUNT || well->littleLineLength == 0) {
        well->bigLineLength = 0;
        well->endianness = 1;
        uthread_cond_broadcast (well->bigLine);
      } else {
        well->littleLineLength = 0;
        well->endianness = 0;
        uthread_cond_broadcast (well->littleLine);
      }
    } else {
      if (well->littleLineLength > FAIR_WAITING_COUNT || well->bigLineLength == 0) {
        well->littleLineLength = 0;
        well->endianness = 0;
        uthread_cond_broadcast (well->littleLine);
      } else {
        well->bigLineLength = 0;
        well->endianness = 1;
        uthread_cond_broadcast (well->bigLine);
      }
    }
  }
  uthread_mutex_unlock (well->mutex);
}

//
// TODO
// You will probably need to create some additional procedures etc.
//

void* lilliputian(void* pv) {
  int g = random() % 2;
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    int start = entryTicker;

    uthread_mutex_lock (well->mutex);
      while (well->endianness != g || well->population >= MAX_OCCUPANCY || (well->littleLineLength > FAIR_WAITING_COUNT && well->endianness == 0) || (well->bigLineLength > FAIR_WAITING_COUNT && well->endianness == 1)) {
        if (g == 0) {
          well->littleLineLength++;
          uthread_cond_wait (well->littleLine);
        } else {
          well->bigLineLength++;
          uthread_cond_wait (well->bigLine);
        }
      }
      enterWell(g, start);
    uthread_mutex_unlock (well->mutex);

    for (int i = 0; i < NUM_PEOPLE; i++) {
      uthread_yield();
    }

    leaveWell();

    for (int i = 0; i < NUM_PEOPLE; i++) {
      uthread_yield();
    }
  }
}

int main (int argc, char** argv) {
  uthread_init (1);
  well = createWell();
  uthread_t pt [NUM_PEOPLE];
  waitingHistogrammutex = uthread_mutex_create ();

  for (int i = 0; i < NUM_PEOPLE; i++) {
    pt[i] = uthread_create (lilliputian, 0);
  }

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
}
