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

	cameraType = PS3;
}

ofxPS3::~ofxPS3() 
{
	deinitializeCamera();
}

void ofxPS3::setVideoRecordingMode(bool isRecording)
{
	if (isVideoPlaying)
		return;
	if (isVideoRecording != isRecording)
	{
		//if (isRecording)
		//{
		//	videoWriter = new ofxVideoWriter();
		//	std::string filePath = "data/record/" +  GUIDToString(guid) + ".avi";
		//	videoWriter->open(filePath.c_str(),width,height,24,0.7*(double)framerate);
		//}
		//else
		//{
		//	if (videoWriter!=NULL)
		//	{
		//		videoWriter->close();
		//		videoWriter = NULL;
		//	}
		//}
	}
	isVideoRecording = isRecording;
}


int ofxPS3::getCameraBaseCount()
{
	return CLEyeGetCameraCount();
}

//void ofxPS3::initializeWithGUID(GUID cameraGuid,int cameraWidth,int cameraHeight,int cameraLeft,int cameraTop,bool useROI,unsigned char cameraDepth,int cameraFramerate,bool isPlaying)
void ofxPS3::initializeWithGUID(GUID cameraGuid,int cameraWidth,int cameraHeight,int cameraLeft,int cameraTop,unsigned char cameraDepth,int cameraFramerate,bool isEmulating,PIXEL_MODE pixelMode)
{
	if (isInitialized)
	{
#if DEBUG
		printf("Error: Initializing already initialized PS3 Eye Camera\n");
#endif
		return;
	}
	width = cameraWidth;
	height = cameraHeight;
	framerate = cameraFramerate;
	left = 0;
	top = 0;
	roiMode = false;
	depth = 1;
	framerate = cameraFramerate;
	guid = cameraGuid;
	isVideoPlaying = isEmulating;
	index = -1;
	if (isEmulating)
	{
		videoPlayer = new ofVideoPlayer();
		videoPlayer->loadMovie("record/" + GUIDToString(cameraGuid) + ".m4v");
		videoPlayer->play();
		videoPlayer->setLoopState(OF_LOOP_NORMAL);
	}
	else
	{
		int camNum = CLEyeGetCameraCount();
		for (int i=0;i<camNum;i++)
		{
			if (CLEyeGetCameraUUID(i) == cameraGuid)
			{
				index = i;
				break;
			}
		}
		if (index == -1)
			return;
		ps3EyeCamera = CLEyeCreateCamera(guid, CLEYE_MONO_PROCESSED, width * height > 320 * 240 ? CLEYE_VGA : CLEYE_QVGA,framerate);
		CLEyeCameraStart(ps3EyeCamera);
		int wid,hei;
		CLEyeCameraGetFrameDimensions(ps3EyeCamera, wid, hei);
	}
	cameraFrame = new unsigned char[width*height];
	videoFrame = new unsigned char[3*width*height];
	depth = 1;
	isInitialized = true;
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

void ofxPS3::deinitializeCamera()
{
	if (isInitialized)
	{
		if (isVideoPlaying)
		{
			delete videoPlayer;	videoPlayer = NULL;
		}
		else
		{
			CLEyeCameraStop(ps3EyeCamera);
			Sleep(50);
			CLEyeDestroyCamera(ps3EyeCamera);
			ps3EyeCamera = NULL;
		}
		delete [] cameraFrame;
		isInitialized = false;
	}
}

void ofxPS3::updateCurrentFrame()
{
	if (isVideoPlaying)
	{

		videoPlayer->idleMovie();
		bool bNewFrame = videoPlayer->isFrameNew();
		if (bNewFrame && videoPlayer->isLoaded())
		{
			unsigned char* data = videoPlayer->getPixels();
			for (int i=0;i<width*height;i++)
			{
				cameraFrame[i] = (unsigned char)(0.3f*(float)data[i*3+2] + 0.59f*(float)data[i*3+1] +0.11f*(float)data[i*3]) ;
			}
		}
	}
	else
	{
		CLEyeCameraGetFrame(ps3EyeCamera,(PBYTE)cameraFrame);

	//	if ((videoWriter!=NULL) && isVideoRecording)
	//	{
	//		for (int i=0;i<width*height;i++)
	//		{
	//			videoFrame[3*i+0] = cameraFrame[i];
	//			videoFrame[3*i+1] = cameraFrame[i];
	//			videoFrame[3*i+2] = cameraFrame[i];
	//		}
	//		videoWriter->appendFrame(videoFrame,false);
	//	}
	}
}

unsigned char* ofxPS3::getCameraFrame()
{
	updateCurrentFrame();
	return cameraFrame;
}

void ofxPS3::getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled)
{
	CLEyeCameraParameter deviceProperty = (CLEyeCameraParameter)0xFFFFFFFF;
	switch (featureCode)
	{
	case BASE_GAIN:
		deviceProperty = CLEYE_GAIN;
		break;
	case BASE_EXPOSURE:
		deviceProperty = CLEYE_EXPOSURE;
		break;
	case BASE_WHITE_BALANCE:
		deviceProperty = CLEYE_WHITEBALANCE_RED;
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

void ofxPS3::callCameraSettingsDialog()
{
	//need to be added
}

