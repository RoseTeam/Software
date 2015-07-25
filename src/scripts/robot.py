#!/usr/bin/env python

from com_moteur import InterfaceMoteur, AsserVel_V
import time
from robot_feedback import InterfaceFeedBack
from actuator import InterfaceActuators

import thread
import threading
from rosebotnav import Vec
import traceback
from scenarios import InterfaceScenarios
from utils import logger, Loggable, suppressLog
import math
from com_moteur import AsserVel_V, AsserPID_V

class EchecAction(Exception):
	pass

class DetectionObstacle(Exception):
	pass


class Robot(InterfaceMoteur,InterfaceFeedBack,InterfaceActuators,InterfaceScenarios,Loggable):

	def __init__(self, jaune=True, log=False, port='/dev/ttyMFD1'):

		Loggable.__init__(self,log=log)
		
		if log:
			print
			print "Creation d'un rosebot", 'jaune' if jaune else 'vert'
			print

		self.distArriere = .152 
		self.distAvant = .155
				
		self.jaune = jaune
		try:# pour tester le robot hors de la carte
			InterfaceMoteur.__init__(self, port=port)
			InterfaceFeedBack.__init__(self)
			InterfaceActuators.__init__(self)
			InterfaceScenarios.__init__(self)
		except:
			traceback.print_exc()
			
		
	@logger()	
	def decompte(self, sec=90., dt=.1):
		endTime = time.time() + sec
		
		while True:
			novTime = time.time()
			if novTime >= endTime:
				break
			
			if self.tirette.armee():
				self.printLog( "sortie du programme sur remise de la tirette apres %.2fs"%(sec-(endTime-novTime)) )
				import sys
				sys.exit(1)
				#raise KeyboardInterrupt(message)
			time.sleep(dt) 
		
		self.disableActuators()
		self.disableActuators()
		self.disablePower()
		self.disablePower()
		time.sleep(0.5)
		self.disableActuators()
		self.disableActuators()
		self.disablePower()
		self.disablePower()

		self.printLog( "fin du temps imparti: %.2fs"%sec )
		
		thread.interrupt_main()
	
	@logger()
	def waitPreTirette(self, pos, orient):
		
		self.disablePower()
		self.disablePower()
		
		if self.tirette.armee():
			self.printLog("Attente de tirage de tirette initiale")
			while self.tirette.armee():
				time.sleep(.1)
		
		self.printLog("Attente d'insertion de tirette, pos ini:", pos, 'orient', orient)

		x,y = pos

		with suppressLog(self):
			while not self.tirette.armee():	
				self.sendReset()
				self.setPos(x,y,orient)
				time.sleep(.1)

		self.enablePower()
		self.enablePower()
		
		self.printLog("puissance activee")

	@logger()
	def preDepart(self, pos, orient):
		
		self.gobgob.close()
		self.waitPreTirette(pos, orient)
		self.gobgob.close()
		self.gobgob.up()
		time.sleep(1.5)	

		self.printLog("avance de 10 cm")
		# avance de 10 cm
		self.avance(0.1, 0, checkObst=0)
		#dtime.sleep(1)
		#self.goToXY(posXY=(0,0.1),relatif=True)

		
		self.printLog("Pret au depart: attente de tirage de tirette")
		
		with suppressLog(self):
			while self.tirette.armee():
				self.sendReset()
				time.sleep(.1)

		self.resetLogTime()		
		thread_sablier = threading.Thread(target=self.decompte)
		thread_sablier.daemon = True
		thread_sablier.start()
	
			
	@logger()
	def waitStop(self, cmdId, checkObst=0, timeoutsec=10., dt=0.05):
		""" Renvoie True si le mouvement courant s'est termine sans etre interrompu par un obstacle """

		timestart = time.time()	
		seenObst = False
		while cmdId > self.getUUID() or self.isMoving():
			
			time.sleep(dt)

			if checkObst > 0:
				
				if self.seesObstacleFront():
					self.printLog( "perception d'un obstacle devant" )    
					seenObst = True
					break
				
			elif checkObst < 0:
				
				if False: # pas de sharp derriere pour l'instant
					if self.seesObstacleBack():
						self.printLog( "perception d'un obstacle derriere" )    
						seenObst = True
						break
				
			if timeoutsec is not None and time.time() > timeoutsec + timestart:
				seenObst = True
				break
				
		if seenObst:
			self.sendStop(emergency=0) # emergency=1 foireux ?
			return False
						
		return True
	
	@logger()
	def avance(self, dist, vFinale=0, checkObst=1):
		
		# aligne sur la cible, on avance tout droit de la distance d'ecart  		
		cmdId = self.sendTarget(dist, 0, finalDistanceSpeed=vFinale, finalAngleSpeed=0)

		return self.waitStop(cmdId, checkObst=checkObst)
			
		
	@logger()
	def goToXYloop(self, pos, butTol=.025, vFinale=0, wFinale=0, checkObst=1):
		
		while not self.goToXY(pos):
			
			if (self.getPos()-pos) < butTol:
				self.printLog("obstacle trouve mais on est a %.2f du but. pos:"%butTol, self.getPos())
				break
			
			time.sleep(1)
			
		self.sendReset()
	
		
	@logger()
	def goToXY(self, posXY, relatif=False, vFinale=0, wFinale=0, checkObst=1):
		""" S'orient et avance vers la position posXY.
			True si la position XY est atteinte ; False si un obstacle interrompt le mouvement 
		"""
		posXY = Vec(*posXY)		
		if relatif:
			posXY += self.pos
				
		# dabord sorienter dans la bonne direction
		#self.printLog("sorienter vers", posXY, 'depuis', self.pos, 'orient', self.getOrient())
		with AsserPID_V(self, kp=0.01,ki=0.,kd=0.): 
			cmdId = self.sendTargetXY(posXY, mode=1)
		
		# pas de verif d'obstacle pour la rotation
		self.waitStop(cmdId, checkObst=0)
		
		# avance en ligne droite
		#self.printLog("en ligne droite vers", posXY, 'depuis', self.pos)
		#self.avance(posXY, vFinale=vFinale, wFinale=wFinale)
		cmdId = self.sendTargetXY(posXY, mode=3)
		
		return self.waitStop(cmdId, checkObst=checkObst)

	@logger()
	def sorienterXY(self, posXY): 
		""" orientation vers un point du terrain """

		with AsserPID_V(self, kp=0.01,ki=0.,kd=0.): 
			cmdId = self.sendTargetXY(posXY, mode=1)
		
		# pas de verif d'obstacle pour la rotation
		self.waitStop(cmdId, checkObst=0)
		
	@logger()
	def sorienter(self, angleAbs):
		""" orientation absolue """

		cmdId = self.sendOrientation(angleAbs)
		# pas de verif d'obstacle pour la rotation		
		return self.waitStop(cmdId=cmdId, checkObst=0)
		
		
	