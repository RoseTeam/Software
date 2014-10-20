#include <stdio.h>
#include <stdlib.h>
#include "arm.h"
#include "log.h"




/**************************************

          PUBLIC FUNCTIONS

**************************************/

ACTION_STATE __actuators_arm(Actuators_Parameters *parameters)
{
    static int test = 1;

    LOG(LOG_DEBUG, "Actuators ARM: %d", parameters->sensors);

    if(test == 40)
    {
        return DONE;
    }
    else
    {
        test++;
        return ONGOING;
    }
}


void __actuators_arm_failed_handler(void)
{

    LOG(LOG_ERROR, "ARM FAILED");

}
