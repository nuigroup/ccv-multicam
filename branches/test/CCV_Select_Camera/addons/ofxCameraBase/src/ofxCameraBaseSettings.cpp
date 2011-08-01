/*
*	ofxCameraBaseSettings.cpp
*
*
*	Created by Yishi Guo on 07/30/2011.
*	Copyright 2011 NUI Group. All right reserved.
*
*/

#include "ofxCameraBaseSettings.h"

std::string CameraTypeToStr( const CAMERATYPE &type ) {
	std::string str = "NULL";
	switch ( type ) {
		case PS3:
			str = "PS3";
			break;
		case CMU:
			str = "CMU";
			break;
		case FFMV:
			str = "FFMV";
			break;
		case DIRECTSHOW:
			str = "DIRECTSHOW";
			break;
		case KINECT:
			str = "KINECT";
			break;
		default:
			break;
	}

	return str;
}

CAMERATYPE StrToCameraType( const std::string &str ) {
	CAMERATYPE type = PS3;
	if ( str == "PS3" ) {
		type = PS3;
	} else if ( str == "FFMV" ) {
		type = FFMV;
	} else if ( str == "CMU" ) {
		type = CMU;
	} else if ( str == "DIRECTSHOW" ) {
		type = DIRECTSHOW;
	} else if ( str == "KINECT" ) {
		type = KINECT;
	}

	return type;
}

ofxCameraBaseSettings& ofxCameraBaseSettings::operator=( const ofxCameraBaseSettings &temp ) {
	this->cameraGuid	= temp.cameraGuid;
	this->cameraType	= temp.cameraType;
	this->pixelMode		= temp.pixelMode;
	this->cameraDepth	= temp.cameraDepth;
	this->cameraX		= temp.cameraX;
	this->cameraY		= temp.cameraY;
	this->cameraWidth	= temp.cameraWidth;
	this->cameraHeight	= temp.cameraHeight;
	this->cameraLeft	= temp.cameraLeft;
	this->cameraTop		= temp.cameraTop;
	this->cameraIndex	= temp.cameraIndex;
	this->cameraFramerate	= temp.cameraFramerate;
	this->videoPlayerOn	= temp.videoPlayerOn;
	this->videoRecorderOn	= temp.videoRecorderOn;
	this->isPropertyOn	= temp.isPropertyOn;
	this->isPropertyAuto	= temp.isPropertyAuto;
	this->propertyFirstValue = temp.propertyFirstValue;
	this->propertySecondValue = temp.propertySecondValue;
	this->propertyType	= temp.propertyType;
	this->calibrationPoints	= temp.calibrationPoints;
	//int i;
	//for ( i = 0; i < isPropertyAuto.size(); ++i ) {
	//	this->isPropertyAuto[i] = temp.isPropertyAuto[i];
	//}
	//for ( i = 0; i < isPropertyOn.size(); ++i ) {
	//	this->isPropertyOn[i] = temp.isPropertyOn[i];
	//}
	//for ( i = 0; i < propertyFirstValue.size(); ++i ) {
	//	this->propertyFirstValue[i] = temp.propertyFirstValue[i];
	//}
	//for ( i = 0; i < propertyFirstValue.size(); ++i ) {
	//	this->propertySecondValue[i] = temp.propertySecondValue[i];
	//}
	//for ( i = 0; i < propertyType.size(); ++i ) {
	//	this->propertyType[i] = temp.propertyType[i];
	//}
	//for ( i = 0; i < calibrationPoints.size(); ++i ) {
	//	this->calibrationPoints[i] = temp.calibrationPoints[i];
	//}

	return *this;
}