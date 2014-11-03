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
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include "sysfs.h"

#define BUFSIZE 1024




/**************************************

          PRIVATE FUNCTIONS

**************************************/




/**************************************

          PUBLIC FUNCTIONS

**************************************/

int sysfs_read(const char *path, unsigned char *buffer)
{
    int ret;
    int fd;
    fd = open(path, O_RDONLY);
    if (fd < 0)
    {
	error("Failed to open %s : %s\n", path, strerror(errno));
	return fd;
    }

    int size = 0;
    unsigned char *buf = malloc(BUFSIZ * sizeof(char));
    if (buffer)
	buf = buffer;
    do
    {
	ret = read(fd, buf, BUFSIZ);
	if (ret == -1)
	    break;
	size += ret;
	if (buffer)
	    buf += ret;
    }while (ret > 0);

  close:
    close(fd);

    if (ret < 0)
    {
	error("Failed to read file %s : %s\n", path, strerror(errno));
	size = ret;
    }
    return size;
}


int sysfs_write(const char *path, unsigned char *buffer)
{


}
