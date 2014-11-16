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

#include "avoid.h"
#include "log.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>




/**************************************

          PRIVATE FUNCTIONS

**************************************/

void avoid_thread(void *data)
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
