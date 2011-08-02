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
#include <string>
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
	//! Sort by alphabet
	BASE_AUTO_EXPOSURE,			// [false, true]	PS3
	BASE_AUTO_GAIN,				// [false, true]	PS3
	BASE_AUTO_WHITE_BALANCE,	// [false, true]	PS3

	BASE_BRIGHTNESS,
	BASE_CONTRAST,
	BASE_EXPOSURE,				// [0, 511]			PS3
	BASE_FOCUS,
	BASE_FRAMERATE,
	BASE_GAIN,					// [0, 79]			PS3
	BASE_GAMMA,
	BASE_HFLIP,					// [false, true]	PS3
	BASE_HUE,
	BASE_IRIS,
	BASE_MOTOR_POSITION,
	BASE_MOTOR_LED,
	BASE_PAN,
	BASE_ROLL,
	BASE_SATURATION,
	BASE_SHARPNESS,
	BASE_SHUTTER,
	BASE_TILT,
	BASE_VFLIP,					// [false, true]	PS3
	BASE_WHITE_BALANCE,			
	BASE_WHITE_BALANCE_BLUE,	// [0, 255]			PS3
	BASE_WHITE_BALANCE_GREEN,	// [0, 255]			PS3
	BASE_WHITE_BALANCE_RED,		// [0, 255]			PS3
	BASE_ZOOM,

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
	void setFeature( const CAMERA_BASE_FEATURE &feature, const int &firstValue, const int &secondValue, const bool &isAuto, const bool &isEnable );
	int getFirstValue( const CAMERA_BASE_FEATURE &feature );
	int getSecondValue( const CAMERA_BASE_FEATURE &feature );
	bool isFeatureOn( const CAMERA_BASE_FEATURE &feature );
	bool isFeatureAuto( const CAMERA_BASE_FEATURE &feature );
} ofxCameraBaseSettings;

std::string CameraTypeToStr( const CAMERATYPE &type );
CAMERATYPE StrToCameraType( const std::string &str );

#endif // OFX_CAMERABASE_SETTINGS_H