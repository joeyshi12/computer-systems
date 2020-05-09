// #include <stdlib.h>
// #include <stdio.h>
// #include <assert.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include "uthread.h"
// #include "uthread_sem.h"
// #include "uthread_mutex_cond.h"
//
// #ifdef VERBOSE
// #define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
// #else
// #define VERBOSE_PRINT(S, ...) ;
// #endif
//
// #define MAX_OCCUPANCY      3
// #define NUM_ITERATIONS     100
// #define NUM_PEOPLE         20
// #define FAIR_WAITING_COUNT 4
//
// /**
//  * You might find these declarations useful.
//  */
// enum Endianness {LITTLE = 0, BIG = 1};
// const static enum Endianness oppositeEnd [] = {BIG, LITTLE};
//
// struct Well {
//   uthread_sem_t mutex;
//   uthread_sem_t lineOne;
//   uthread_sem_t lineTwo;
//   int population;
//   int endianness;
//   int lineupOne;
//   int lineupTwo;
// };
//
// struct Well* createWell() {
//   struct Well* well = malloc (sizeof (struct Well));
//   well->mutex = uthread();
//   well->lineOne = uthread_cond_create(well->mutex);
//   well->lineTwo = uthread_cond_create(well->mutex);
//   well->population = 0;
//   well->endianness = 0;
//   well->lineupOne = 0;
//   well->lineupTwo = 0;
//   return well;
// }
//
// struct Well* well;
//
// #define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
// int             entryTicker;                                          // incremented with each entry
// int             waitingHistogram         [WAITING_HISTOGRAM_SIZE];
// int             waitingHistogramOverflow;
// uthread_sem_t   waitingHistogrammutex;
// int             occupancyHistogram       [2] [MAX_OCCUPANCY + 1];
//
// void recordWaitingTime (int waitingTime) {
//   uthread_sem_wait (waitingHistogrammutex);
//   if (waitingTime < WAITING_HISTOGRAM_SIZE)
//     waitingHistogram [waitingTime] ++;
//   else
//     waitingHistogramOverflow ++;
//   uthread_sem_signal (waitingHistogrammutex);
// }
//
// void enterWell (enum Endianness g, int start) {
//     assert(well->endianness == g);
//     assert(well->population < MAX_OCCUPANCY);
//
//     recordWaitingTime(entryTicker - start);
//     entryTicker++;
//     well->population++;
//     occupancyHistogram[g][well->population]++;
// }
//
// void leaveWell() {
//   uthread_sem_wait (well->mutex);
//   well->population--;
//   if (well->population == 0) {
//     if (well->lineupOne > FAIR_WAITING_COUNT || well->lineupTwo == 0) {
//       well->lineupOne = 0;
//       well->endianness = !well->endianness;
//       for (int i = 0; i < NUM_PEOPLE; i++) {
//         uthread_cond_signal (well->lineOne);
//       }
//     } else {
//       well->lineupTwo = 0;
//       for (int i = 0; i < NUM_PEOPLE; i++) {
//         uthread_cond_signal (well->lineTwo);
//       }
//     }
//   }
//   uthread_sem_signal (well->mutex);
// }
//
// void* lilliputian(void* pv) {
//   int g = random() % 2;
//   for (int i = 0; i < NUM_ITERATIONS; i++) {
//     int start = entryTicker;
//
//     uthread_sem_wait (wellNotFull);
//     uthread_sem_wait (well->mutex);
//     if (well->endianness != g) {
//       well->lineupOne++;
//       uthread_sem_wait (well->lineOne);
//     } else {
//       well->lineupTwo++;
//       uthread_sem_wait (well->lineTwo);
//     }
//     enterWell(g, start);
//     uthread_sem_signal (well->mutex);
//     uthread_sem_signal (wellNotEmpty);
//
//     for (int i = 0; i < NUM_PEOPLE; i++) {
//       uthread_yield();
//     }
//
//     leaveWell();
//
//     for (int i = 0; i < NUM_PEOPLE; i++) {
//       uthread_yield();
//     }
//   }
// }

int main (int argc, char** argv) {
  // uthread_init (1);
  // well = createWell();
  // uthread_t pt [NUM_PEOPLE];
  // waitingHistogrammutex = uthread_sem_create (1);
  //
  // for (int i = 0; i < NUM_PEOPLE; i++) {
  //   pt[i] = uthread_create (lilliputian, 0);
  // }
  //
  // for (int i = 0; i < NUM_PEOPLE; i++) {
  //   uthread_join (pt[i], 0);
  // }
  //
  // printf ("Times with 1 little endian %d\n", occupancyHistogram [LITTLE]   [1]);
  // printf ("Times with 2 little endian %d\n", occupancyHistogram [LITTLE]   [2]);
  // printf ("Times with 3 little endian %d\n", occupancyHistogram [LITTLE]   [3]);
  // printf ("Times with 1 big endian    %d\n", occupancyHistogram [BIG] [1]);
  // printf ("Times with 2 big endian    %d\n", occupancyHistogram [BIG] [2]);
  // printf ("Times with 3 big endian    %d\n", occupancyHistogram [BIG] [3]);
  // printf ("Waiting Histogram\n");
  // for (int i=0; i<WAITING_HISTOGRAM_SIZE; i++)
  //   if (waitingHistogram [i])
  //     printf ("  Number of times people waited for %d %s to enter: %d\n", i, i==1?"person":"people", waitingHistogram [i]);
  // if (waitingHistogramOverflow)
  //   printf ("  Number of times people waited more than %d entries: %d\n", WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}
