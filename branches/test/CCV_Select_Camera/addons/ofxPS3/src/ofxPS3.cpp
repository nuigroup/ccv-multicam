/*
*  ofxPS3.cpp
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#include "ofxPS3.h"

ofxPS3::ofxPS3() 
{
	cameraFrame = 0;
	ps3EyeCamera = NULL;
}

ofxPS3::~ofxPS3() 
{
	deinitializeCamera();
}

void ofxPS3::getNewFrame(unsigned char* newFrame)
{
	CLEyeCameraGetFrame(ps3EyeCamera,(PBYTE)newFrame, 1000);
}

void ofxPS3::cameraInitializationLogic()
{
	cameraType = PS3;
	int camNum = CLEyeGetCameraCount();
	for (int i=0;i<camNum;i++)
	{
		if (CLEyeGetCameraUUID(i) == guid)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
		return;
	ps3EyeCamera = CLEyeCreateCamera(guid, (depth == 1) ? CLEYE_MONO_RAW : CLEYE_COLOR_RAW, width * height > 320 * 240 ? CLEYE_VGA : CLEYE_QVGA, framerate);
	Sleep(300);
	CLEyeCameraStart(ps3EyeCamera);
	Sleep(100);
}
void ofxPS3::setCameraType()
{
	cameraType = PS3;
}

void ofxPS3::cameraDeinitializationLogic()
{
	CLEyeCameraStop(ps3EyeCamera);
	Sleep(50);
	CLEyeDestroyCamera(ps3EyeCamera);
	ps3EyeCamera = NULL;
}

void ofxPS3::setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled)
{
	if ((ps3EyeCamera == NULL) || (!isInitialized))
		return;
	CLEyeCameraParameter deviceProperty = (CLEyeCameraParameter)0xFFFFFFFF;
	switch (featureCode)
	{
		case BASE_GAIN:
			CLEyeSetCameraParameter(ps3EyeCamera,CLEYE_AUTO_GAIN,isAuto);
			deviceProperty = CLEYE_GAIN;
			break;
		case BASE_EXPOSURE:
			CLEyeSetCameraParameter(ps3EyeCamera,CLEYE_AUTO_EXPOSURE,isAuto);
			deviceProperty = CLEYE_EXPOSURE;
			break;
	}
	if (deviceProperty!=0xFFFFFFFF)
	{
		CLEyeSetCameraParameter(ps3EyeCamera,deviceProperty,firstValue);
	}
}

void ofxPS3::getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled,int* minValue,int* maxValue)
{
	CLEyeCameraParameter deviceProperty = (CLEyeCameraParameter)0xFFFFFFFF;
	switch (featureCode)
	{
	case BASE_GAIN:
		deviceProperty = CLEYE_GAIN;
		*minValue = 0;
		*maxValue = 79;
		break;
	case BASE_EXPOSURE:
		deviceProperty = CLEYE_EXPOSURE;
		*minValue = 0;
		*maxValue = 511;
		break;
	case BASE_WHITE_BALANCE:
		deviceProperty = CLEYE_WHITEBALANCE_RED;
		*minValue = 0;
		*maxValue = 255;
		break;
	}
	if ((deviceProperty!=0xFFFFFFFF) && (isInitialized) && (ps3EyeCamera!=NULL))
	{
		int parm = CLEyeGetCameraParameter(ps3EyeCamera,deviceProperty);
		*isEnabled = true;
		*firstValue = parm;
		*secondValue = false;
		*isAuto = false;
	}	
	else
	{
		*isEnabled = false;
	}	
}

int ofxPS3::getCameraBaseCount()
{
	return CLEyeGetCameraCount();
}
