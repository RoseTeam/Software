
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

#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "log.h"


int log_level;




/**************************************

          PRIVATE FUNCTIONS

**************************************/

char* get_date(void)
{
    time_t timer;
    struct tm* tm_info;
    char* date = malloc(25*sizeof(char));

    time(&timer);
    tm_info = localtime(&timer);

    strftime(date, 25, "[%d/%m/%Y %H:%M:%S]", tm_info);

    return date;
}


void log_format(LOG_LEVEL lvl, char* file, char* func, int line, char* msg)
{
    char* color = malloc(strlen(KWHT));
    char* log_type = NULL;

    switch(lvl)
    {

    case LOG_INFO:
        memcpy(color,KBLU,strlen(KBLU));
        log_type = malloc(strlen("LOG_INFO "));
        memcpy(log_type,"LOG_INFO ",strlen("LOG_INFO "));
        break;

    case LOG_ERROR:
        memcpy(color,KRED,strlen(KRED));
        log_type = malloc(strlen("LOG_ERROR"));
        memcpy(log_type,"LOG_ERROR",strlen("LOG_ERROR"));
        break;

    case LOG_DEBUG:
        memcpy(color,KGRN,strlen(KGRN));
        log_type = malloc(strlen("LOG_DEBUG"));
        memcpy(log_type,"LOG_DEBUG",strlen("LOG_DEBUG"));
        break;

    }

    /* print additional info */
    printf("%s%s %s: (%s %s l%d)\t%s\n", color, log_type, get_date(), file, func, line, msg);

    free(color);
    free(log_type);
}




/**************************************

          PUBLIC FUNCTIONS

**************************************/

void log_init(void)
{
    char* env;
    env = getenv ("LOG_LEVEL");
    if (env!=NULL)
        log_level = atoi(env);
    else
        log_level = 0;
}


void log_print(LOG_LEVEL lvl, char* file, char* func, int line, char *fmt, ... )
{
    va_list  list;
    char *s, c;
    int i, j = 0;

    char *msg = malloc(512 * sizeof(char));

    va_start(list, fmt);
    while (*fmt)
    {
        if (*fmt != '%')
            memcpy(msg + j, fmt, 1);
        else
        {
            switch (*++fmt)
            {
            case 's':
                s = va_arg( list, char * );
                sprintf(msg + j, "%s",s);
                j = j + strlen(s) - 1;
                break;

            case 'd':
                i = va_arg( list, int );
                sprintf(msg + j, "%d",i);
                break;

            case 'c':
                c = va_arg( list, int);
                sprintf(msg + j, "%c",c);
                break;

            default:
                memcpy(msg + j, fmt, 1);
                break;
            }
        }
        j++;
        ++fmt;
    }
    va_end(list);


    switch (log_level)
    {

    case 0:
        break;

    case 1:
        if (lvl == LOG_INFO)
            log_format(lvl, file, func, line, msg);
        break;

    case 2:
        if (lvl == LOG_DEBUG)
            log_format(lvl, file, func, line, msg);
        break;

    case 3:
        if (lvl == LOG_ERROR)
            log_format(lvl, file, func, line, msg);
        break;

    case 4:
        if (lvl == LOG_ERROR || lvl == LOG_DEBUG)
            log_format(lvl, file, func, line, msg);
        break;

    case 5:
        log_format(lvl, file, func, line, msg);
        break;


    }

    fflush(stdout);

}
