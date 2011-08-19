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

//#include "PS3.h"
#include "ofxPS3.h"
#include "ofxDShow.h"
#include "ofxXmlSettings.h"
#include "ofxCameraBase.h"
#include "ofxCameraBaseSettings.h"


// ----------------------------------------------

class CamsUtils {
public:
	CamsUtils();
	~CamsUtils();

	void setup( CLEyeCameraColorMode colorMode, CLEyeCameraResolution camRes, float frameRate );
	void setup();
	void update();
	void start();
	void stop();

	int getCount();
	int getXGrid();
	int getYGrid();

	//int getrawid( ps3* cam );

	//ps3* getcam( int index );
	//ps3* getcam( int x, int y );
	//ps3* getrawcam( int index );

	//ps3** getcams();
	//ps3** getrawcams();

	int getRawId( ofxCameraBase* cam );

	ofxCameraBase* getCam( int index );
	ofxCameraBase* getCam( int x, int y );
	ofxCameraBase* getRawCam( int index );

	ofxCameraBase** getCams();
	ofxCameraBase** getRawCams();

	ofxCameraBaseSettings* getRawCamSettings( int index );

	//! Set the camera feature by raw cam id
	void setRawCamFeature( int rawId, CAMERA_BASE_FEATURE feature, int firstValue, int secondValue, bool isAuto, bool isEnable );

	bool isSelected( int rawId );
	void setSelected( int rawId, bool reset = false );
	bool isUsed( int displayId );

	void setXY( int x, int y );
	//void setCam( int index, PS3* cam );
	//void setCam( int x, int y, PS3* cam );
	void setCam( int index, ofxCameraBase* cam );
	void setCam( int x, int y, ofxCameraBase* cam );
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
	//PS3** rawCams;
	//PS3** displayCams;

	//! XML settings
	//PS3** xmlCams;
	int numCamTags;

private:
	int getDevicesCount( bool bAll = true, CAMERATYPE type = PS3, bool bPure = false );
	GUID getGUID( CAMERATYPE type, int camId );
	char* getDevicePath( CAMERATYPE type, int camId );
	void createDisplayCams( int x, int y);
	
	void resetCamsSelected();

	//! Set the default value for camera
	void setupCameraSettings( ofxCameraBaseSettings *settings );
	void applyCameraSettings();
	void receiveSettingsFromCameras();
	void receiveSettingsFromRawSettings();
	void copySettingsFromXmlSettings( ofxCameraBaseSettings *src, ofxCameraBaseSettings *dst );
	void startCameras();

	CLEyeCameraColorMode colorMode;
	CLEyeCameraResolution camRes;
	float frameRate;
	ofxXmlSettings XML;

	ofxCameraBase** rawCams;
	ofxCameraBase** displayCams;
	ofxCameraBase** xmlCams;
	ofxCameraBaseSettings** rawCamsSettings;
	ofxCameraBaseSettings** displayCamsSettings;
	ofxCameraBaseSettings** xmlCamsSettings;
	bool* camsSelected;	//< For raw cameras
	bool* camsUsed;		//< For display cameras
};

// ----------------------------------------------

#endif

// ----------------------------------------------