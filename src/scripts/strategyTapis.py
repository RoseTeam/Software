#!/usr/bin/env python
'''
Created on May 14, 2015

@author: gg99
'''

import robot
import argparse

from donneescarte import convCoul

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Lance Rosebot')
    parser.add_argument('-l', '--log', help='active le log', action="store_true")
    parser.add_argument('--vert', help='robot vert ?', action="store_true")
    
    args = parser.parse_args()
    
    rosebot = robot.Robot(log = args.log, jaune=not args.vert)

    rosebot.demarrage()
    
    pos = (1.0,0.6)
    if not rosebot.jaune:
        pos = convCoul(pos) 
    rosebot.goToXYloop(pos);
    
    rosebot.lacheTapis()