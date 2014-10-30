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

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

typedef enum
{
    LOG_INFO,
    LOG_ERROR,
    LOG_DEBUG
}LOG_LEVEL;

void log_init(void);

void log_print(LOG_LEVEL lvl, char* file, char* func, int line, char *fmt, ... );

#if LOG_ENABLE
#define LOG(LVL,MSG,...) do {						\
	log_print(LVL, __FILE__, __func__, __LINE__, MSG, ##__VA_ARGS__); \
    } while(0)
#else
#define LOG(LVL,MSG,...) do {						\
    } while(0)
#endif


#endif
