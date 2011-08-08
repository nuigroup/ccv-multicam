/*
*  ofxffmv.cpp
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/

#include "ofxDShow.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


ofxDShow::ofxDShow()
{	
	//vidGrabber = NULL;
}
ofxDShow::~ofxDShow()
{
	deinitializeCamera();
}

void ofxDShow::setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled)
{
	if (isVideoPlaying)
		return;
	//vidGrabber->setCameraFeature((int)guid.Data1,featureCode,firstValue,secondValue,isAuto,isEnabled);
}

void ofxDShow::getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled,int* minValue,int* maxValue)
{
	//vidGrabber->getCameraFeature((int)guid.Data1,featureCode,firstValue,secondValue,isAuto,isEnabled,minValue,maxValue);
}

int ofxDShow::getCameraBaseCount()
{
	//return vidGrabber->getDeviceCount();
	return 0;
}

void ofxDShow::getNewFrame(unsigned char* newFrame)
{
	//vidGrabber->grabFrame();
	////isNewFrame = vidGrabber->isFrameNew();
	//memcpy((void*)cameraFrame,vidGrabber->getPixels(),width * height * depth * sizeof(unsigned char));
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
