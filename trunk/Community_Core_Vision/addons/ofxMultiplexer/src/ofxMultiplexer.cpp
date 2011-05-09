/*
*  ofxMultiplexer.cpp
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#include "ofxMultiplexer.h"

unsigned char* ofxMultiplexer::getCapturedFrameFromCamera(unsigned int* frameWidth,unsigned int* frameHeight,unsigned char* frameDepth,unsigned char camID)
{
	if ((!isInitialized) || (camID >= cameraCount))
		return NULL;
	cameras[camID]->updateCurrentFrame();
	cameras[camID]->getCameraSize(frameWidth,frameHeight,frameDepth);
	return cameras[camID]->getCameraFrame();
}

unsigned char* ofxMultiplexer::getGPUStitchedFrame(unsigned int* frameWidth,unsigned int* frameHeight)
{
	return NULL;
}

unsigned char* ofxMultiplexer::getCPUStitchedFrame(unsigned int* frameWidth,unsigned int* frameHeight)
{
	int size = stitchedFrameWidth*stitchedFrameHeight;
	*frameWidth = stitchedFrameWidth;
	*frameHeight = stitchedFrameHeight;
	
	for (int i=0;i<cameraCount;i++)
	{	
		cameras[i]->updateCurrentFrame();
	}
	for (int i=0;i<cameraCount;i++)
	{	
		cameraFrames[i] = cameras[i]->getCameraFrame();
	}
	if (isCalibratingMode)
	{
		if (isInterleaveMode)
		{
			for (int i=0;i<size;i++)
			{
				if ((cameraMap[0][i] == calibrationCameraIndex) || (cameraMap[1][i] == calibrationCameraIndex) || (cameraMap[2][i] == calibrationCameraIndex) || (cameraMap[3][i] == calibrationCameraIndex))
					cpuStitchedFrame[i] = cameraFrames[calibrationCameraIndex][offsetTextureCPU[calibrationCameraIndex][i]];
				else
					cpuStitchedFrame[i] = 0;
			}
		}
		else
		{
			for (int i=0;i<size;i++)
				cpuStitchedFrame[i] = calibrationCameraIndex == cameraMap[0][i] ? cameraFrames[cameraMap[0][i]][offsetTextureCPU[cameraMap[0][i]][i]] : 0;
		}
		return cpuStitchedFrame;
	}
	if (isInterleaveMode)
	{
		for (int i=0;i<size;i++)
		{
			if (cameraMap[1][i] == NULL_CAMERA)
			{
				cpuStitchedFrame[i] = cameraFrames[cameraMap[0][i]][offsetTextureCPU[cameraMap[0][i]][i]];
			}
			else
			{
				float result = 0;
				result+=((float)cameraFrames[0][offsetTextureCPU[0][i]]*weightTextureCPU[0][i]);
				result+=((float)cameraFrames[1][offsetTextureCPU[1][i]]*weightTextureCPU[1][i]);
				result+=((float)cameraFrames[2][offsetTextureCPU[2][i]]*weightTextureCPU[2][i]);
				result+=((float)cameraFrames[3][offsetTextureCPU[3][i]]*weightTextureCPU[3][i]);
				if (result>255.0f)
					result = 255.0f;
				cpuStitchedFrame[i] = (unsigned char)(result);
			}
		}
	}
	else
	{
		for (int i=0;i<size;i++)
		{
			cpuStitchedFrame[i] = cameraFrames[cameraMap[0][i]][offsetTextureCPU[cameraMap[0][i]][i]];
		}
	}
	return cpuStitchedFrame;
}


void ofxMultiplexer::initializeMultiplexer()
{
	readSettingsFromXML();
	initializeCameras();
	applySettingsToCameras();
	//for (int i=0;i<cameraCount;i++)
	//	cameras[i]->initializeWithGUID(cameraSettings[i].cameraGuid,cameraSettings[i].cameraWidth,cameraSettings[i].cameraHeight,cameraSettings[i].cameraLeft,cameraSettings[i].cameraTop,true,cameraSettings[i].cameraDepth,stitchedFrameFramerate);

	initCPUMode();
	initGPUMode();
	computeDistortion();
}
void ofxMultiplexer::deinitializeMultiplexer()
{
	receiveSettingsFromCameras();
	saveSettingsToXML();
}
void ofxMultiplexer::setInterleaveMode(bool isInterleave)
{
	int size = stitchedFrameWidth*stitchedFrameHeight;
	isInterleaveMode = isInterleave;
	if (cameraMap == NULL)
	{
		cameraMap = (unsigned char**)malloc(4 * sizeof(unsigned char*));
		for (int i=0;i<4;i++)
			cameraMap[i] = (unsigned char*)malloc(stitchedFrameWidth*stitchedFrameHeight*sizeof(unsigned char));
	}
	computeDistortion();
}

void ofxMultiplexer::setCalibratingMode(bool isCalibrating)
{
	bool isNewValue = (isCalibrating!=isCalibratingMode);
	isCalibratingMode = isCalibrating;
	if (isNewValue)
	{
		if (isCalibratingMode)
		{
			for (int i=0;i<cameraCount;i++)
			{
				cameraSettings[i].calibrationPoints.clear();
				float cameraCellWidth = ((float)cameraSettings[i].cameraWidth) / calibrationGridWidth;
				float cameraCellHeight = ((float)cameraSettings[i].cameraHeight) / calibrationGridHeight;
				for (int j = 0;j<(calibrationGridWidth+1)*(calibrationGridHeight+1);j++)
				{
					vector2df newPoint;
					newPoint.X = (j%(calibrationGridWidth+1)) * cameraCellWidth;
					newPoint.Y = (j/(calibrationGridWidth+1)) * cameraCellHeight;
					cameraSettings[i].calibrationPoints.push_back(newPoint);
				}
			}
		}
		else
		{
			cameraSettings[cameraGridWidth*cameraGridHeight-1].calibrationPoints.clear();
			for (int i=0;i<(calibrationGridWidth+1)*(calibrationGridHeight+1); i++)
			{
				cameraSettings[cameraGridWidth*cameraGridHeight-1].calibrationPoints.push_back(calibration->calibrate.cameraPoints[i]);
			}
			computeDistortion();
			saveSettingsToXML();
			calibration->calibrate.bCalibrating = false;
			calibration->calibrating = false;
		}
		filter->bLearnBakground = true;
	}
	else
	{
		if (isCalibratingMode)
		{
			cameraSettings[calibrationCameraIndex-1].calibrationPoints.clear();
			for (int i=0;i<(calibrationGridWidth+1)*(calibrationGridHeight+1); i++)
			{
				cameraSettings[calibrationCameraIndex-1].calibrationPoints.push_back(calibration->calibrate.cameraPoints[i]);
			}
		}
	}
}

void ofxMultiplexer::setCalibrationCamera(int cameraIndex)
{
	calibration->calibrate.bNextCamera = false;
	if (cameraIndex >= cameraGridWidth * cameraGridHeight)
	{
		calibrationCameraIndex = NULL_CAMERA;
		setCalibratingMode(false);
	}
	else
	{
		calibrationCameraIndex = cameraIndex;
		setCalibratingMode(true);
		calibration->calibrate.setCalibrationCamera(cameraIndex);
	}
//	setCalibratingMode(calibrationCameraIndex!=NULL_CAMERA);
	filter->bLearnBakground = true;
}

void ofxMultiplexer::readSettingsFromXML(char* fileName)
{
	ofxXmlSettings* xmlSettings = new ofxXmlSettings();
	xmlSettings->loadFile(fileName);
	isInterleaveMode			= xmlSettings->getValue("CAMERASETTINGS:GENERALSETTINGS:INTERLEAVE", 0);
	stitchedFrameWidth			= xmlSettings->getValue("CAMERASETTINGS:GENERALSETTINGS:STITCHEDFRAMEWIDTH", 640);
	stitchedFrameHeight			= xmlSettings->getValue("CAMERASETTINGS:GENERALSETTINGS:STITCHEDFRAMEHEIGHT", 360);
	calibrationGridWidth		= xmlSettings->getValue("CAMERASETTINGS:GENERALSETTINGS:CALIBRATIONGRIDWIDTH", 33);
	calibrationGridHeight		= xmlSettings->getValue("CAMERASETTINGS:GENERALSETTINGS:CALIBRATIONGRIDHEIGHT", 19);
	cameraGridWidth				= xmlSettings->getValue("CAMERASETTINGS:GENERALSETTINGS:CAMERAGRIDWIDTH", 1);
	cameraGridHeight			= xmlSettings->getValue("CAMERASETTINGS:GENERALSETTINGS:CAMERAGRIDHEIGHT", 1);
	cameraCount					= cameraGridWidth * cameraGridHeight;
	useGPUProcessing			= xmlSettings->getValue("CAMERASETTINGS:GENERALSETTINGS:GPUMODE", 0);
	cameraType					= (CAMERATYPE)xmlSettings->getValue("CAMERASETTINGS:GENERALSETTINGS:CAMERATYPE", 2);
	stitchedFrameFramerate		= (int)xmlSettings->getValue("CAMERASETTINGS:GENERALSETTINGS:FRAMERATE", 60);
	xmlSettings->pushTag("CAMERASETTINGS", 0);
	int numCameraTags = xmlSettings->getNumTags("CAMERA");
	if (numCameraTags!=cameraCount)
		return;
	cameraSettings = new ofxCameraBaseSettings[cameraCount];
	int settingValue = 0;
	for (int i=0;i<numCameraTags;i++)
	{ 
		xmlSettings->pushTag("CAMERA", i);
		cameraSettings[i].cameraIndex = i;
		settingValue = xmlSettings->getValue("SETTINGS:WIDTH", -1);
		if (settingValue>=0)
			cameraSettings[i].cameraWidth = settingValue;
		settingValue = xmlSettings->getValue("SETTINGS:HEIGHT", -1);
		if (settingValue>=0)
			cameraSettings[i].cameraHeight = settingValue;
		cameraSettings[i].cameraGuid = StringToGUID(xmlSettings->getValue("SETTINGS:GUID", "F774AC33-4FE2-4457-8DAF-28BE229FFC0A"));
		settingValue = xmlSettings->getValue("SETTINGS:DEPTH", -1);
		if (settingValue>=0)
			cameraSettings[i].cameraDepth = settingValue;
		settingValue = xmlSettings->getValue("SETTINGS:LEFT", -1);
		if (settingValue>=0)
			cameraSettings[i].cameraLeft = settingValue;
		settingValue = xmlSettings->getValue("SETTINGS:TOP", -1);
		if (settingValue>=0)
			cameraSettings[i].cameraTop = settingValue;
		settingValue = xmlSettings->getValue("SETTINGS:BRIGHTNESS", -1);
		if (settingValue>=0)
		{
			cameraSettings[i].isPropertyAuto.push_back(false);
			cameraSettings[i].isPropertyOn.push_back(true);
			cameraSettings[i].propertyFirstValue.push_back(settingValue);
			cameraSettings[i].propertySecondValue.push_back(0);
			cameraSettings[i].propertyType.push_back(BASE_BRIGHTNESS);
		}
		settingValue = xmlSettings->getValue("SETTINGS:EXPOSURE", -1);
		if (settingValue>=0)
		{
			cameraSettings[i].isPropertyAuto.push_back(false);
			cameraSettings[i].isPropertyOn.push_back(true);
			cameraSettings[i].propertyFirstValue.push_back(settingValue);
			cameraSettings[i].propertySecondValue.push_back(0);
			cameraSettings[i].propertyType.push_back(BASE_EXPOSURE);
		}
		settingValue = xmlSettings->getValue("SETTINGS:SHARPNESS", -1);
		if (settingValue>=0)
		{
			cameraSettings[i].isPropertyAuto.push_back(false);
			cameraSettings[i].isPropertyOn.push_back(true);
			cameraSettings[i].propertyFirstValue.push_back(settingValue);
			cameraSettings[i].propertySecondValue.push_back(0);
			cameraSettings[i].propertyType.push_back(BASE_SHARPNESS);
		}
		settingValue = xmlSettings->getValue("SETTINGS:WHITEBALANCE", -1);
		if (settingValue>=0)
		{
			int isFoundPropertyIndex = -1;
			for (int k=0;k<cameraSettings[i].propertyType.size();k++)
			{
				if (cameraSettings[i].propertyType[k] == BASE_WHITE_BALANCE)
				{
					isFoundPropertyIndex = k;
					break;
				}
			}
			if (isFoundPropertyIndex>0)
			{
				cameraSettings[i].propertySecondValue[isFoundPropertyIndex] = settingValue;
			}
			else
			{
				cameraSettings[i].isPropertyAuto.push_back(false);
				cameraSettings[i].isPropertyOn.push_back(true);
				cameraSettings[i].propertyFirstValue.push_back(settingValue);
				cameraSettings[i].propertySecondValue.push_back(0);
				cameraSettings[i].propertyType.push_back(BASE_WHITE_BALANCE);
			}
		}
		settingValue = xmlSettings->getValue("SETTINGS:BLACKBALANCE", -1);
		if (settingValue>=0)
		{
			int isFoundPropertyIndex = -1;
			for (int k=0;k<cameraSettings[i].propertyType.size();k++)
			{
				if (cameraSettings[i].propertyType[k] == BASE_WHITE_BALANCE)
				{
					isFoundPropertyIndex = k;
					break;
				}
			}
			if (isFoundPropertyIndex>0)
			{
				cameraSettings[i].propertyFirstValue[isFoundPropertyIndex] = settingValue;
			}
			else
			{
				cameraSettings[i].isPropertyAuto.push_back(false);
				cameraSettings[i].isPropertyOn.push_back(true);
				cameraSettings[i].propertyFirstValue.push_back(0);
				cameraSettings[i].propertySecondValue.push_back(settingValue);
				cameraSettings[i].propertyType.push_back(BASE_WHITE_BALANCE);
			}		
		}
		settingValue = xmlSettings->getValue("SETTINGS:HUE", -1);
		if (settingValue>=0)
		{
			cameraSettings[i].isPropertyAuto.push_back(false);
			cameraSettings[i].isPropertyOn.push_back(true);
			cameraSettings[i].propertyFirstValue.push_back(settingValue);
			cameraSettings[i].propertySecondValue.push_back(0);
			cameraSettings[i].propertyType.push_back(BASE_HUE);
		}
		settingValue = xmlSettings->getValue("SETTINGS:SATURATION", -1);
		if (settingValue>=0)
		{
			cameraSettings[i].isPropertyAuto.push_back(false);
			cameraSettings[i].isPropertyOn.push_back(true);
			cameraSettings[i].propertyFirstValue.push_back(settingValue);
			cameraSettings[i].propertySecondValue.push_back(0);
			cameraSettings[i].propertyType.push_back(BASE_SATURATION);
		}
		settingValue = xmlSettings->getValue("SETTINGS:GAMMA", -1);
		if (settingValue>=0)
		{
			cameraSettings[i].isPropertyAuto.push_back(false);
			cameraSettings[i].isPropertyOn.push_back(true);
			cameraSettings[i].propertyFirstValue.push_back(settingValue);
			cameraSettings[i].propertySecondValue.push_back(0);
			cameraSettings[i].propertyType.push_back(BASE_GAMMA);
		}
		settingValue = xmlSettings->getValue("SETTINGS:SHUTTER", -1);
		if (settingValue>=0)
		{
			cameraSettings[i].isPropertyAuto.push_back(false);
			cameraSettings[i].isPropertyOn.push_back(true);
			cameraSettings[i].propertyFirstValue.push_back(settingValue);
			cameraSettings[i].propertySecondValue.push_back(0);
			cameraSettings[i].propertyType.push_back(BASE_SHUTTER);
		}
		settingValue = xmlSettings->getValue("SETTINGS:GAIN", -1);
		if (settingValue>=0)
		{
			cameraSettings[i].isPropertyAuto.push_back(false);
			cameraSettings[i].isPropertyOn.push_back(true);
			cameraSettings[i].propertyFirstValue.push_back(settingValue);
			cameraSettings[i].propertySecondValue.push_back(0);
			cameraSettings[i].propertyType.push_back(BASE_GAIN);
		}
		settingValue = xmlSettings->getValue("SETTINGS:IRIS", -1);
		if (settingValue>=0)
		{
			cameraSettings[i].isPropertyAuto.push_back(false);
			cameraSettings[i].isPropertyOn.push_back(true);
			cameraSettings[i].propertyFirstValue.push_back(settingValue);
			cameraSettings[i].propertySecondValue.push_back(0);
			cameraSettings[i].propertyType.push_back(BASE_IRIS);
		}
		settingValue = xmlSettings->getValue("SETTINGS:FOCUS", -1);
		if (settingValue>=0)
		{
			cameraSettings[i].isPropertyAuto.push_back(false);
			cameraSettings[i].isPropertyOn.push_back(true);
			cameraSettings[i].propertyFirstValue.push_back(settingValue);
			cameraSettings[i].propertySecondValue.push_back(0);
			cameraSettings[i].propertyType.push_back(BASE_FOCUS);
		}
		settingValue = xmlSettings->getValue("SETTINGS:ZOOM", -1);
		if (settingValue>=0)
		{
			cameraSettings[i].isPropertyAuto.push_back(false);
			cameraSettings[i].isPropertyOn.push_back(true);
			cameraSettings[i].propertyFirstValue.push_back(settingValue);
			cameraSettings[i].propertySecondValue.push_back(0);
			cameraSettings[i].propertyType.push_back(BASE_ZOOM);
		}
		settingValue = xmlSettings->getValue("SETTINGS:PAN", -1);
		if (settingValue>=0)
		{
			cameraSettings[i].isPropertyAuto.push_back(false);
			cameraSettings[i].isPropertyOn.push_back(true);
			cameraSettings[i].propertyFirstValue.push_back(settingValue);
			cameraSettings[i].propertySecondValue.push_back(0);
			cameraSettings[i].propertyType.push_back(BASE_PAN);
		}
		settingValue = xmlSettings->getValue("SETTINGS:FRAMERATE", -1);
		if (settingValue>=0)
		{
			cameraSettings[i].isPropertyAuto.push_back(false);
			cameraSettings[i].isPropertyOn.push_back(true);
			cameraSettings[i].propertyFirstValue.push_back(settingValue);
			cameraSettings[i].propertySecondValue.push_back(0);
			cameraSettings[i].propertyType.push_back(BASE_FRAMERATE);
		}
		settingValue = xmlSettings->getValue("SETTINGS:VIDEOMODE", 0);
		if (settingValue>=0)
		{
			cameraSettings[i].videoPlayerOn = settingValue;
		}

		xmlSettings->pushTag("POINTARRAY", 0);
		int numPointTags = xmlSettings->getNumTags("POINT");

		int calibrationPointWidth = calibrationGridWidth + 1;
		int calibrationPointHeight = calibrationGridHeight + 1;
	
		float cameraCellWidth = ((float)cameraSettings[i].cameraWidth) / calibrationGridWidth;
		float cameraCellHeight = ((float)cameraSettings[i].cameraHeight) / calibrationGridHeight;
		
		for (int j = 0;j<(calibrationGridWidth+1)*(calibrationGridHeight+1);j++)
		{
			if (numPointTags==calibrationPointWidth*calibrationPointHeight)
				xmlSettings->pushTag("POINT", j);
			vector2df newPoint;
			newPoint.X = xmlSettings->getValue("X",(j%(calibrationGridWidth+1)) * cameraCellWidth);
			newPoint.Y = xmlSettings->getValue("Y",(j/(calibrationGridWidth+1)) * cameraCellHeight);
			cameraSettings[i].calibrationPoints.push_back(newPoint);
			if (numPointTags==calibrationPointWidth*calibrationPointHeight)
				xmlSettings->popTag();
		}
		xmlSettings->popTag();
		xmlSettings->popTag();
	}
	unsigned int* camWidth = (unsigned int*)malloc(cameraCount * sizeof(unsigned int));
	unsigned int* camHeight = (unsigned int*)malloc(cameraCount * sizeof(unsigned int));
	for (int i=0;i<cameraCount;i++)
	{
		camWidth[i] = cameraSettings[i].cameraWidth;
		camHeight[i] = cameraSettings[i].cameraHeight;
	}
	calibration->setup(stitchedFrameWidth,stitchedFrameHeight,cameraGridWidth,cameraGridHeight,calibrationGridWidth,calibrationGridHeight,camWidth,camHeight,isInterleaveMode);
	delete xmlSettings;
}


void ofxMultiplexer::initializeCameras()
{
	cameras = new ofxCameraBase*[cameraCount];
	switch (cameraType)
	{
	case PS3:
		for (int i=0;i<cameraCount;i++)
			cameras[i] = (ofxCameraBase*)(new ofxPS3());
		break;
	case CMU:
		for (int i=0;i<cameraCount;i++)
			cameras[i] = (ofxCameraBase*)(new ofxCMUCamera());		
		break;
	case FFMV:
		for (int i=0;i<cameraCount;i++)
			cameras[i] = (ofxCameraBase*)(new ofxffmv());
		break;
	case UNIBRAIN:
		//need to be added here
		break;
	}
	for (int i=0;i<cameraCount;i++)
		cameras[i]->initializeWithGUID(cameraSettings[i].cameraGuid,cameraSettings[i].cameraWidth,cameraSettings[i].cameraHeight,cameraSettings[i].cameraLeft,cameraSettings[i].cameraTop,true,cameraSettings[i].cameraDepth,stitchedFrameFramerate,cameraSettings[i].videoPlayerOn);
}

void ofxMultiplexer::changeRecordingMode()
{
	isRecordingMode = !isRecordingMode;
	for (int i=0;i<cameraCount;i++)
		cameras[i]->setVideoRecordingMode(isRecordingMode);
}

void ofxMultiplexer::applySettingsToCameras()
{
	for (int i=0;i<cameraCount;i++)
		for (int j=0;j<cameraSettings[i].propertyType.size();j++)
			cameras[i]->setCameraFeature(cameraSettings[i].propertyType[j],cameraSettings[i].propertyFirstValue[j],cameraSettings[i].propertySecondValue[j],cameraSettings[i].isPropertyAuto[j],cameraSettings[i].isPropertyOn[j]);
}

void ofxMultiplexer::receiveSettingsFromCameras()
{
	for (int i=0;i<cameraCount;i++)
		for (int j=0;j<cameraSettings[i].propertyType.size();j++)
		{
			int firstValue,secondValue;
			bool isAuto,isEnabled;
			cameras[i]->getCameraFeature(cameraSettings[i].propertyType[j],&firstValue,&secondValue,&isAuto,&isEnabled);
			cameraSettings[i].propertyFirstValue[j] = firstValue;
			cameraSettings[i].propertySecondValue[j] = secondValue;
			cameraSettings[i].isPropertyOn[j] = isEnabled;
			cameraSettings[i].isPropertyAuto[j] = isAuto;
		}
}

void ofxMultiplexer::saveSettingsToXML(char* fileName)
{

	ofxXmlSettings* xmlSettings = new ofxXmlSettings();
	xmlSettings->setValue("CAMERASETTINGS:GENERALSETTINGS:INTERLEAVE", isInterleaveMode);
	xmlSettings->setValue("CAMERASETTINGS:GENERALSETTINGS:STITCHEDFRAMEWIDTH", stitchedFrameWidth);
	xmlSettings->setValue("CAMERASETTINGS:GENERALSETTINGS:STITCHEDFRAMEHEIGHT", stitchedFrameHeight);
	xmlSettings->setValue("CAMERASETTINGS:GENERALSETTINGS:CALIBRATIONGRIDWIDTH", calibrationGridWidth);
	xmlSettings->setValue("CAMERASETTINGS:GENERALSETTINGS:CALIBRATIONGRIDHEIGHT", calibrationGridHeight);
	xmlSettings->setValue("CAMERASETTINGS:GENERALSETTINGS:GPUMODE", useGPUProcessing);
	xmlSettings->setValue("CAMERASETTINGS:GENERALSETTINGS:FRAMERATE", stitchedFrameFramerate);
	xmlSettings->setValue("CAMERASETTINGS:GENERALSETTINGS:CAMERAGRIDWIDTH", cameraGridWidth);
	xmlSettings->setValue("CAMERASETTINGS:GENERALSETTINGS:CAMERAGRIDHEIGHT", cameraGridHeight);
	xmlSettings->setValue("CAMERASETTINGS:GENERALSETTINGS:CAMERATYPE", cameraType);

	xmlSettings->pushTag("CAMERASETTINGS", 0);

	for (int i=0;i<cameraCount;i++)
	{ 
		xmlSettings->setValue("CAMERA","",i);
		xmlSettings->pushTag("CAMERA", i);
		
		xmlSettings->setValue("SETTINGS:GUID", GUIDToString(cameraSettings[i].cameraGuid));
		xmlSettings->setValue("SETTINGS:WIDTH", cameraSettings[i].cameraWidth);
		xmlSettings->setValue("SETTINGS:HEIGHT", cameraSettings[i].cameraHeight);
		xmlSettings->setValue("SETTINGS:LEFT", cameraSettings[i].cameraLeft);
		xmlSettings->setValue("SETTINGS:TOP", cameraSettings[i].cameraTop);
		xmlSettings->setValue("SETTINGS:DEPTH", cameraSettings[i].cameraDepth);
		xmlSettings->setValue("SETTINGS:VIDEOMODE", (int)cameraSettings[i].videoPlayerOn);
		for (int j=0;j<cameraSettings[i].propertyType.size();j++)
		{
			if (cameraSettings[i].propertyType[j] == BASE_WHITE_BALANCE)
			{
				 xmlSettings->setValue("SETTINGS:WHITEBALANCE", cameraSettings[i].propertyFirstValue[j]);
				 xmlSettings->setValue("SETTINGS:BLACKBALANCE", cameraSettings[i].propertySecondValue[j]);
			}
			else
			{
				switch (cameraSettings[i].propertyType[j])
				{
				case BASE_BRIGHTNESS:
					xmlSettings->setValue("SETTINGS:BRIGHTNESS", cameraSettings[i].propertyFirstValue[j]);
					break;
				case BASE_EXPOSURE:
					xmlSettings->setValue("SETTINGS:EXPOSURE", cameraSettings[i].propertyFirstValue[j]);
					break;
				case BASE_SHARPNESS:
					xmlSettings->setValue("SETTINGS:SHARPNESS", cameraSettings[i].propertyFirstValue[j]);
					break;
				case BASE_HUE:
					xmlSettings->setValue("SETTINGS:HUE", cameraSettings[i].propertyFirstValue[j]);
					break;
				case BASE_SATURATION:
					xmlSettings->setValue("SETTINGS:SATURATION", cameraSettings[i].propertyFirstValue[j]);
					break;
				case BASE_GAMMA:
					xmlSettings->setValue("SETTINGS:GAMMA", cameraSettings[i].propertyFirstValue[j]);
					break;
				case BASE_SHUTTER:
					xmlSettings->setValue("SETTINGS:SHUTTER", cameraSettings[i].propertyFirstValue[j]);
					break;
				case BASE_GAIN:
					xmlSettings->setValue("SETTINGS:GAIN", cameraSettings[i].propertyFirstValue[j]);
					break;
				case BASE_IRIS:
					xmlSettings->setValue("SETTINGS:IRIS", cameraSettings[i].propertyFirstValue[j]);
					break;
				case BASE_FOCUS:
					xmlSettings->setValue("SETTINGS:FOCUS", cameraSettings[i].propertyFirstValue[j]);
					break;
				case BASE_ZOOM:
					xmlSettings->setValue("SETTINGS:ZOOM", cameraSettings[i].propertyFirstValue[j]);
					break;
				case BASE_PAN:
					xmlSettings->setValue("SETTINGS:PAN", cameraSettings[i].propertyFirstValue[j]);
					break;
				case BASE_FRAMERATE:
					xmlSettings->setValue("SETTINGS:FRAMERATE", cameraSettings[i].propertyFirstValue[j]);
					break;
				}
			}
		}

		int calibrationPointWidth = calibrationGridWidth + 1;
		int calibrationPointHeight = calibrationGridHeight + 1;
	
		int numPointTags = calibrationPointWidth * calibrationPointHeight;

		float widthStep = (float)cameraSettings[i].cameraWidth / (calibrationPointWidth-1);
		float heightStep = (float)cameraSettings[i].cameraHeight / (calibrationPointHeight-1);
		
		xmlSettings->setValue("POINTARRAY","",0);
		xmlSettings->pushTag("POINTARRAY", 0);

		for (int j = 0;j<numPointTags;j++)
		{
			xmlSettings->setValue("POINT","",j);
 			xmlSettings->pushTag("POINT", j);
			xmlSettings->setValue("X",cameraSettings[i].calibrationPoints[j].X);
			xmlSettings->setValue("Y",cameraSettings[i].calibrationPoints[j].Y);
			xmlSettings->popTag();
		}
		xmlSettings->popTag();

		xmlSettings->popTag();
	}
	xmlSettings->saveFile(fileName);
	delete xmlSettings;
}

void ofxMultiplexer::computeDistortion()
{
	calibration->calibrate.computeCameraMaps(cameraMap,stitchedFrameWidth,stitchedFrameHeight,cameraGridWidth,cameraGridHeight,calibrationGridWidth,calibrationGridHeight,isInterleaveMode);
	for (int i=0;i<cameraCount;i++)
	{
		vector2df* calibrations = (vector2df*)malloc((calibrationGridWidth+1)*(calibrationGridHeight+1)*sizeof(vector2df));
		for (int j=0;j<(calibrationGridWidth+1)*(calibrationGridHeight+1);j++)
		{
			calibrations[j] = cameraSettings[i].calibrationPoints[j];
		}
		calibration->calibrate.computeOffsetMap(offsetTextureCPU[i],calibrations,i,stitchedFrameWidth,stitchedFrameHeight,cameraSettings[i].cameraWidth,cameraSettings[i].cameraHeight,cameraGridWidth,cameraGridHeight,calibrationGridWidth,calibrationGridHeight,isInterleaveMode);
		calibration->calibrate.computeWeightMap(weightTextureCPU[i],calibrations,i,stitchedFrameWidth,stitchedFrameHeight,cameraSettings[i].cameraWidth,cameraSettings[i].cameraHeight,cameraGridWidth,cameraGridHeight,calibrationGridWidth,calibrationGridHeight,isInterleaveMode);
		free(calibrations);
	}
	isInitialized = true;
}

void ofxMultiplexer::initCPUMode()
{
	offsetTextureCPU = (unsigned int**)malloc(cameraCount * sizeof(unsigned int*));
	weightTextureCPU = (float**)malloc(cameraCount * sizeof(float*));
	cameraFrames = (unsigned char**)malloc(cameraCount * sizeof(unsigned char*));
	cpuStitchedFrame = (unsigned char*)malloc(stitchedFrameWidth * stitchedFrameHeight * sizeof(unsigned char));
	cameraMap = (unsigned char**)malloc(4 * sizeof(unsigned char*));
	for (int i=0;i<cameraCount;i++)
	{
		offsetTextureCPU[i] = (unsigned int*)malloc(stitchedFrameWidth * stitchedFrameHeight * sizeof(unsigned int));
		weightTextureCPU[i] = (float*)malloc(stitchedFrameWidth * stitchedFrameHeight * sizeof(float));
		cameraFrames[i] = (unsigned char*)malloc(cameraSettings[i].cameraWidth * cameraSettings[i].cameraHeight * sizeof(float));
	}
	for (int i=0;i<4;i++)
	{
		cameraMap[i] = (unsigned char*)malloc(stitchedFrameWidth*stitchedFrameHeight*sizeof(unsigned char));
		memset((void*)cameraMap[i],NULL_CAMERA,stitchedFrameWidth*stitchedFrameHeight*sizeof(unsigned char));

	}
}

void ofxMultiplexer::initGPUMode()
{
}