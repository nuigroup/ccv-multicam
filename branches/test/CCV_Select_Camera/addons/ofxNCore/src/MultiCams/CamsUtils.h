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
#include "ofxXmlSettings.h"

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

	bool isSelected( int rawId );
	void setSelected( int rawId );
	bool isUsed( int displayId );

	void setXY( int x, int y );
	void setCam( int index, PS3* cam );
	void setCam( int x, int y, PS3* cam );
	void resetAll();

	//! Reset the camera parameters
	void resetCam( int rawIndex );
	void resetCams();

	//! Save the settings to XML file
	void saveXML( string filename = "MultiCams.xml" );

	//! Load the settings from XML file
	void loadXML( string filename = "MultiCams.xml" );

	int camCount, selectedCamCount;
	int xGrid, yGrid;
	PS3** rawCams;
	PS3** displayCams;
	bool* camsSelected;	//< For raw cameras
	bool* camsUsed;		//< For display cameras

	//! XML settings
	PS3** xmlCams;
	int numCamTags;

private:
	int getDevicesCount();
	GUID getGUID( int camId );
	void createDisplayCams( int x, int y);
	
	void resetCamsSelected();

	CLEyeCameraColorMode colorMode;
	CLEyeCameraResolution camRes;
	float frameRate;
	ofxXmlSettings XML;
};

// ----------------------------------------------

#endif

// ----------------------------------------------