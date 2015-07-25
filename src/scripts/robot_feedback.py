from rosebotnav import Vec
import threading
from utils import logger

class InterfaceFeedBack(object):
    
    @logger('InterfaceFeedBack')
    def __init__(self):
        
        #last values
        self.pos = Vec(0,0)
        self.orient = 0.0
        self.distanceSpeed = 0.0
        self.AngleSpeed = 0.0
        self.currentId = -1
        self.moving = False
        self.blockageDist = 0.0
        self.blockageAngle = 0.0
        
        self.lock = threading.RLock()    
        
        self.myThread = threading.Thread(target=self.serialParser)
        self.myThread.setDaemon(True)
        self.myThread.start()
        
    def serialParser(self):
        while True:
            try:
                l = self.ser.readline()
                self.verifyCs_(l)
                self.interpretedData(l[2:])
                
            except (RuntimeError, TypeError, NameError, ValueError) as e:
                import traceback
                traceback.print_exc()
                print e
                pass
    
    def interpretedData(self, data):
        cmd = data[0]
        with self.lock:
            if cmd == 'O':
                array = data[1:].split(";")
                if len(array) >= 3:
                    self.pos = Vec(self.hexToFloat(array[0]), self.hexToFloat(array[1]))
                    self.orient = self.hexToFloat(array[2])
                if len(array) >= 5:
                    self.distanceSpeed = self.hexToFloat(array[3])
                    self.angleSpeed = self.hexToFloat(array[4])
        
            if cmd == 'S':
                array = data[1:].split(";")
                if len(array) >= 4:
                    self.currentId = self.hexToInt(array[0])
                    self.moving = bool(self.hexToInt(array[1]))
                    self.blockageDist = self.hexToFloat(array[2])
                    self.blockageAngle = self.hexToFloat(array[3])
    
    def getPos(self):
        with self.lock:
            return self.pos
            
    def getOrient(self):
        with self.lock:
            return self.orient
            
    def getUUID(self):
        with self.lock:
            return self.currentId
    
    def getBlockage(self):
        with self.lock:
            return self.blockageDist, self.blockageAngle

    def isMoving(self):
        with self.lock:
            return self.moving
    
    def verifyCs_(self, buff):
        if (len(buff)<3): raise NameError('too short')
        readCs = int(buff[0:2],16)
        cs = 0    
        for c in buff[2:-1]:    #rm '/n'
            cs = cs + ord(c)
        cs = cs % 256
        if cs != readCs:
            raise NameError('wrong cs ' + str(cs) + " != " + str(readCs))
