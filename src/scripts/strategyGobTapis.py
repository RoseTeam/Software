#!/usr/bin/env python
'''
Created on May 14, 2015

@author: gg99
'''

import robot
import argparse
import math
import time
from utils import Vec, logger
from com_moteur import AsserVel_V, AsserPID_V, AsserPID_W


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
    rosebot.goToXYloop(pos)

    
    poteau = (1.355, 0.870)
    if not rosebot.jaune:
        poteau = convCoul(poteau) 
       
    rosebot.saisirObjet(poteau)
    
    poteau = (1.400, 1.300)
    if not rosebot.jaune:
        poteau = convCoul(poteau) 
       
    rosebot.saisirObjet(poteau)

    
    #deposer
    depose = (1.1 , 0.6)
    if not rosebot.jaune:
        depose = convCoul(depose) 
    rosebot.goToXYloop(depose)
        
    if rosebot.jaune:
        rosebot.sorienter(-math.pi/2.0 - 0.01 )
    else:
        rosebot.sorienter(math.pi/2.0 + 0.01)
    
    rosebot.sendReset()
    rosebot.gobgob.openFull()
    time.sleep(0.9)
        
    rosebot.avance(-0.15);
    
    # gobelet
    
    gobelet = (0.830, 0.910)
    if not rosebot.jaune:
        gobelet = convCoul(gobelet) 
       
    rosebot.saisirGobelet(gobelet)
    
    #deposer gob
    depose = (0.9 , 0.61)
    if not rosebot.jaune:
        depose = convCoul(depose) 
    rosebot.goToXYloop(depose)
        
    if rosebot.jaune:
        rosebot.sorienter(-math.pi/2.0 + 0.01)
    else:
        rosebot.sorienter(math.pi/2.0 + 0.01)
    
    rosebot.gobgob.openFull()
    time.sleep(0.8)
        
    rosebot.avance(-0.25);
    
    #  tapis
    
    rosebot.lacheTapis()
        
    
    #clap
    
    
    #gotclap
    posDebutClap = (1.75,0.80)
    posEndClap = (1.75, 1.01)
    
    if not rosebot.jaune:
        posDebutClap = convCoul(posDebutClap) 
        posEndClap = convCoul(posEndClap)
    
    rosebot.goToXYloop(posDebutClap)
    
    if rosebot.jaune:
        with AsserPID_V(rosebot, kp=0.004,ki=0,kd=0):
            rosebot.sorienter(-3*math.pi/4)
            rosebot.sendReset()
            rosebot.arm.horiz()
            time.sleep(1)
            rosebot.sendTarget(-0.05,0)
            time.sleep(2)
            rosebot.sendReset()
        with AsserPID_W(rosebot, kp=0.01,ki=0,kd=0):
            rosebot.sorienter(-math.pi/2)
            rosebot.sendTarget(-0.31,0)
    else:
        with AsserPID_V(rosebot, kp=0.004,ki=0,kd=0):
            rosebot.sorienter(-math.pi/4.0)
            rosebot.sendReset()
            rosebot.arm.horiz()
            time.sleep(2)
            rosebot.sendReset()
        with AsserPID_W(rosebot, kp=0.01,ki=0,kd=0):
            rosebot.sorienter(-math.pi/2)
            rosebot.sendTarget(0.30,0)
    
    time.sleep(3.5)
    rosebot.sendReset();
    
    
    