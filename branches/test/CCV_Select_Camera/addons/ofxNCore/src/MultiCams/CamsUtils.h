//! CamsUtils.h
/*!
*  
*
*  Created by Yishi Guo on 06/13/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/
// ----------------------------------------------

#ifndef CAMS_UTILS_H
#define CAMS_UTILS_H

// ----------------------------------------------

#include "PS3.h"

// ----------------------------------------------

class CamsUtils {
public:
	CamsUtils();
	~CamsUtils();

	void setup( CLEyeCameraColorMode colorMode, CLEyeCameraResolution camRes, float frameRate );
	void update();
	void start();
	void stop();

	int getCount();
	int getXGrid();
	int getYGrid();

	PS3* getCam( int index );
	PS3* getCam( int x, int y );
	PS3* getRawCam( int index );

	PS3** getCams();
	PS3** getRawCams();

	void setXY( int x, int y );
	void setCam( int index, PS3* cam );
	void setCam( int x, int y, PS3* cam );

	int camCount, selectedCamCount;
	int xGrid, yGrid;
	PS3** rawCams;
	PS3** displayCams;

private:
	int getDevicesCount();
	GUID getGUID( int camId );
	void createDisplayCams( int x, int y);

	CLEyeCameraColorMode colorMode;
	CLEyeCameraResolution camRes;
	float frameRate;
};

// ----------------------------------------------

#endif

// ----------------------------------------------