/*
*  ofxCMUCamera.cpp
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#include "ofxCMUCamera.h"
#include <iostream>
using namespace std;

int ofxCMUCamera::getCameraBaseCount()
{
    C1394Camera* theCamera = new C1394Camera();
	int cNum = theCamera->RefreshCameraList();
	delete theCamera;
	return cNum;
}

void ofxCMUCamera::initializeWithGUID(GUID cameraGuid,int cameraWidth,int cameraHeight,int cameraLeft,int cameraTop,bool useROI,unsigned char cameraDepth,int cameraFramerate,bool isPlaying)
{
	guid = cameraGuid;
	width = cameraWidth;
	height = cameraHeight;
	framerate = cameraFramerate;
	left = cameraLeft;
	top = cameraTop;
	roiMode = useROI;
	depth = cameraDepth;
	if (theCamera == NULL)
		theCamera = new C1394Camera();
	int camNum = theCamera->RefreshCameraList();
	int error_code = 0;
	unsigned long id = 0;
	for (int i=0;i<camNum;i++)
	{
		error_code = theCamera->SelectCamera(i);
		theCamera->GetCameraUniqueID((PLARGE_INTEGER)&id);
		if (error_code)	return;
		if (id == guid.Data1)
		{
			index = i;
			break;
		}
	}
	error_code = theCamera->SelectCamera(index);
	if (error_code)	return;
	error_code = theCamera->InitCamera(false);
	if (error_code)	return;
	error_code = theCamera->SetVideoFormat(FORMAT0_CODE);
	if (error_code)	return;
	error_code = theCamera->SetVideoMode(FORMAT1_CODE);
	if (error_code)	return;
	error_code = theCamera->SetVideoFrameRate(RATE_CODE);
	if (error_code)	return;
	theCamera->StartImageAcquisitionEx(6,0,ACQ_START_VIDEO_STREAM);
	theCamera->GetVideoDataDepth((unsigned short*)&depth);
	theCamera->GetVideoFrameDimensions((unsigned long*)&width,(unsigned long*)&height);
	currentFrame = (unsigned char*)malloc(width * height * depth * sizeof(unsigned char));
	memset((void*)currentFrame,0,width * height * depth * sizeof(unsigned char));
	isInitialized = (error_code == 0);
}

unsigned char* ofxCMUCamera::getCameraFrame()
{
	return currentFrame;
}

void ofxCMUCamera::setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled)
{
	CAMERA_FEATURE deviceProperty = (CAMERA_FEATURE)0xFFFFFFFF;
	switch (featureCode)
	{
	case BASE_BRIGHTNESS:
		deviceProperty = FEATURE_BRIGHTNESS;
		break;
	case BASE_EXPOSURE:
		deviceProperty = FEATURE_AUTO_EXPOSURE;
		break;
	case BASE_SHARPNESS:
		deviceProperty = FEATURE_SHARPNESS;
		break;
	case BASE_SATURATION:
		deviceProperty = FEATURE_SATURATION;
		break;
	case BASE_GAMMA:
		deviceProperty = FEATURE_GAMMA;
		break;
	case BASE_SHUTTER:
		deviceProperty = FEATURE_SHUTTER;
		break;
	case BASE_GAIN:
		deviceProperty = FEATURE_GAIN;
		break;
	case BASE_WHITE_BALANCE:
		deviceProperty = FEATURE_WHITE_BALANCE;
		break;
	}
	if ((deviceProperty!=0xFFFFFFFF) && (isInitialized))
	{
		C1394CameraControl* control = theCamera->GetCameraControl(deviceProperty);
	    if (control!=NULL)
		{
			control->SetValue(firstValue,secondValue);
			control->SetAutoMode(isAuto);
		}
	}
}
void ofxCMUCamera::deinitializeCamera()
{
	if (isInitialized)
	{
		if (theCamera!=NULL)
		{	
			theCamera->StopImageAcquisition();
			delete theCamera;
		}
		theCamera = NULL;
		isInitialized = false;
		if (currentFrame!=NULL)
			free(currentFrame);
	}
}
void ofxCMUCamera::updateCurrentFrame()
{
	if (theCamera == NULL)
		return;
	HANDLE hFrameEvent;
    DWORD dwRet;
	bool localFrameNew = false;
	hFrameEvent = theCamera->GetFrameEvent();
	if(hFrameEvent == NULL)
	{
		if(theCamera->AcquireImageEx(FALSE,NULL) != CAM_ERROR_FRAME_TIMEOUT)
			return;
		hFrameEvent = theCamera->GetFrameEvent();
	}
	dwRet = MsgWaitForMultipleObjects(1,&hFrameEvent,FALSE,1000,QS_ALLEVENTS);
	switch(dwRet)
	{
		case WAIT_OBJECT_0:
			localFrameNew = true;
			if(theCamera->AcquireImageEx(FALSE,NULL) != CAM_SUCCESS)
				theCamera->StopImageAcquisition();
			break;
		case WAIT_OBJECT_0 + 1:
			break;
		case WAIT_TIMEOUT:
			break;
		default:
			theCamera->StopImageAcquisition();
			break;
	}
	if (localFrameNew)
	{
		theCamera->GetVideoFrameDimensions((unsigned long*)(&width),(unsigned long*)(&height));
		theCamera->getDIB(currentFrame,width*height*depth*sizeof(unsigned char));
	}
}

void ofxCMUCamera::getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled)
{
	CAMERA_FEATURE deviceProperty = (CAMERA_FEATURE)0xFFFFFFFF;
	switch (featureCode)
	{
		case BASE_BRIGHTNESS:
			deviceProperty = FEATURE_BRIGHTNESS;
			break;
		case BASE_EXPOSURE:
			deviceProperty = FEATURE_AUTO_EXPOSURE;
			break;
		case BASE_SHARPNESS:
			deviceProperty = FEATURE_SHARPNESS;
			break;
		case BASE_SATURATION:
			deviceProperty = FEATURE_SATURATION;
			break;
		case BASE_GAMMA:
			deviceProperty = FEATURE_GAMMA;
			break;
		case BASE_SHUTTER:
			deviceProperty = FEATURE_SHUTTER;
			break;
		case BASE_GAIN:
			deviceProperty = FEATURE_GAIN;
			break;
		case BASE_WHITE_BALANCE:
			deviceProperty = FEATURE_WHITE_BALANCE;
			break;
	}
	if ((deviceProperty!=0xFFFFFFFF) && (isInitialized))
	{
		C1394CameraControl* control = theCamera->GetCameraControl(deviceProperty);
	    *isEnabled = (control!=NULL);
		if (control!=NULL)
		{
			*isAuto = control->StatusAutoMode();
			unsigned short first;
			unsigned short second;
			control->GetValue(&first,&second);
			*firstValue = first;
			*secondValue = second;
		}
	}
	else
		*isEnabled = false;

}

ofxCMUCamera* cmu = NULL;
DWORD iProcess = 0;

BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD id=GetWindowThreadProcessId(hwnd,&id);
	if(GetWindowThreadProcessId(hwnd,&id)==iProcess)
	{
		CameraControlDialog(hwnd,cmu->theCamera,TRUE);
		return false;
	}
	return true;
}


void ofxCMUCamera::callCameraSettingsDialog()
{
	iProcess = GetProcessId(GetCurrentProcess());
	cmu = this;
	EnumWindows(enumWindowsProc, 0);
}