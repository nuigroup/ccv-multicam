/*
*  ofxMultiplexer.h
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/

#ifndef _OFX_MULTIPLEXER_
#define _OFX_MULTIPLEXER_

#include <stdio.h>
#include <omp.h>
#include <memory.h>
#include <vector>
#include "omp.h"
#include "ofxCameraBase.h"
#include "ofxCameraBaseSettings.h"
#include "ofxffmv.h"
#include "ofxPS3.h"
//#include "ofxUnibrain.h"
#include "ofxCMUCamera.h"
#include "ofxGUIDHelper.h"
#include "ofxXMLSettings.h"
#include "Calibration.h"
#include "Filters\Filters.h"
//#include "ofxNCore.h"

#define NULL_CAMERA 0xFF


class ofxMultiplexer
{
public:
	~ofxMultiplexer()
	{
		for (int i=0;i<cameraGridWidth*cameraGridHeight;i++)
			if ((cameras!=NULL) && (cameras[i]!=NULL))
				cameras[i]->deinitializeCamera();
	}
	void initializeMultiplexer();
	void deinitializeMultiplexer();
	unsigned char* getCapturedFrameFromCamera(unsigned int* frameWidth,unsigned int* frameHeight,unsigned char* frameDepth,unsigned char camID);
	unsigned char* getGPUStitchedFrame(unsigned int* frameWidth,unsigned int* frameHeight);	
	unsigned char* getCPUStitchedFrame(unsigned int* frameWidth,unsigned int* frameHeight);	
	int getStitchedFrameWidth(){ return stitchedFrameWidth; }
	int getStitchedFrameHeight(){ return stitchedFrameHeight; }
	int getCameraCount(){return cameraCount;}
	bool isMultiplexerInitialized() { return isInitialized;}
	bool isGPUProcessing() { return useGPUProcessing;}
	bool isNeedToBeBackgroundUpdated(){if (backgroundUpdate){backgroundUpdate = false;return true;}return false;}
	void setInterleaveMode(bool isInterleave);
	void setCalibratingMode(bool isCalibrating);
	void setCalibrationCamera(int cameraIndex);
	bool getInterleaveMode() { return isInterleaveMode;}
	bool getCalibratingMode() { return isCalibratingMode;}
	int getCalibrationCamera() { return calibrationCameraIndex;}
	void setFilters(Filters* filter)
	{
		this->filter = filter;
	}
	ofxCameraBaseSettings* cameraSettings;
	int cameraGridWidth,cameraGridHeight,cameraCount,stitchedFrameWidth,stitchedFrameHeight,calibrationGridWidth,calibrationGridHeight,stitchedFrameFramerate;
	void changeRecordingMode();
	ofxMultiplexer()
	{
		isInitialized = false;
		cameraCount = 1;
		stitchedFrameWidth = 640;
		stitchedFrameHeight = 380;
		calibrationGridWidth = 33;
		calibrationGridHeight = 19;
		cameras = NULL;
		cameraSettings = NULL;
		useGPUProcessing = false;
		stitchedFrameFramerate = 60;
		cameraType = FFMV;
		cpuStitchedFrame = NULL;
		weightTextureCPU = NULL;
		offsetTextureCPU = NULL;
		//calibrationSystem = NULL;
		levelTextureCPU = NULL;
		cameraGridWidth = 1;
		cameraGridHeight = 1;
		isInterleaveMode = false;
		isCalibratingMode = false;
		backgroundUpdate = false;
		calibrationCameraIndex = NULL_CAMERA;
		calibration = Calibration::getInstance();
		isRecordingMode = false;
	//	application = ofxNCoreVision::Instance();
	}
private:
	int calibrationCameraIndex;
	void updateCameraMap();
	void initCPUMode();
	void initGPUMode();
	void initializeCameras();
	void applySettingsToCameras();
	void receiveSettingsFromCameras();
	void readSettingsFromXML(char* fileName="CameraSettings.xml");
	void saveSettingsToXML(char* fileName="CameraSettings.xml");
	void computeDistortion();
//	ofxCalibrationSystem* calibrationSystem;
	CAMERATYPE cameraType;
	bool isInitialized,useGPUProcessing;
	ofxCameraBase** cameras;
	short int*		  levelTextureCPU;
	unsigned int** offsetTextureCPU;
	float**		   weightTextureCPU;
	unsigned char* cpuStitchedFrame;
	unsigned char** cameraFrames;
	unsigned char** cameraMap;
	bool isInterleaveMode,isCalibratingMode,backgroundUpdate;
	Calibration* calibration;
	Filters* filter;
	bool isRecordingMode;
};

#endif//_OFX_MULTIPLEXER_