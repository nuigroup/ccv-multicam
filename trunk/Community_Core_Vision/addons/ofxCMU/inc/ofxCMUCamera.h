/*
*  ofxCMUCamera.h
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#ifndef _OFX_CMU_CAMERA_
#define _OFX_CMU_CAMERA_

#include <windows.h>
#include <process.h>
#include <mmsystem.h>
#include <stdio.h>
#include "1394Camera.h"
#include "ofxCameraBase.h"

#define RATE_CODE 4
#define FORMAT0_CODE 0
#define FORMAT1_CODE 5

class ofxCMUCamera : ofxCameraBase
{
public:
	ofxCMUCamera()
	{
		theCamera = NULL;
	}
	~ofxCMUCamera()
	{
	}
	int getCameraBaseCount();
	void setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled);
	void initializeWithGUID(GUID cameraGuid,int cameraWidth,int cameraHeight,int cameraLeft,int cameraTop,bool useROI,unsigned char cameraDepth,int cameraFramerate,bool isPlaying = false);
	void deinitializeCamera();
	void updateCurrentFrame();
	void getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled);
	void callCameraSettingsDialog();
	unsigned char* getCameraFrame();
	C1394Camera* theCamera;
private:
	unsigned char* currentFrame;
};

#endif//_OFX_CMU_CAMERA_