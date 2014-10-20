#include <stdio.h>
#include <stdbool.h>
#include "log.h"
#include "action.h"
#include "actuators.h"
#include "propulsion.h"
#include "strategy.h"
#include "timer.h"
#include "avoid.h"


int main(void)
{

    File *actions = action_init();

    log_init();

    propulsion_init();

    actuators_init();

    strategy_init(actions);

    avoid_init();

    while(1)
    {

        propulsion_loop(actions);

        actuators_loop(actions);

        strategy_loop(actions);

        usleep(10000);

    }

    return 0;

}
