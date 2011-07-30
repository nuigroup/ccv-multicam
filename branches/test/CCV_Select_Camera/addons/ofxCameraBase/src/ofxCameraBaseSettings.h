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
#include <windows.h>
#include <string.h>
#include "Calibration/vector2d.h"

typedef enum 
{
	PS3,
	CMU,
	FFMV,
	DIRECTSHOW,
	KINECT
} CAMERATYPE;

//for FFMV cameras or other cameras which supports different types of capturing
//MODE0 - 1 texel -> 1 pixel
//MODE1 - 4 texel -> 1 pixel
//MODE2 - Bayer mask -> 1 pixel
typedef enum
{
	MODE0,
	MODE1,
	MODE2
} PIXEL_MODE;

typedef enum 
{
	BASE_BRIGHTNESS,
	BASE_EXPOSURE,
	BASE_SHARPNESS,
	BASE_WHITE_BALANCE,
	BASE_HUE,
	BASE_CONTRAST,
	BASE_SATURATION,
	BASE_GAMMA,
	BASE_SHUTTER,
	BASE_GAIN,
	BASE_IRIS,
	BASE_FOCUS,
	BASE_ZOOM,
	BASE_PAN,
	BASE_TILT,
	BASE_ROLL,
	BASE_FRAMERATE,
	BASE_MOTOR_POSITION,
	BASE_MOTOR_LED,
	DEPTH_NEAR_BOUND,
	DEPTH_FAR_BOUND
} CAMERA_BASE_FEATURE;

typedef struct ofxCameraBaseSettings
{
	GUID cameraGuid;
	CAMERATYPE cameraType;
	PIXEL_MODE pixelMode;
	unsigned char cameraDepth;
	int cameraX, cameraY;
	int cameraWidth,cameraHeight,cameraLeft,cameraTop,cameraIndex,cameraFramerate;
	bool videoPlayerOn,videoRecorderOn;
	std::vector<bool> isPropertyOn;
	std::vector<bool> isPropertyAuto;
	std::vector<int> propertyFirstValue;
	std::vector<int> propertySecondValue;
	std::vector<CAMERA_BASE_FEATURE> propertyType;
	std::vector<vector2df> calibrationPoints;

	ofxCameraBaseSettings& operator = ( const ofxCameraBaseSettings& temp );
} ofxCameraBaseSettings;

std::string CameraTypeToStr( const CAMERATYPE &type );

#endif // OFX_CAMERABASE_SETTINGS_H