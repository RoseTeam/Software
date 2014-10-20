#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "timer.h"
#include "log.h"


timer_t timer1;
struct itimerspec itimer1;
timer_t timer2;
struct itimerspec itimer2;




/**************************************

          PUBLIC FUNCTIONS

**************************************/

void timer1_add(void *callback)
{
	struct sigevent event;

	signal(SIGUSR1, callback);

	event.sigev_notify = SIGEV_SIGNAL;
	event.sigev_signo  = SIGUSR1;

	if (timer_create(CLOCK_REALTIME, & event, & timer1) != 0) {
		perror("timer_create");
		exit(EXIT_FAILURE);
	}
}


void timer1_set(int seconds)
{
	itimer1.it_value.tv_sec = seconds;
	itimer1.it_value.tv_nsec = 0;
	itimer1.it_interval.tv_sec = seconds;
	itimer1.it_interval.tv_nsec = 0;

	if (timer_settime(timer1, 0, & itimer1, NULL) != 0) {
		perror("timer_settime");
		exit(EXIT_FAILURE);
	}
}


void timer1_reset(void)
{
	itimer1.it_value.tv_sec = 0;
	itimer1.it_value.tv_nsec = 0;
	itimer1.it_interval.tv_sec = 0;
	itimer1.it_interval.tv_nsec = 0;
	if (timer_settime(timer1, 0, & itimer1, NULL) != 0) {
		perror("timer_settime");
		exit(EXIT_FAILURE);
	}
}


void timer2_add(void *callback)
{
	struct sigevent event;

	signal(SIGUSR2, callback);

	event.sigev_notify = SIGEV_SIGNAL;
	event.sigev_signo  = SIGUSR2;

	if (timer_create(CLOCK_REALTIME, & event, & timer2) != 0) {
		perror("timer_create");
		exit(EXIT_FAILURE);
	}
}


void timer2_set(int seconds)
{
	itimer2.it_value.tv_sec = seconds;
	itimer2.it_value.tv_nsec = 0;
	itimer2.it_interval.tv_sec = seconds;
	itimer2.it_interval.tv_nsec = 0;

	if (timer_settime(timer2, 0, & itimer2, NULL) != 0) {
		perror("timer_settime");
		exit(EXIT_FAILURE);
	}
}


void timer2_reset(void)
{
	itimer2.it_value.tv_sec = 0;
	itimer2.it_value.tv_nsec = 0;
	itimer2.it_interval.tv_sec = 0;
	itimer2.it_interval.tv_nsec = 0;
	if (timer_settime(timer2, 0, & itimer2, NULL) != 0) {
		perror("timer_settime");
		exit(EXIT_FAILURE);
	}
}
