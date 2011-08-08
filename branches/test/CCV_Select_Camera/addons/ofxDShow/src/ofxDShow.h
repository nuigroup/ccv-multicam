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
//#include "ofxVideoGrabber.h"

class ofxDShow : ofxCameraBase
{
public:
	ofxDShow();
	~ofxDShow();
	void setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled);
	void getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled,int* minValue,int* maxValue);
	int getCameraBaseCount();
protected:
	void getNewFrame(unsigned char* newFrame);
	void cameraInitializationLogic();
	void cameraDeinitializationLogic();
	void setCameraType();
private:
	//ofxVideoGrabber*	vidGrabber;
};

#endif // OFXDSHOW_H_INCLUDED
