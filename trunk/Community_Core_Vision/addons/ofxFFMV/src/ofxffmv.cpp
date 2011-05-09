/*
*  ofxffmv.cpp
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#include "ofxffmv.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#pragma warning(disable : 4018)	// signed/unsigned mismatch

ofxffmv::ofxffmv()
{

}

int ofxffmv::getCameraBaseCount()
{
   unsigned int tcamNum = 0;
   FlyCaptureInfoEx* tArInfo = new FlyCaptureInfoEx[_MAX_CAMERAS_];
   flycaptureBusEnumerateCamerasEx( tArInfo, &tcamNum );
   delete tArInfo;
   return tcamNum;
}

void ofxffmv::initializeWithGUID(GUID cameraGuid,int cameraWidth,int cameraHeight,int cameraLeft,int cameraTop,bool useROI,unsigned char cameraDepth,int cameraFramerate,bool isPlaying)
{
	FlyCaptureInfoEx* tempArInfo = new FlyCaptureInfoEx[_MAX_CAMERAS_];
	unsigned int camNum = 0;
	flycaptureBusEnumerateCamerasEx( tempArInfo, &camNum );
	for (int i=0;i<camNum;i++)
	{
		if ((unsigned long)tempArInfo[i].SerialNumber == cameraGuid.Data1)
			index = i;
	}
	arInfo = tempArInfo[index];
	flycaptureCreateContext(&context);
	flycaptureInitialize( context, index );
	guid = cameraGuid;
	width = cameraWidth;
	height = cameraHeight;
	framerate = cameraFramerate;
	left = cameraLeft;
	top = cameraTop;
	roiMode = useROI;
	depth = cameraDepth;
	flycaptureStartCustomImage(context,0,left,top,width,height,100,FLYCAPTURE_MONO8);
	flycaptureGrabImage2( context, &fcImage );
	width = fcImage.iCols;
	height = fcImage.iRows;
	depth = (width*height!=0) ? fcImage.iRowInc/fcImage.iRows : 0;
	flycaptureSetColorProcessingMethod( context,FLYCAPTURE_DISABLE);
	isInitialized = true;
	delete tempArInfo;
}

unsigned char* ofxffmv::getCameraFrame()
{
	return fcImage.pData;
}

void ofxffmv::setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled)
{
	FlyCaptureProperty deviceProperty = (FlyCaptureProperty)0xFFFFFFFF;
	switch (featureCode)
	{
	case BASE_BRIGHTNESS:
		deviceProperty = FLYCAPTURE_BRIGHTNESS;
		break;
	case BASE_EXPOSURE:
		deviceProperty = FLYCAPTURE_AUTO_EXPOSURE;
		break;
	case BASE_SATURATION:
		deviceProperty = FLYCAPTURE_SATURATION;
		break;
	case BASE_GAMMA:
		deviceProperty = FLYCAPTURE_GAMMA;
		break;
	case BASE_SHUTTER:
		deviceProperty = FLYCAPTURE_SHUTTER;
		break;
	case BASE_GAIN:
		deviceProperty = FLYCAPTURE_GAIN;
		break;
	case BASE_FRAMERATE:
		deviceProperty = FLYCAPTURE_FRAME_RATE;
		break;
	case BASE_PAN:
		deviceProperty = FLYCAPTURE_PAN;
		break;
	}
	if ((deviceProperty!=0xFFFFFFFF) && (isInitialized))
		flycaptureSetCameraProperty(context,deviceProperty,firstValue,secondValue,isAuto);
}

void ofxffmv::deinitializeCamera()
{
	if (isInitialized)
	{
		flycaptureStop( context );
		flycaptureDestroyContext( context );
		isInitialized = false;
	}
}

void ofxffmv::updateCurrentFrame()
{
	flycaptureGrabImage2(context,&fcImage);
}


ofxffmv::~ofxffmv()
{
	deinitializeCamera();
}

void ofxffmv::getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled)
{
	FlyCaptureProperty deviceProperty = (FlyCaptureProperty)0xFFFFFFFF;
	switch (featureCode)
	{
	case BASE_BRIGHTNESS:
		deviceProperty = FLYCAPTURE_BRIGHTNESS;
		break;
	case BASE_EXPOSURE:
		deviceProperty = FLYCAPTURE_AUTO_EXPOSURE;
		break;
	case BASE_SATURATION:
		deviceProperty = FLYCAPTURE_SATURATION;
		break;
	case BASE_GAMMA:
		deviceProperty = FLYCAPTURE_GAMMA;
		break;
	case BASE_SHUTTER:
		deviceProperty = FLYCAPTURE_SHUTTER;
		break;
	case BASE_GAIN:
		deviceProperty = FLYCAPTURE_GAIN;
		break;
	case BASE_FRAMERATE:
		deviceProperty = FLYCAPTURE_FRAME_RATE;
		break;
	case BASE_PAN:
		deviceProperty = FLYCAPTURE_PAN;
		break;
	}
	if ((deviceProperty!=0xFFFFFFFF) && (isInitialized))
		*isEnabled = (flycaptureGetCameraProperty(context,deviceProperty,(long*)firstValue,(long*)secondValue,isAuto) == FLYCAPTURE_OK);
	else
		*isEnabled = false;

}
void ofxffmv::callCameraSettingsDialog()
{
	
}
