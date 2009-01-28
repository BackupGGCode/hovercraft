/*
 *  sonar.h
 *  Sonar
 *
 *  Created by Katherine Gunion on 25/01/09.
 *  Copyright 2009 University of Victoria. All rights reserved.
 *
 */

#ifndef _SONAR_H__
#define  _SONAR_H__


#define SONAR_PULSE 30

void fireSonar(void);
void sonarDistanceToDuty(long);
void sonarInit(void);
#endif


