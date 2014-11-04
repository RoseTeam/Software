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
#include "xml.h"
#include "roxml.h"
#include <sys/types.h>
#include <sys/stat.h>




/**************************************

          PRIVATE FUNCTIONS

**************************************/




/**************************************

          PUBLIC FUNCTIONS

**************************************/

char *xml_get_value(char *name_file, char *field)
{
    int j, max;
    node_t *root;
    node_t *cur;
    node_t **ans;

    root = roxml_load_doc(name_file);
    cur = root;
    if (root == NULL) 
    {
	return NULL;
    }

    ans = roxml_xpath(cur, field, &max);

    for (j = 0; j < max; j++)
    {
    	char *c = NULL;
    	char *s = NULL;
    	node_t *child;
    	c = roxml_get_content(ans[j], NULL, 0, NULL);
    	if (c) 
	{
    	    if (*c == 0) 
	    {
    		int i = 0;
    		int nb_chld = roxml_get_chld_nb(ans[j]);

    		for (i = 0; i < nb_chld; i++)
		{
    		    child = roxml_get_chld(ans[j], NULL, i);
    		    c = roxml_get_name(child, NULL, 0);
    		    return c;
    		}
    	    }else 
	    {
		return c;
    	    }
    	}
    }
    roxml_release(RELEASE_ALL);

    roxml_close(root);
    return 0;
}
