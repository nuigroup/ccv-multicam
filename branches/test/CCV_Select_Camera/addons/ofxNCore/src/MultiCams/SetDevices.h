//! SetDevices.h
/*!
*  
*
*  Created by Yishi Guo on 31/05/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

#ifndef SET_DEVICES_H_
#define SET_DEVICES_H_

#include "ofxNCore.h"

class SetDevices : public ofBaseApp {
public:
	SetDevices( bool bDebug = false );

	void setup();
	void update();
	void draw();
	void exit();

	////////////////
	// TEST STUFF
	int testInt;
	ofTrueTypeFont testFont;

};

#endif