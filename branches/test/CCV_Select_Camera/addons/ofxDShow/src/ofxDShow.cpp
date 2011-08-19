/*
*  ofxffmv.cpp
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*  Edited by: Yishi Guo
*
*/

#include "ofxDShow.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


ofxDShow::ofxDShow()
{
	cameraFrame = NULL;
	VI = NULL;
	devicePath = NULL;
	deviceId = -1;

	cameraType = DIRECTSHOW;
}

ofxDShow::~ofxDShow()
{
	deinitializeCamera();
}

void ofxDShow::setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled)
{
	if (isVideoPlaying)
		return;
	if (isInitialized) 
	{
		long filterSettings = -1;
		long deviceSettings = -1;
		switch (featureCode)
		{
		case BASE_BRIGHTNESS:
			filterSettings = VI->propBrightness;
			break;
		case BASE_CONTRAST:
			filterSettings = VI->propContrast;
			break;
		case BASE_HUE:
			filterSettings = VI->propHue;
			break;
		case BASE_GAMMA:
			filterSettings = VI->propGamma;
			break;
		case BASE_GAIN:
			filterSettings = VI->propGain;
			break;
		case BASE_SATURATION:
			filterSettings = VI->propSaturation;
			break;
		case BASE_SHARPNESS:
			filterSettings = VI->propSharpness;
			break;
		case BASE_WHITE_BALANCE:
			filterSettings = VI->propWhiteBalance;
			break;

		case BASE_PAN:
			deviceSettings = VI->propPan;
			break;
		case BASE_TILT:
			deviceSettings = VI->propTilt;
			break;
		case BASE_ROLL:
			deviceSettings = VI->propRoll;
			break;
		case BASE_ZOOM:
			deviceSettings = VI->propZoom;
			break;
		case BASE_EXPOSURE:
			deviceSettings = VI->propExposure;
			break;
		case BASE_IRIS:
			deviceSettings = VI->propIris;
			break;
		case BASE_FOCUS:
			deviceSettings = VI->propFocus;
			break;
		}
		if (filterSettings>=0)
			VI->setVideoSettingFilter(deviceId,filterSettings,firstValue);
		if (deviceSettings>=0)
			VI->setVideoSettingCamera(deviceId,filterSettings,firstValue);
	}
}

void ofxDShow::getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled,int* minValue,int* maxValue)
{
	if ( isVideoPlaying ) {
		return;
	}
	if (isInitialized) 
	{
		long filterSettings = -1;
		long deviceSettings = -1;
		switch (featureCode)
		{
		case BASE_BRIGHTNESS:
			filterSettings = VI->propBrightness;
			break;
		case BASE_CONTRAST:
			filterSettings = VI->propContrast;
			break;
		case BASE_HUE:
			filterSettings = VI->propHue;
			break;
		case BASE_GAMMA:
			filterSettings = VI->propGamma;
			break;
		case BASE_GAIN:
			filterSettings = VI->propGain;
			break;
		case BASE_SATURATION:
			filterSettings = VI->propSaturation;
			break;
		case BASE_SHARPNESS:
			filterSettings = VI->propSharpness;
			break;
		case BASE_WHITE_BALANCE:
			filterSettings = VI->propWhiteBalance;
			break;
		case BASE_PAN:
			deviceSettings = VI->propPan;
			break;
		case BASE_TILT:
			deviceSettings = VI->propTilt;
			break;
		case BASE_ROLL:
			deviceSettings = VI->propRoll;
			break;
		case BASE_ZOOM:
			deviceSettings = VI->propZoom;
			break;
		case BASE_EXPOSURE:
			deviceSettings = VI->propExposure;
			break;
		case BASE_IRIS:
			deviceSettings = VI->propIris;
			break;
		case BASE_FOCUS:
			deviceSettings = VI->propFocus;
			break;
		}
		long delta,flags,defaultValue,MinValue,MaxValue,CurrentValue;
		if (filterSettings>=0)
		{
			VI->getVideoSettingFilter(deviceId,filterSettings,MinValue,MaxValue,delta,CurrentValue,delta,defaultValue);
			*firstValue = CurrentValue;
			*minValue = MinValue;
			*maxValue = MaxValue;
			*secondValue = 0;
		}
		if (deviceSettings>=0)
		{
			VI->getVideoSettingCamera(deviceId,deviceSettings,MinValue,MaxValue,delta,CurrentValue,delta,defaultValue);
			*firstValue = CurrentValue;
			*minValue = MinValue;
			*maxValue = MaxValue;
			*secondValue = 0;
		}
	}
}

int ofxDShow::getCameraBaseCount()
{
	return VI->getDeviceCount();
}

void ofxDShow::initializeWithDevicePath( char* devicePath, int cameraWidth, int cameraHeight, int cameraLeft, int cameraTop, unsigned char cameraDepth, int cameraFramerate, bool isEmulating, PIXEL_MODE pixelMode ) {
	if ( isInitialized ) {
		return;
	}
	this->devicePath = devicePath;
	this->width = cameraWidth;
	this->height = cameraHeight;
	this->left = cameraLeft;
	this->top = cameraTop;
	this->depth = cameraDepth;
	this->framerate = cameraFramerate;
	this->isVideoPlaying = isEmulating;

	if ( isEmulating ) {
		// TODO
	} else {
		if ( VI == NULL ) {
			VI = new videoInput();
		}
		this->deviceId = VI->getDeviceId( this->devicePath );
		if ( deviceId >= 0 ) {
			VI->setIdealFramerate( deviceId, framerate );
			VI->setupDevice( deviceId, width, height );
			isInitialized = true;
		} else {
			return;
		}
	}

	cameraFrame = new unsigned char[width*height*depth];
}

unsigned char* ofxDShow::getCameraFrame() {
	if ( !isInitialized ) {
		return NULL;
	}

	updateCurrentFrame();
	return cameraFrame;
}

void ofxDShow::deinitializeCamera() {
	// TODO
	// Stop the videoInput
	if ( deviceId >= 0 ) {
		VI->stopDevice( deviceId );
	}
}

void ofxDShow::getNewFrame(unsigned char* newFrame)
{
	//vidGrabber->grabFrame();
	////isNewFrame = vidGrabber->isFrameNew();
	//memcpy((void*)cameraFrame,vidGrabber->getPixels(),width * height * depth * sizeof(unsigned char));
}

void ofxDShow::updateCurrentFrame() {
	if ( isVideoPlaying ) {
		// TODO
	} else {
		if ( deviceId >= 0
			&& VI->isFrameNew( deviceId ) ) {
			//VI->getPixels( deviceId, cameraFrame );
				VIFrame = VI->getPixels(deviceId, false, true);
				for ( int i = 0; i < width * height; ++i ) {
					cameraFrame[i] = 0.21f * VIFrame[3*i+0] + 0.71f * VIFrame[3*i+1] + 0.07f * VIFrame[3*i+2];
				}
			//memcpy((void*)cameraFrame,VI->getPixels(deviceId),width * height * depth * sizeof(unsigned char));
		}
	}
}

void ofxDShow::setCameraType()
{
	cameraType = DIRECTSHOW;
}
void ofxDShow::cameraInitializationLogic()
{
	//vidGrabber = new ofxVideoGrabber();
	//vidGrabber->listDevices();
	//vidGrabber->setDeviceID(guid.Data1 >= getCameraBaseCount() ? 0 : guid.Data1);
	//vidGrabber->setVerbose(true);
	//vidGrabber->setDesiredFrameRate(framerate);
	//bool  isInit = vidGrabber->initGrabber(width,height);
	//width = vidGrabber->width;
	//height = vidGrabber->height;
}

void ofxDShow::cameraDeinitializationLogic()
{
	//if (vidGrabber!=NULL)
	//{
	//	vidGrabber->close();
	//	delete vidGrabber; vidGrabber = NULL;
	//}
}