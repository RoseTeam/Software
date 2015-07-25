'''
Created on May 13, 2015

@author: gg99
'''
import math
from donneescarte import convCoul
import time
from utils import Vec, logger
from com_moteur import AsserVel_V, AsserPID_V

class AllerRetour(object):
    """ Contexte pour executer une action et revenir au point de depart """    
    
    def __init__(self, robot, chemin, orientFinale=None):
        self.robot = robot
        self.chemin = chemin
        self.posIni = robot.pos
        self.orientFinale = orientFinale

    def __enter__(self):
        """ aller a l'objectif """
        for pos in self.chemin:
            self.robot.goToXY(pos)
                    
    def __exit__(self, _type, value, traceback):
        """retour au depart"""
        for pos in list[reversed(self.chemin)]+[self.posIni]:
            self.robot.goToXY(pos)
        
        if self.orientFinale is not None:
            self.robot.sendTarget(0, self.orientFinale-self.angle)


class InterfaceScenarios(object):
    
    @logger('InterfaceScenarios')
    def __init__(self):
        pass


    @logger()
    def lancer(self):
        self.homologueJaune()
        
    @logger()    
    def lancer1(self):
        pass
    
    @logger()
    def saisirGobelet(self, posXY, rayon=.06):
        self.gobgob.openFull()
        self.sapprocherObject(posXY)
        self.miseEnBoucheObject(posXY)
        self.gobgob.closeGobelet()
        time.sleep(0.1)
        
    @logger()
    def saisirObjet(self, posXY, rayon=.06):
                
        #ouvre la pince
        self.gobgob.closeHard()
        self.gobgob.up()
        self.sapprocherObject(posXY)
        self.miseEnBoucheObject(posXY)
        
        self.gobgob.gob()
        # ferme la pince
        self.gobgob.close(bloquant=False)
        
        return True

    
    def sapprocherObject(self, posXY, rayon=.06, vApproche=.06):
        self.sorienterXY(posXY)
        
        posActuel = self.getPos()
        dist = Vec(*posXY) - posActuel
        distUni = dist.uni
        
        # approche rapide 
        distApproche = dist.nor - rayon - self.distAvant 
        self.printLog("distApproche object ", distApproche)
        
        dist = distApproche * distUni
        
        
        target = dist + posActuel
        self.printLog("target object ", target, "vProximite", vApproche)
        
        self.goToXYloop(target, butTol=.03, vFinale=vApproche)
        
    def miseEnBoucheObject(self,posXY, rayon=.06, vApproche=.06):
        posActuel = self.getPos()
        dist = Vec(*posXY) - posActuel
        
        dGobage = dist.nor + rayon
        
        self.printLog("dGobage ", dGobage)

# approche fine en vitesse lente
        with AsserVel_V(dec = 0.5, vmax = vApproche):
            self.avance(dGobage, checkObst=0)
        
        self.sendReset()
        

    @logger()
    def lacheTapis(self):

        vlente = .15
        
        kp = self.vparams.kp * .2
        ki = 0
        kd = 0
        
        if self.jaune:
            posIni = Vec(0.780, 0.767)
            self.goToXYloop(posIni);
            self.sorienter(0)
            self.arm.horiz()

            with AsserVel_V(self,vmax=vlente):
                self.avance(-abs(0.310 - self.getPos()[0]),  checkObst=0);
                self.arm.down()
                with AsserPID_V(self, kp=kp,ki=ki,kd=kd):
                    self.sendTarget(-0.045,0.18)
                    time.sleep(1)
                    self.sendReset()
                    
                    time.sleep(.2)
                    #DECOINCER  BORD
                    self.sendTarget(0.15, -0.5)
                    time.sleep(1.5)
                    self.sendReset()
                    self.avance(0.15)
                    time.sleep(1.6)
                    self.arm.up()
        
        else: #robot vert
            posIni = Vec(0.780, 2.233)            
            self.goToXYloop(posIni);
            self.sorienter(math.pi)
            self.arm.horiz()
            
            with AsserVel_V(self, vmax=vlente):
                self.avance(abs(0.370 - self.getPos()[0]), checkObst=0);
                self.arm.down()
                with AsserPID_V(self, kp=kp,ki=ki,kd=kd):
                    self.sendTarget(0.045,-0.18)
                    time.sleep(1)
                    self.sendReset()
                    time.sleep(.2)
                    #DECOINCER  BORD
                    self.sendTarget(-0.15, 0.5)
                    time.sleep(1.5)
                    self.sendReset()
                    self.avance(-0.15)
                    time.sleep(1.6)
                    self.arm.up()
        
        return True


    
    def homologueMoi(self):
        
        posIni = 1., self.distArriere + .15
        orientIni =  math.pi/2.
        
        pointPass = Vec(1.0, 0.6) 
        posObj    = Vec(1.355, 0.87)
        
        if not self.jaune:
            posIni = convCoul(*posIni) 
            orientIni *= -1
            pointPass = convCoul(*pointPass)
            posObj = convCoul(posObj)
            
        self.preDepart(posIni, orientIni)
        
        chemin = [pointPass]
                            
        with AllerRetour(self,chemin):
            self.saisirObjet(posObj,rayon=.05)
        
        # depose l'objet
        self.gobgob.open()
    
    
    @logger()
    def demarrage(self):
        posdepart = 1., self.distArriere + .07
        aDepart =  math.pi/2.
        
        if not self.jaune:
            posdepart = convCoul(posdepart) 
            aDepart *= -1
                 
        self.preDepart(posdepart,aDepart)
    
    @logger()
    def homologueJaune(self):
        

        self.setBlockageThreshold(1,1)

        self.setAsserDistParameter(0.3, 0.3, 0.3, 0.3)

        self.setPIDAorder(0.04, 0, 0.00035)

        self.setPIDLorder(0.04, 0, 0.00035)

        self.setAsserAngleParameter(3.2, 3.5, 3, 1.9)

        
        posdepart = 1., self.distArriere + .15
        aDepart =  math.pi/2.
        
        if not self.jaune:
            posdepart = convCoul(*posdepart) 
            aDepart *= -1
                 
        self.preDepart(posdepart,aDepart)
        
        self.printLog()
        

        self.goToXYloop((1.0, 0.6))

        self.goToXYloop((1.355, 0.87))

        self.avance(0.05)            
        
        self.gobgob.close()
        
        self.goToXYloop((1.0, 0.6))
        
        self.printLog('depose')
        self.gobgob.openFull()

        self.printLog('recule de 5 cm')
        self.avance(-.05)           
        

    
