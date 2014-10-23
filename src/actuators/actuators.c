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
#include "actuators.h"
#include "log.h"
#include "timer.h"
#include "arm.h"


volatile Action *actuators_action_handler;
volatile bool actuators_timer_expired;
bool actuators_set_timer;




/**************************************

          PRIVATE FUNCTIONS

**************************************/

void actuators_handle_state(Action *actuators, ACTION_STATE ret)
{
    if(actuators_timer_expired == false)
    {
        actuators->state = ret;

        if(actuators->state == DONE)
        {
            timer2_reset();
            actuators_timer_expired = false;
            actuators_set_timer = false;
        }

    }
    else
    {

        actuators->state = FAILED;
        timer2_reset();
        actuators_timer_expired = false;
        actuators_set_timer = false;

    }
}


void actuators_callback_timer(int signal)
{
    LOG(LOG_DEBUG, "Actuators Timer Expired");
    if(actuators_action_handler->failed_handler != NULL)
    {
        actuators_action_handler->failed_handler();
    }
    actuators_action_handler->action = NULL;
    actuators_action_handler->failed_handler = NULL;
    actuators_timer_expired = true;
}


void actuators_management(Action *actuators)
{
    ACTION_STATE ret;

    if(actuators_set_timer == false)
    {
        actuators_action_handler = actuators;
        timer2_set(actuators->timeout);
        actuators_set_timer = true;
    }

    if(actuators->action != NULL)
    {
        ret = actuators->action(actuators->data);
    }
    actuators_handle_state(actuators, ret);
}




/**************************************

          PUBLIC FUNCTIONS

**************************************/

void actuators_init(void)
{
    LOG(LOG_INFO, "Init Actuators");
    actuators_timer_expired = false;
    actuators_set_timer == false;
    timer2_add(actuators_callback_timer);
}


void actuators_loop(File *actions)
{
    Action *actuators = actions->first;
    if(actuators == NULL)
    {
        return;
    }

    LOG(LOG_INFO, "Loop Actuators");

    if(actuators->type == ACTUATORS || (actuators->priority > NO))
    {

        if(actuators->type == ACTUATORS && (actuators->state == INIT || actuators->state == ONGOING))
        {
            actuators_management(actuators);
        }

    }else{

        actuators = actuators->next;
        if(actuators == NULL)
        {
            return;
        }

        if(actuators->type == ACTUATORS && (actuators->state == INIT || actuators->state == ONGOING))
        {

            actuators_management(actuators);

        }

    }
}
