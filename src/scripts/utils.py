'''
Created on May 14, 2015

@author: gg99
'''

import time

from rosebotnav import Vec

class Loggable(object):
    
    def __init__(self, log=True, toFile=True, logDir = '/home/scripts/logs/'):
        
        self.resetLogTime()
        self._doLog = log
        self.indentLevel = 0
        if toFile:
            logFile = logDir + '%s.log'%(str(time.time()))
            self._logFile = open(logFile,'w')
            print 'writing logs to', logFile
        else:
            self._logFile = None

    def resetLogTime(self):
        self._logStartTime = time.time()
        
    def printLog(self,*args):
        if self._doLog:            
            elapsedTime = time.time() - self._logStartTime
            indent = '\t' * self.indentLevel
            timeStr = "[%.2f]%s"%(elapsedTime, indent)
            msg = ' '.join([timeStr, ' '.join(str(arg) for arg in args)])
            print msg
            
            if self._logFile:
                self._logFile.writelines([msg])
                

def logger(comment=''):
        
    def logger_decorator(func):
        
        def f(loggable, *args, **kwargs):
            
            aargs = list(args) + [str(name) + '=' + str(val) for name, val in kwargs.iteritems()]
            argsStr = '(' + ', '.join(str(arg) for arg in aargs) + ')'
            if comment:
                argsStr += '# ' + comment 
            
            loggable.printLog('--'+func.__name__+argsStr)
            loggable.indentLevel += 1
            
            res = func(loggable, *args, **kwargs)            
            loggable.indentLevel -= 1
            
            return res
        
        return f
    
    return logger_decorator

   
class suppressLog(object):
    
    def __init__(self, loggable, **kwargs):
        self.loggable = loggable
        
    def __enter__(self):
        self.loggable.printLog("stop log")
        self.loggable._doLog = False
        
    def __exit__(self, _type, value, traceback):
        self.loggable._doLog = True
        self.loggable.printLog("start log")
 

class MockRobot(Loggable):
    
    def __init__(self):
        Loggable.__init__(self)
    
    @logger()
    def MethodA(self,*args, **kwargs):
        self.printLog('CoucouA')
        time.sleep(.2)
        
        self.MethodB(*args, v = 0, w=Vec(1,1))
        with suppressLog(self):
            self.MethodB(*args, v = 0, w=Vec(1,1))

    @logger()
    def MethodB(self, *args, **kwargs):
        self.printLog( 'coucouB')

if __name__ == '__main__':
    
    mR = MockRobot()
    mR.MethodA()