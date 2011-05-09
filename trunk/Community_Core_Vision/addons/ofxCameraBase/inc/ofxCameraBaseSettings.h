/*
*  ofxCameraBaseSettings.h
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#ifndef OFX_CAMERABASE_SETTINGS_H
#define OFX_CAMERABASE_SETTINGS_H

#include <vector>
#include "vector2d.h"
#include <windows.h>

typedef enum 
{
	BASE_BRIGHTNESS,
	BASE_EXPOSURE,
	BASE_SHARPNESS,
	BASE_WHITE_BALANCE,
	BASE_HUE,
	BASE_SATURATION,
	BASE_GAMMA,
	BASE_SHUTTER,
	BASE_GAIN,
	BASE_IRIS,
	BASE_FOCUS,
	BASE_ZOOM,
	BASE_PAN,
	BASE_FRAMERATE
} CAMERA_BASE_FEATURE;

typedef struct ofxCameraBaseSettings
{
	GUID cameraGuid;
	unsigned char cameraDepth;
	int cameraWidth,cameraHeight,cameraLeft,cameraTop,cameraIndex;
	bool videoPlayerOn,videoRecorderOn;
	std::vector<bool> isPropertyOn;
	std::vector<bool> isPropertyAuto;
	std::vector<int> propertyFirstValue;
	std::vector<int> propertySecondValue;
	std::vector<CAMERA_BASE_FEATURE> propertyType;
	std::vector<vector2df > calibrationPoints;
} ofxCameraBaseSettings;


#endif // OFX_CAMERABASE_SETTINGS_H