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
