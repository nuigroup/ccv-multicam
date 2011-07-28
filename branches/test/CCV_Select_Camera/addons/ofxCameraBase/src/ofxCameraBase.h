/*
*  ofxCameraBase.h
*  
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#ifndef OFX_CAMERABASE_H
#define OFX_CAMERABASE_H

#define _MAX_CAMERAS_ 16
#define _MAX_FRAME_LIFETIME_ 3
#include <windows.h>
#include "ofxCameraBaseSettings.h"
#include "ofMain.h"
#include "ofxGUIDHelper.h"
//#include "ofxVideoWriter.h"

class ofxCameraBase
{
public:
	//Constructor for CameraBase class
	ofxCameraBase()
	{
		depth = 1;
		width = 640;
		height = 480;
		left = 0;
		top = 0;
		framerate = 30;
		isInitialized = false;
		index = 0;
		memset((void*)&guid,0,sizeof(GUID));
		isVideoPlaying = false;
		isVideoRecording = false;
		videoPlayer = NULL;
		//videoWriter = NULL;
		captureThread = NULL;
		isCaptureThreadRunning = false;
		isNewFrame = true;
		newFrameCurrentLifetime = 0;
		cameraPixelMode = MODE1;
	}
	//Virtual destructor for CameraBase class
	virtual ~ofxCameraBase(){}
	//Initialization procedure that get
	//GUID cameraGuid - global idetifier of camera. PS3 Eye uses all GUID data structure, but FFMV, CMU and etc. uses only 64 bit for global identifier. For these cameras use first 64 bits of GUID (Data1)
	//int cameraWidth - set camera frame width property
	//int cameraHeight - set camera frame height property
	//int cameraLeft - set frame left offset if camera supports ROI mode (FFMV and etc.)
	//int cameraTop - set frame top offset if camera supports ROI mode (FFMV and etc. )
	//unsigned char cameraDepth - set pixel depth for camera frame. Preferred 1 byte for 1 pixel.
	//int cameraFramerate - set framerate for camera. This value could be in absolute mode (for FFMV - 1275 is about 50 fps).
	//bool isEmulating - if some video was captured from this camera setting this parameter to true will start reading avi file and emulating camera work.
	//PIXEL_MODE pixelMode - some cameras uses different pixel decoding mode - for more info - look at this structure decription
	void initializeWithGUID(GUID cameraGuid,int cameraWidth,int cameraHeight,int cameraLeft,int cameraTop,unsigned char cameraDepth,int cameraFramerate,bool isEmulating = false,PIXEL_MODE pixelMode = MODE0);
	//Deinitialization of CameraBase class
	void deinitializeCamera();
	//setting base feature to CameraBase class
	virtual void setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled){}
	//get current value of feature with min and max value
	virtual void getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled,int* minValue,int* maxValue) {}
	//get number of connected cameras
	virtual int getCameraBaseCount() {return 0;}	
	////start video recording from this camera. Due to some specific of video coding/decoding - video will be recorded with 30 fps
	//void startRecording();
	////stop video recording from this camera. Due to some specific of video coding/decoding - video will be recorded with 30 fps
	//void stopRecording();
	//logic for calling camera settings dialog
	void callCameraSettingsDialog();
	//public getter of camera frame size and depth
	void getCameraSize(unsigned int* cameraWidth,unsigned int* cameraHeight,unsigned char* cameraDepth);
	//public getter of cameraFrame
	void getCameraFrame(unsigned char* newFrameData);	
	//public getter of camera index position
	int getCameraIndex(){return index;}
	//public getter of camera global identifier
	GUID getCameraGUID(){return guid;}
	//public getter of camera initialized status
	bool isCameraInitialized() { return isInitialized; }
	//public getter of camera isNewFrame property
	bool isCapturedNewFrame() { return isNewFrame;}
	//start camera logic
	void startCamera();
protected:
	//logic for updating current frame
	void updateCurrentFrame();
	//capturing thread logic
	static DWORD WINAPI CaptureThread(LPVOID instance);
	//capturing thread start
	void StartThreadingCapture();
	//capturing thread stop
	void StopThreadingCapture();
	//specific logic for getting frame from each camera
	virtual void getNewFrame(unsigned char* newFrame) {}
	//specific logic for initialization camera
	virtual void cameraInitializationLogic() {}
	//specific logic for deinitialization camera
	virtual void cameraDeinitializationLogic() {}
	//setting camera type
	virtual void setCameraType() {}
private:
	void Capture();
protected:
	HANDLE captureThread;
	bool isCaptureThreadRunning;
	CRITICAL_SECTION criticalSection;
	GUID guid;
	CAMERATYPE cameraType;
	unsigned int width,height,framerate;
	int index,left,top;
	unsigned char depth;
	bool isInitialized;
	bool isVideoPlaying,isVideoRecording;
	bool isNewFrame;
	int newFrameCurrentLifetime;
	PIXEL_MODE cameraPixelMode;
	unsigned char* cameraFrame;
	unsigned char* videoFrame;
	ofVideoPlayer* videoPlayer;
	//ofxVideoWriter* videoWriter;
};

#endif // OFX_CAMERABASE_H