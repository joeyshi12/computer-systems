#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"
uthread_mutex_t mutex;
uthread_cond_t cond;
uthread_cond_t cond2;
int flag = 0;

void randomStall() {
  int i, r = random() >> 20;
  while (i++<r);
}

void a() {
  randomStall();
  printf("%s\n", "a");
}

void b() {
  randomStall();
  printf("%s\n", "b");
}

void c() {
   randomStall();
  printf("%s\n", "c");
}

void d() {
   randomStall();
  printf("%s\n", "d");
}

void* t1 (void* av) {
  a();
  uthread_mutex_lock(mutex);
  b();
  uthread_cond_wait(cond);
  uthread_mutex_unlock(mutex);
  return NULL;

}

void* t2 (void* av) {
  uthread_mutex_lock(mutex);
  c();
  uthread_cond_signal(cond);
  d();
  uthread_mutex_unlock(mutex);
  return NULL;
}


int main(int argc, char const *argv[]) {
  uthread_init (3);

  mutex = uthread_mutex_create();
  cond = uthread_cond_create (mutex);
  cond2 = uthread_cond_create (mutex);

  uthread_t ttwo = uthread_create(t2, NULL);
  uthread_t tone = uthread_create(t1, NULL);


  uthread_join(tone, NULL);
  uthread_join(ttwo, NULL);



  return 0;
}
