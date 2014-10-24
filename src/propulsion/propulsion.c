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
#include "propulsion.h"
#include "log.h"
#include "timer.h"
#include "move.h"


volatile Action *propulsion_action_handler;
volatile bool propulsion_timer_expired;
bool propulsion_set_timer;




/**************************************

          PRIVATE FUNCTIONS

**************************************/

void propulsion_handle_state(Action *propulsion, ACTION_STATE ret)
{
    if(propulsion_timer_expired == false)
    {
        propulsion->state = ret;

        if(propulsion->state == DONE)
        {
            timer1_reset();
            propulsion_timer_expired = false;
            propulsion_set_timer = false;
        }

    }else
    {
        propulsion->state = FAILED;
        timer1_reset();
        propulsion_timer_expired = false;
        propulsion_set_timer = false;
    }
}


void propulsion_callback_timer(int signal)
{
    LOG(LOG_DEBUG, "Propulsion Timer Expired");
    if (propulsion_action_handler->failed_handler != NULL)
    {
        propulsion_action_handler->failed_handler();
    }
    propulsion_action_handler->action = NULL;
    propulsion_action_handler->failed_handler = NULL;
    propulsion_timer_expired = true;
}


void propulsion_management(Action *propulsion)
{
    ACTION_STATE ret;

    if (propulsion_set_timer == false)
    {
        propulsion_action_handler = propulsion;
        timer1_set(propulsion->timeout);
        propulsion_set_timer = true;
    }

    if (propulsion->action != NULL)
    {
        ret = propulsion->action(propulsion->data);
    }
    propulsion_handle_state(propulsion, ret);
}




/**************************************

          PUBLIC FUNCTIONS

**************************************/

void propulsion_init(void)
{
    LOG(LOG_INFO, "Init Propulsion");
    propulsion_timer_expired = false;
    propulsion_set_timer == false;
    timer1_add(propulsion_callback_timer);
}


void propulsion_loop(File *actions)
{
    Action *propulsion = actions->first;
    if (propulsion == NULL)
    {
        return;
    }

    LOG(LOG_INFO, "Loop Propulsion");

    if (propulsion->type == PROPULSION  || (propulsion->priority > NO))
    {

        if (propulsion->type == PROPULSION && (propulsion->state == INIT || propulsion->state == ONGOING))
        {
            propulsion_management(propulsion);
        }

    }else
    {

        propulsion = propulsion->next;
        if (propulsion == NULL)
        {
            return;
        }

        if (propulsion->type == PROPULSION && (propulsion->state == INIT || propulsion->state == ONGOING))
        {

            propulsion_management(propulsion);

        }

    }
}
