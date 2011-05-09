/*
*  ofxffmv.h
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#ifndef OFXFFMV_H_INCLUDED
#define OFXFFMV_H_INCLUDED

#include "ofxCameraBase.h"
#include "pgrflycapture.h"

class ofxffmv : ofxCameraBase
{
    public:
        ofxffmv();
		~ofxffmv();
		int getCameraBaseCount();
	    void setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled);
		void initializeWithGUID(GUID cameraGuid,int cameraWidth,int cameraHeight,int cameraLeft,int cameraTop,bool useROI,unsigned char cameraDepth,int cameraFramerate,bool isPlaying = false);
		void deinitializeCamera();
		void updateCurrentFrame();
		void getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled);
		void callCameraSettingsDialog();
		unsigned char* getCameraFrame();
    private:
	    FlyCaptureImage fcImage;
		FlyCaptureContext context;
        FlyCaptureInfoEx  arInfo;
};
#endif // OFXFFMV_H_INCLUDED
