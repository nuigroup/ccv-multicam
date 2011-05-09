/*
*  ofxDShow
*  
*
*  Created on 24/04/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#ifndef OFXDSHOW_H_INCLUDED
#define OFXDSHOW_H_INCLUDED

#include "ofxCameraBase.h"
#include "ofMain.h"

class ofxDShow : ofxCameraBase
{
public:
	/*
	ofxDShow();
	~ofxDShow();
	
	int getCameraBaseCount();
    void setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled);
	void initializeWithGUID(GUID cameraGuid,int cameraWidth,int cameraHeight,int cameraLeft,int cameraTop,bool useROI,unsigned char cameraDepth,int cameraFramerate);
	void deinitializeCamera();
	void updateCurrentFrame();
	void getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled);
	void callCameraSettingsDialog();
	unsigned char* getCameraFrame();*/
private:
	ofVideoGrabber*		vidGrabber;
};

#endif // OFXDSHOW_H_INCLUDED
