/*
*  ofxPS3.h
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#ifndef OFXPS3_H
#define OFXPS3_H

#include "ofxCameraBase.h"
#include "CLEyeMulticam.h"

class ofxPS3 : ofxCameraBase
{
public:
	ofxPS3();
	~ofxPS3();
	void setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled);
	void getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled,int* minValue,int* maxValue);
	int getCameraBaseCount();
	CAMERATYPE getCameraType();
protected:
	void getNewFrame(unsigned char* newFrame);
	void cameraInitializationLogic();
	void cameraDeinitializationLogic();
	void setCameraType();
private:
	CLEyeCameraInstance ps3EyeCamera;
};
#endif // OFXPS3_H_
