#include <stdint.h>
#include <time.h>
#include "../yield.h"

#define FRAMETIME_NS 16666667

struct timespec last_timespec, elapsed_timespec, duration_timespec;

struct timespec diff_timespec(const struct timespec *time1,
    const struct timespec *time0) {
  struct timespec diff = {.tv_sec = time1->tv_sec - time0->tv_sec, //
      .tv_nsec = time1->tv_nsec - time0->tv_nsec};
  if (diff.tv_nsec < 0) {
    diff.tv_nsec += 1000000000; // nsec/sec
    diff.tv_sec--;
  }
  return diff;
}

void timer_init(void){
    clock_gettime(CLOCK_MONOTONIC, &last_timespec);
}

void yield_frame(void){
    clock_gettime(CLOCK_MONOTONIC, &elapsed_timespec);
    duration_timespec = diff_timespec(&elapsed_timespec, &last_timespec);
    if (duration_timespec.tv_sec == 0) {
        duration_timespec.tv_nsec = FRAMETIME_NS - duration_timespec.tv_nsec;
        if (duration_timespec.tv_nsec < 0) duration_timespec.tv_nsec = 0;
    }
    else duration_timespec = (struct timespec){(time_t)0, (long)0};
    nanosleep(&duration_timespec, NULL);
    clock_gettime(CLOCK_MONOTONIC, &last_timespec);
}
