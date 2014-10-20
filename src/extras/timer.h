#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>

void timer1_add(void *callback);

void timer1_set(int seconds);

void timer1_reset(void);

void timer2_add(void *callback);

void timer2_set(int seconds);

void timer2_reset(void);


#endif
