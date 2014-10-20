#include <stdio.h>
#include <stdlib.h>
#include "move.h"
#include "log.h"




/**************************************

          PUBLIC FUNCTIONS

**************************************/

ACTION_STATE __propulsion_move(Propulsion_Parameters *parameters)
{
    static int test = 1;

    LOG(LOG_DEBUG, "Propulsion MOVE: %d", parameters->speed);

    if(test == 0)
    {
        return DONE;
    }
    else
    {
        test++;
        return ONGOING;
    }
}


void __propulsion_move_failed_handler(void)
{

    LOG(LOG_ERROR, "MOVE FAILED");

}
