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
