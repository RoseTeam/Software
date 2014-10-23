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
