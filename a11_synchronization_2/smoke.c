#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_ITERATIONS 1000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t  match;
  uthread_cond_t  paper;
  uthread_cond_t  tobacco;
  uthread_cond_t  smoke;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->paper   = uthread_cond_create (agent->mutex);
  agent->match   = uthread_cond_create (agent->mutex);
  agent->tobacco = uthread_cond_create (agent->mutex);
  agent->smoke   = uthread_cond_create (agent->mutex);
  return agent;
}

//
// TODO
// You will probably need to add some procedures and struct etc.
//

int hasMatch;
int hasPaper;
int hasTobacco;
int countDown = NUM_ITERATIONS;

uthread_cond_t paperAndTobacco;
uthread_cond_t matchAndTobacco;
uthread_cond_t matchAndPaper;

/**
 * You might find these declarations helpful.
 *   Note that Resource enum had values 1, 2 and 4 so you can combine resources;
 *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
 */
enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

int signal_count [5];  // # of times resource signalled
int smoke_count  [5];  // # of times smoker with resource smoked

/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can re-write it if you like, but be sure that all it does
 * is choose 2 random reasources, signal their condition variables, and then wait
 * wait for a smoker to smoke.
 */
void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};

  uthread_mutex_lock (a->mutex);
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      int r = random() % 3;
      signal_count [matching_smoker [r]] ++;
      int c = choices [r];
      if (c & MATCH) {
        VERBOSE_PRINT ("match available\n");
        uthread_cond_signal (a->match);
      }
      if (c & PAPER) {
        VERBOSE_PRINT ("paper available\n");
        uthread_cond_signal (a->paper);
      }
      if (c & TOBACCO) {
        VERBOSE_PRINT ("tobacco available\n");
        uthread_cond_signal (a->tobacco);
      }
      VERBOSE_PRINT ("agent is waiting for smoker to smoke\n");
      uthread_cond_wait (a->smoke);
    }
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

void* match(void* av) {
  struct Agent* a = av;
  uthread_mutex_lock (a->mutex);
  while (countDown > 0) {
    uthread_cond_wait(a->match);
    hasMatch = 1;
    if (hasPaper) {
      uthread_cond_signal(matchAndPaper);
    }
    if (hasTobacco) {
      uthread_cond_signal(matchAndTobacco);
    }
  }
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

void* paper(void* av) {
  struct Agent* a = av;
  uthread_mutex_lock (a->mutex);
  while (countDown > 0) {
    uthread_cond_wait(a->paper);
    hasPaper = 1;
    if (hasMatch) {
      uthread_cond_signal(matchAndPaper);
    }
    if (hasTobacco) {
      uthread_cond_signal(paperAndTobacco);
    }
  }
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

void* tobacco(void* av) {
  struct Agent* a = av;
  uthread_mutex_lock (a->mutex);
  while (countDown > 0) {
    uthread_cond_wait(a->tobacco);
    hasTobacco = 1;
    if (hasMatch) {
      uthread_cond_signal(matchAndTobacco);
    }
    if (hasPaper) {
      uthread_cond_signal(paperAndTobacco);
    }
  }
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

void* match_smoker(void* av) {
  struct Agent* a = av;
  uthread_mutex_lock (a->mutex);
  while (countDown > 0) {
    while (!(hasPaper && hasTobacco)) {
      uthread_cond_wait(paperAndTobacco);
    }
    smoke_count [MATCH]++;
    countDown--;
    hasPaper = 0;
    hasTobacco = 0;
    uthread_cond_signal (a->smoke);
  }
  uthread_mutex_unlock (a->mutex);
}

void* paper_smoker(void* av) {
  struct Agent* a = av;
  uthread_mutex_lock (a->mutex);
  while (countDown > 0) {
    while (!(hasMatch && hasTobacco)) {
      uthread_cond_wait(matchAndTobacco);
    };
    smoke_count [PAPER]++;
    countDown--;
    hasMatch = 0;
    hasTobacco = 0;
    uthread_cond_signal (a->smoke);
  }
  uthread_mutex_unlock (a->mutex);
}

void* tobacco_smoker(void* av) {
  struct Agent* a = av;
  uthread_mutex_lock (a->mutex);
  while (countDown > 0) {
    while (!(hasMatch && hasPaper)) {
      uthread_cond_wait(matchAndPaper);
    }
    smoke_count [TOBACCO]++;
    countDown--;
    hasMatch = 0;
    hasPaper = 0;
    uthread_cond_signal (a->smoke);
  }
  uthread_mutex_unlock (a->mutex);
}

int main (int argc, char** argv) {
  uthread_init (7);
  struct Agent*  a = createAgent();
  paperAndTobacco = uthread_cond_create(a->mutex);
  matchAndTobacco = uthread_cond_create(a->mutex);
  matchAndPaper = uthread_cond_create(a->mutex);
  // TODO
  uthread_t t0 = uthread_create (match, a);
  uthread_t t1 = uthread_create (paper, a);
  uthread_t t2 = uthread_create (tobacco, a);
  uthread_t t3 = uthread_create (match_smoker, a);
  uthread_t t4 = uthread_create (paper_smoker, a);
  uthread_t t5 = uthread_create (tobacco_smoker, a);
  uthread_t t6 = uthread_create (agent, a);
  uthread_join (t6, 0);
  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);
  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);
}
