/*
*  ofxCameraBase.h
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#ifndef OFX_CAMERABASE_H
#define OFX_CAMERABASE_H

#define _MAX_CAMERAS_ 16
#include <windows.h>
#include "ofxCameraBaseSettings.h"
//#include "ofMain.h"
#include "ofxGUIDHelper.h"
#include "ofVideoPlayer.h"
//#include "ofxVideoWriter.h"

class ofxCameraBase
{
public:
	virtual int getCameraBaseCount(){return 0;}
	virtual void setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled){}
	virtual void getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled) {}
	virtual void callCameraSettingsDialog() {}
	virtual void initializeWithGUID(GUID cameraGuid,int cameraWidth,int cameraHeight,int cameraLeft,int cameraTop,unsigned char cameraDepth,int cameraFramerate,bool isEmulating = false,PIXEL_MODE pixelMode = MODE0) {}
	virtual void deinitializeCamera(){}
	virtual void updateCurrentFrame(){}
	virtual void setVideoRecordingMode(bool isRecording){}
	virtual unsigned char* getCameraFrame()	{ return 0; }
	void getCameraSize(unsigned int* cameraWidth,unsigned int* cameraHeight,unsigned char* cameraDepth)
	{
		if (!isInitialized)
		{
			*cameraWidth = *cameraHeight = *cameraDepth = 0;
			return;
		}
		*cameraWidth = width;
		*cameraHeight = height;
		*cameraDepth = depth;
	}
	void getCameraRoi(bool* isRoiMode,int* cameraLeft,int* cameraTop,unsigned int* cameraWidth,unsigned int* cameraHeight,unsigned char* cameraDepth)
	{
		getCameraSize(cameraWidth,cameraHeight,cameraDepth);
		*isRoiMode = roiMode;
		*cameraLeft = left;
		*cameraTop = top;
	}
	int getCameraIndex(){return index;}
	GUID getCameraGUID(){return guid;}
	// Added by Yishi Guo
	// BEGIN
	CAMERATYPE getCameraType() {return cameraType;}
	unsigned int getCameraFramerate() {return framerate;}
	// END
	bool isCameraInitialized() { return isInitialized; }
	ofxCameraBase()
	{
		depth = 1;
		width = 640;
		height = 480;
		left = 0;
		top = 0;
		framerate = 30;
		roiMode = false;
		isInitialized = false;
		index = 0;
		memset((void*)&guid,0,sizeof(GUID));
		isVideoPlaying = false;
		isVideoRecording = false;
		videoPlayer = NULL;
		//videoWriter = NULL;
	}
protected:
	GUID guid;
	CAMERATYPE cameraType;
	unsigned int width,height,framerate;
	int index,left,top;
	unsigned char depth;
	bool roiMode,isInitialized;
	bool isVideoPlaying,isVideoRecording;
	ofVideoPlayer* videoPlayer;
	//ofxVideoWriter* videoWriter;
};


#endif // OFX_CAMERABASE_H