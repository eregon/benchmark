#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "benchmark.h"

#define N 100
#define SIZE_1 0x1000   //  4 KiB
#define SIZE_2 0x10000  // 64 KiB
#define SIZE_3 0x100000 //  1 MiB

char stack_1 () {
  char s[SIZE_1];
  s[SIZE_1 / 2] = 0;
  return s[0];
}

char stack_2 () {
  char s[SIZE_2];
  s[SIZE_2 / 2] = 0;
  return s[0];
}

char stack_3 () {
  char s[SIZE_3];
  s[SIZE_3 / 2] = 0;
  return s[0];
}

char heap_1 () {
  char *s = (char *) malloc(sizeof(char) * SIZE_1);
  s[SIZE_1 / 2] = 0;
  return s[0];
  //free(s);
}

char heap_2 () {
  char *s = (char *) malloc(sizeof(char) * SIZE_2);
  s[SIZE_2 / 2] = 0;
  return s[0];
  //free(s);
}

char heap_3 () {
  char *s = (char *) malloc(sizeof(char) * SIZE_3);
  s[SIZE_3 / 2] = 0;
  return s[0];
  //free(s);
}

void benchmark_fun (timer *t, char (*fun) (), recorder *rec, int x) {
  int i;
  start_timer(t);
  for (i = 0; i < N; i++) {
    char c = fun();
  }
  write_record_n(rec, x, stop_timer(t), N);
}

int main (int argc, char *argv[]) {
  timer *t = timer_alloc();

  // brk/sbrk
  recorder *stack_rec = recorder_alloc("stack.csv");
  recorder *heap_rec = recorder_alloc("heap.csv");

  benchmark_fun(t, heap_1, heap_rec, SIZE_1);
  benchmark_fun(t, heap_2, heap_rec, SIZE_2);
  benchmark_fun(t, heap_3, heap_rec, SIZE_3);

  benchmark_fun(t, stack_1, stack_rec, SIZE_1);
  benchmark_fun(t, stack_2, stack_rec, SIZE_2);
  benchmark_fun(t, stack_3, stack_rec, SIZE_3);

  printf("%p\n", sbrk(0));

  recorder_free(stack_rec);
  recorder_free(heap_rec);

  timer_free(t);
  return EXIT_SUCCESS;
}
