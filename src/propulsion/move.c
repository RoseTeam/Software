/******************************************************************************
 **  Copyright (c) 2014, RoseTeam Software. All Rights Reserved.
 **
 **  This file is part of RoseTeam Software.
 **
 **  RoseTeam Software is free software; you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation; either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  RoseTeam Software is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 **
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "move.h"
#include "log.h"
#include "timer.h"




/**************************************

          PUBLIC FUNCTIONS

**************************************/

ACTION_STATE __propulsion_move(Propulsion_Move *parameters)
{
    int test = 0;

    LOG(LOG_DEBUG, "Propulsion MOVE: %d, %d, %d, %d", parameters->speed, parameters->angle, parameters->x, parameters->y);

    srand (time(NULL));

    /* value between 1 and 100 */
    test = (rand() % 100) + 1;
    LOG(LOG_DEBUG, "Rand Value: %d", test);

    if (test < 10)
    {
        parameters->x = 6;
        return DONE;
    }
    else if (test > 95)
    {
        parameters->x = 4;
        return FAILED;
    }
    else
    {
        parameters->x = 5;
        return ONGOING;
    }

}


void __propulsion_move_failed_handler(void)
{

    LOG(LOG_ERROR, "MOVE FAILED");

}
