#gpio
ELEVATOR_PUSH_BUTTON=4
TIRETTE_PIN=2

#pwm
ELEVATOR=5
CLAMP=3
RIGHT_ARM=9
LEFT_ARM=6

#bras
ARM_UP		= 180
ARM_HORIZ 	= 80
ARM_OPEN 	= 60

#clamp
CLAMP_CLOSED 	= 20
CLAMP_CLOSED_HARD 		= 14
CLAMP_CLOSED_GOBELET	= 70
CLAMP_OPEN 		= 50
CLAMP_OPENFULL 	= 120

#elevator
ELEVATOR_UP_SPEED	= 40
ELEVATOR_DOWN_SPEED	= 180
ELEVATOR_UP_TIME	= 14 #second
ELEVATOR_DOWN_TIME	= 2

#Sharp
SHARP_LEFT=0
SHARP_RIGHT=1
SHARP_BEHIND=2


import Servo
import time
import mraa #@UnresolvedImport
from utils import logger, Vec
import donneescarte
from donneescarte import convCoul
import math
import threading

class Arm(object):
	
	
	def __init__(self, pin, upAngle, horizAngle, downAngle):
		self.servo = Servo.Servo("Arm")
		self.servo.attach(pin)
		self.upAngle = upAngle
		self.downAngle = downAngle
		self.horizAngle = horizAngle
		self.disable()
		
	def disable(self):
		self.servo.disable()

	def up(self):
		self.servo.enable()
		self.servo.write(self.upAngle)

	def down(self):
		self.servo.enable()
		self.servo.write(self.downAngle)

	def horiz(self):
		self.servo.enable()
		self.servo.write(self.horizAngle)
		
class Gobgob(object):
	def __init__(self, pinClamp, pinElevator, pinPushButton):
		#push button
		self.pushButton = mraa.Gpio(pinPushButton)
		self.pushButton.dir(mraa.DIR_IN)
		self.pushButton.mode(mraa.MODE_PULLUP)
		
		self.servoClamp = Servo.Servo("Clamp")
		self.servoElevator = Servo.Servo("Elevator")
		
		self.servoClamp.attach(pinClamp)
		self.servoElevator.attach(pinElevator)
		self.ballNumber = 0
		self.cylinderNumber = 0
		self.upSpeed = ELEVATOR_UP_SPEED
		self.downSpeed = ELEVATOR_DOWN_SPEED

		self.stateClamp = "none"
		self.stateElevator = "none"

		self.disable()
	def disable(self):
		self.servoElevator.disable()
		self.servoClamp.disable()
		
	def up(self):
		self.servoElevator.enable()
		self.servoElevator.write(self.upSpeed)
		for _d in range(0,int(ELEVATOR_UP_TIME/0.1)):
			if self.pushButton.read() == 0:
				print "up elevator"		
				break			
			time.sleep(0.1)
		self.servoElevator.disable()

	def down(self):
		self.servoElevator.enable()
		self.servoElevator.write(self.downSpeed)
		time.sleep(2)
		self.servoElevator.disable()

	def close(self, bloquant=False):
		self.servoClamp.enable()
		self.servoClamp.write(CLAMP_CLOSED)
		if bloquant:
			time.sleep(1.)		
			
	def closeGobelet(self):
		self.servoClamp.enable()
		self.servoClamp.write(CLAMP_CLOSED_GOBELET)
		time.sleep(0.3)


	def closeHard(self, bloquant=False):
		self.servoClamp.enable()
		self.servoClamp.write(CLAMP_CLOSED_HARD)
		if bloquant:
			time.sleep(0.9)		

	def open(self):
		self.servoClamp.enable()
		self.servoClamp.write(CLAMP_OPEN)
		time.sleep(0.1)

	def	openFull(self):
		self.servoClamp.enable()
		self.servoClamp.write(CLAMP_OPENFULL)

	#meta function
	def gob(self, async = False):
		self.gobEmpty = False;
		self.servoElevator.enable()
		self.servoElevator.write(self.downSpeed)
		time.sleep(1)
		self.open()
		time.sleep(3.9)
		self.servoElevator.disable()
		
		
	def prepareGob(self, async = False):
		self.close();
		self.up();
		
	def depose(self):
		self.down()
		self.open()
		self.openFull()
		
class Tirette(object):
	
	def __init__(self, pinTirette):
		self.gpio = mraa.Gpio(pinTirette)
		self.gpio.dir(mraa.DIR_IN)
		self.gpio.mode(mraa.MODE_PULLUP)
		
	def armee(self):
		""" True = tirette mise """
		return self.gpio.read() == 0

	def waitTiree(self):
		"""bloque jusqu'a ce que l'on tire"""
		while self.armee():
			time.sleep(0.1)
		
class Sharp(object):
	
	bits = 12
	threshold = 11500
	
	def __init__(self, pin):
		self.adc = mraa.Aio(pin) 
		self.Value = 0.0  #Initialization of value converted
		
	def read(self):
		self.adc.setBit(self.bits)         #Use 8 bits of the ADC
		self.Value = self.adc.read()  	
		return self.Value

	def detect(self):
		return self.read() > self.threshold


class InterfaceActuators(object):
	
	@logger('InterfaceActuators')
	def __init__(self):
		#self.rightArm = Arm(RIGHT_ARM, RIGHT_ARM_UP, RIGHT_ARM_DOWN)
		self.arm = Arm(pin=LEFT_ARM, upAngle=ARM_UP, horizAngle=ARM_HORIZ, downAngle=ARM_OPEN)
		self.gobgob = Gobgob(CLAMP, ELEVATOR, ELEVATOR_PUSH_BUTTON)
		self.sharpLeft = Sharp(SHARP_LEFT)
		self.sharpRight = Sharp(SHARP_RIGHT)
		self.sharpRear = Sharp(SHARP_BEHIND)
		self.tirette = Tirette(TIRETTE_PIN)
		
		self.arm.up()
		
	@logger()
	def disableActuators(self):
		self.arm.disable()
		self.gobgob.disable()

	def seesObstacleFront(self):
		return self.sharpRight.detect() or self.sharpLeft.detect()
	
	def seesObstacleBack(self):
		return self.sharpRear.detect()

