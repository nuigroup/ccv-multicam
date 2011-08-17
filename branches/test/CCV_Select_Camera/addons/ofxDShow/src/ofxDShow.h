/*
*  ofxDShow
*  
*
*  Created on 24/04/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*  Edited by: Yishi Guo
*
*/
#ifndef OFXDSHOW_H_INCLUDED
#define OFXDSHOW_H_INCLUDED

#include "ofxCameraBase.h"
#include "ofMain.h"
//#include "ofxVideoGrabber.h"

class ofxDShow : ofxCameraBase
{
public:
	ofxDShow();
	~ofxDShow();
	void setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled);
	void getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled,int* minValue,int* maxValue);
	int getCameraBaseCount();
	void deinitializeCamera();
	void initializeWithDevicePath( char* devicePath, int cameraWidth, int cameraHeight, int cameraLeft, int cameraTop, unsigned char cameraDepth, int cameraFramerate, bool isEmulating, PIXEL_MODE pixelMode );
	unsigned char* getCameraFrame();
protected:
	void getNewFrame(unsigned char* newFrame);
	void cameraInitializationLogic();
	void cameraDeinitializationLogic();
	void updateCurrentFrame();
	void setCameraType();
private:
	//ofxVideoGrabber*	vidGrabber;
	unsigned char* cameraFrame;

	unsigned char* VIFrame;

	// Save the device id
	int deviceId;

	videoInput* VI;

};

#endif // OFXDSHOW_H_INCLUDED
