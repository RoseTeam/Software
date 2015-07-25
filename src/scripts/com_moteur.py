'''
Created on May 12, 2015

@author: gg99
'''
import mraa #@UnresolvedImport
import serial
import struct
from ctypes import cast, pointer, c_int, POINTER, c_float
import traceback
from utils import logger

class ParamSet(object):

    VEL_PARAMS = 'acc', 'dec', 'decEme', 'vmax'
    PID_PARAMS = 'kp', 'ki', 'kd'
        
    def __init__(self, acc, dec, decEme, vmax, kp, ki, kd):
        
        self.setAsserParams(acc, dec, decEme, vmax)
        self.setPIDParams(kp, ki, kd)
    
    def __str__(self):
        return ', '.join(str(param) for param in [self.acc, self.dec, self.decEme, self.vmax, self.kp, self.ki, self.kd])
    
    __repr__ = __str__ 
    
    def setAsserParams(self, acc, dec, decEme, vmax):
        self.acc = acc
        self.dec = dec
        self.decEme = decEme
        self.vmax = vmax

    def setPIDParams(self, kp, ki, kd):
        self.kp = kp
        self.ki = ki
        self.kd = kd
        

class AsserVel_V(object):
    
    PARAM_NAMES = ParamSet.VEL_PARAMS 
    SET_METHOD = 'setAsserDistParameter'
    PARAM_SET = 'vparams'
    
    def __init__(self, robot, **kwargs):
        self.newParams = kwargs
        self.robot = robot
        
    def __enter__(self):
        paramSet = getattr(self.robot, self.PARAM_SET)
        try:
            self.params = dict( (paramName, getattr(paramSet,paramName)) for paramName in self.PARAM_NAMES)
        except:
            print 'EXCCCCC'
            print self, paramSet, self.PARAM_NAMES
            raise
        newParams = [self.newParams.get(paramName, self.params[paramName]) for paramName in self.PARAM_NAMES] 
      
        getattr(self.robot,self.SET_METHOD)(*newParams)
        
        self.robot.printLog("setting new %s:"%self.PARAM_SET, dict(zip(self.PARAM_NAMES, newParams)))
        
    def __exit__(self, _type, value, traceback):
        self.robot.printLog("restoring %s:"%self.PARAM_SET, self.params)
        args = [self.params[paramName] for paramName in self.PARAM_NAMES]
        getattr(self.robot,self.SET_METHOD)(*args)

class AsserVel_W(AsserVel_V):
    PARAM_SET = 'wparams'
    PARAM_NAMES = ParamSet.VEL_PARAMS 
    SET_METHOD = 'setAsserAngleParameter'

class AsserPID_V(AsserVel_V):
    PARAM_SET = 'vparams'
    PARAM_NAMES = ParamSet.PID_PARAMS 
    SET_METHOD = 'setPIDLorder'

class AsserPID_W(AsserVel_W):
    PARAM_SET = 'wparams'
    PARAM_NAMES = ParamSet.PID_PARAMS 
    SET_METHOD = 'setPIDAorder'


    
    
class ConfigMoteur(object):
    
    def __init__(self):

        self.params = {}
        
        self.vparams = ParamSet(acc=0.85, dec=1.1, decEme=1.1, vmax=0.55, kp=0.03, ki=0., kd=0.0003)
        self.wparams = ParamSet(acc=3.2, dec=3.5, decEme=3.5, vmax=1.9,  kp=0.03, ki=0., kd=0.0003)
        
        for params, setAsserMethod, setPIDMethod in [(self.vparams, self.setAsserDistParameter,  self.setPIDLorder),
                                                     (self.wparams, self.setAsserAngleParameter, self.setPIDAorder)]:
            setAsserMethod(params.acc, params.dec, params.decEme, params.vmax)
            setPIDMethod(params.kp, params.ki, params.kd)
            
        self.setBlockageThreshold(0.5,1)
                
                
class InterfaceMoteur(ConfigMoteur):
    '''
    classdocs
    '''

    @logger('InterfaceMoteur')
    def __init__(self, port='/dev/ttyMFD1'):
        

        self.uuid = 0;
        
        mraa.Uart(0)
        try:
            self.ser = serial.Serial(
                port=port,
                baudrate=115200,
                #parity=serial.PARITY_ODD,
                #stopbits=serial.STOPBITS_TWO,
                #bytesize=serial.EIGHTBITS,
                xonxoff=serial.XOFF,
            )
            
            self.sendReset();
            self.sendReset();

        except:
            traceback.print_exc()

        ConfigMoteur.__init__(self)

        self.printLog('vparams', self.vparams)
        self.printLog('wparams', self.wparams)
                        
    def hexToFloat(self, s):
        i = int(s, 16)                   # convert from hex to a Python int
        cp = pointer(c_int(i))           # make this into a c integer
        fp = cast(cp, POINTER(c_float))  # cast the int pointer to a float pointer
        return fp.contents.value         # dereference the pointer, get the float

    def hexToInt(self, s):    
        return int(s, 16)

    def floatToHex(self, f):
        return ''.join('%.2x' % ord(c) for c in struct.pack('>f', f))
        
    def intToHex(self, i):
        s = "%X" % i
        return s

    def computeCs(self, buff):
        cs = 0
        for c in buff:    #rm '/n'
            cs = cs + ord(c)
        cs = cs % 256
        return '%.2x' % cs

    def verifyCs(self, buff):
        if (len(buff)<3): raise NameError('too short')
        readCs = int(buff[0:2],16)
        cs = 0    
        for c in buff[2:-1]:    #rm '/n'
            cs = cs + ord(c)
        cs = cs % 256
        if cs != readCs:
            raise NameError('wrong cs ' + str(cs) + " != " + str(readCs))
    
    
    @logger()
    def sendOrientation(self, angleAbs):
        self.uuid += 1
        cmd = 'A' + self.floatToHex(angleAbs) + ';' + self.intToHex(self.uuid)
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
        return self.uuid
        
    @logger()
    def sendTarget(self, distance, angle, finalDistanceSpeed=0, finalAngleSpeed=0):
        self.uuid += 1; 
        
        cmd = 'C' + self.floatToHex(distance) + ';' + self.floatToHex(angle) + ';' + self.floatToHex(finalDistanceSpeed) + ';' + self.floatToHex(finalAngleSpeed) + ';' + self.intToHex(self.uuid)

        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
        return self.uuid
    
    @logger()
    def sendTargetXY(self, pos, finalDistanceSpeed=0, mode=3):
        """ mode:
            1: orientation seule
            2: distance seule (tout droit)
            3: orient et distance en meme temps 
        """
        self.uuid = self.uuid + 1; 
        cmd = 'K' + self.floatToHex(pos[0]) + ';' + self.floatToHex(pos[1]) + ';' + self.floatToHex(finalDistanceSpeed) + ';' + self.intToHex(mode) + ';' + self.intToHex(self.uuid)
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
        return self.uuid
        
    @logger()
    def setAsserDistParameter(self, acc, dec, decEmergency, vmax):
        self.vparams.setAsserParams(acc, dec, decEmergency, vmax)
        cmd = 'P' + self.floatToHex(acc) + ';' + self.floatToHex(dec) + ';' + self.floatToHex(decEmergency) + ';' + self.floatToHex(vmax)
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
        
    @logger()
    def setAsserAngleParameter(self, acc, dec, decEmergency, vmax):
        self.wparams.setAsserParams(acc, dec, decEmergency, vmax)
        cmd = 'Q' + self.floatToHex(acc) + ';' + self.floatToHex(dec) + ';' + self.floatToHex(decEmergency) + ';' + self.floatToHex(vmax)
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
        
    @logger()
    def setBlockageThreshold(self, blockDist, blockAngle):        
        self.params["BlockageThreshold"] = (blockDist, blockAngle)
        cmd = 'E' + self.floatToHex(blockDist) + ';' + self.floatToHex(blockAngle)
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
    
    @logger()
    def sendTtwist(self, Twist):
        cmd = 'T' + self.floatToHex(Twist)
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
        
    @logger()
    def sendVtwist(self, Vtwist):    
        cmd = 'V' + self.floatToHex(Vtwist)
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')

    @logger()
    def enablePower(self):
        cmd = 'U' + self.floatToHex(float(1))
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
        #rospy.loginfo("Robot Start")

    @logger()
    def disablePower(self):
        cmd = 'U' + self.floatToHex(float(0))
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
        #rospy.loginfo("Robot Stop")

    @logger()
    def enableOdo(self):
        cmd = 'O' + self.floatToHex(float(1))
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
        #rospy.loginfo("enable odo")

    @logger()
    def disableOdo(self):
        cmd = 'O' + self.floatToHex(float(0))
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
        #rospy.loginfo("disable odo")

    @logger()
    def enableMetrics(self):
        cmd = 'M' + self.floatToHex(float(1))
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
        #rospy.loginfo("enable metrics")

    @logger()
    def disableMetrics(self):
        cmd = 'M' + self.floatToHex(float(0))
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
        #rospy.loginfo("disable metrics")
        
    @logger()
    def sendReset(self):
        cmd = 'S' + self.floatToHex(float(0))
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
        self.ser.write(cs+cmd+'\n') # send 2 times
        #rospy.loginfo("Robot Reset")

    @logger()
    def setPIDLorder(self, kp, ki, kd):        
        self.vparams.setPIDParams(kp, ki, kd)
        cmd = '{' + self.floatToHex(kp) + ';' + self.floatToHex(ki) + ';' + self.floatToHex(kd)
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
        
    @logger()
    def setPIDAorder(self, kp, ki, kd):        
        self.wparams.setPIDParams(kp, ki, kd)
        cmd = '(' + self.floatToHex(kp) + ';' + self.floatToHex(ki) + ';' + self.floatToHex(kd)
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')

    @logger()
    def setX(self, x):
        cmd = 'x' + self.floatToHex(x)
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')

    @logger()
    def setY(self, y):
        cmd = 'y' + self.floatToHex(y)
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')

    @logger()
    def setAngle(self, angle):
        cmd = 'w' + self.floatToHex(angle)
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')
        
    @logger()
    def setPos(self, x, y, w):
        self.setX(x);
        self.setY(y);
        self.setAngle(w);
        
    @logger()
    def sendStop(self, emergency = 0):
        val = 0
        if emergency:
            val = 1.0
        cmd = 'D' + self.floatToHex(val)
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')

    @logger()
    def setTickCoef(self, tickToAngle, tickToDistance):
        
        cmd = 'c' + self.floatToHex(tickToAngle) + ';' + self.floatToHex(tickToDistance)
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')

    @logger()
    def setSampleTime(self, angleSampleTIme, distanceSampleTime):
        
        cmd = 'd' + self.floatToHex(angleSampleTIme) + ';' + self.floatToHex(distanceSampleTime)
        cs = self.computeCs(cmd)
        self.ser.write(cs+cmd+'\n')

    def printInterpretedData(self, data):
        cmd = data[0]
        if cmd == 'O':
            array = data[1:].split(";")
            if len(array) >= 3:
                print "X " + str(self.hexToFloat(array[0])),
                print "Y " + str(self.hexToFloat(array[1])),
                print "W " + str(self.hexToFloat(array[2])),
            if len(array) >= 5:
                print "V " + str(self.hexToFloat(array[3])),
                print "O " + str(self.hexToFloat(array[4])),
            print
        
        if cmd == 'S':
            array = data[1:].split(";")
            if len(array) >= 4:
                print "uid " + str(self.hexToInt(array[0])),
                print "run " + str(self.hexToInt(array[1])),
                print "blockDist " + str(self.hexToFloat(array[2])),
                print "blockAngle " + str(self.hexToFloat(array[3]))

    def read(self):
        #while self.ser.inWaiting():
        while True:
            try:
                l = self.ser.readline()
                self.verifyCs(l)
                self.printInterpretedData(l[2:])

            except (RuntimeError, TypeError, NameError, ValueError) as e:
                import traceback
                traceback.print_exc()
                print e
                pass
        