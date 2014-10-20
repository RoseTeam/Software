#include "avoid.h"
#include "log.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>




/**************************************

          PRIVATE FUNCTIONS

**************************************/

void *avoid_thread(void *data)
{

    while(1)
    {

	LOG(LOG_DEBUG, "Loop Avoid");
	sleep(2);

    }

}




/**************************************

          PUBLIC FUNCTIONS

**************************************/

void avoid_init(void)
{
    LOG(LOG_INFO, "Init Avoid");

    pthread_t avoid;

    if (pthread_create(&avoid, NULL, avoid_thread, NULL) != 0) {
	fprintf(stderr, "Error in avoid thread\n");
	exit(EXIT_FAILURE);
    }
}
