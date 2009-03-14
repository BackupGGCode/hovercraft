#!/usr/bin/env python

prevError  = 0
totalError = 0

def updatePid(setPoint, processVar):
    kP = 0.3
    kD = 0.2
    kI = 0.3
    global totalError, prevError
    
    error = setPoint - processVar
    totalError += error
    proportionalTerm = kP * error
    derivativeTerm   = kD * (error - prevError)
    integralTerm     = kI * totalError
    prevError        = error
    
    return proportionalTerm + derivativeTerm + integralTerm
    
def main():
    setPoint = 20
    pv = 0
    
    for i in range(0, 200):
        pv = updatePid(setPoint, pv)
        print 'Iteration: %d, %f' % (i, pv)

if __name__ == '__main__':
    main()
    