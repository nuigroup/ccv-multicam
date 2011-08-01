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
	int getCameraBaseCount();
	unsigned char* getCameraFrame();
	void setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled);
	void getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled);
	void callCameraSettingsDialog();
	void initializeWithGUID(GUID cameraGuid,int cameraWidth,int cameraHeight,int cameraLeft,int cameraTop,unsigned char cameraDepth,int cameraFramerate,bool isEmulating,PIXEL_MODE pixelMode);
	void deinitializeCamera();
	void updateCurrentFrame();
	void setVideoRecordingMode(bool isRecording);
private:
	unsigned char* cameraFrame;
	unsigned char* videoFrame;
	CLEyeCameraInstance ps3EyeCamera;
};
#endif // OFXPS3_H_
