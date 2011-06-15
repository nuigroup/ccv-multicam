//! CamsUtils.cpp
/*!
*  
*
*  Created by Yishi Guo on 06/13/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

// ----------------------------------------------

#include "CamsUtils.h"

// ----------------------------------------------

CamsUtils::CamsUtils() {
	displayCams = NULL;
	rawCams = NULL;
	camCount = 0;
}

// ----------------------------------------------

CamsUtils::~CamsUtils() {
	if ( rawCams != NULL ) {
		stop();
	}
}

// ----------------------------------------------

void CamsUtils::setup( CLEyeCameraColorMode colorMode, CLEyeCameraResolution camRes, float frameRate ) {
	this->camCount = getDevicesCount();
	if ( camCount <= 0 ) {
		return;
	}

	this->colorMode = colorMode;
	this->camRes = camRes;
	this->frameRate = frameRate;

	rawCams = new PS3*[camCount];

	start();
}

// ----------------------------------------------

void CamsUtils::update() {
	for ( int i = 0; i < camCount; ++i ) {
		printf( "CamsUtils::update()\t i = %d\n", i );
		bool newF = rawCams[i]->IsFrameNew();
		printf( "CamsUtils::update()\t new = %d\n", newF );
	}
}

// ----------------------------------------------
void CamsUtils::start() {
	//! init each camera.
	for ( int i = 0; i < camCount; ++i ) {
		GUID guid = getGUID( i );

		rawCams[i] = new PS3();
		rawCams[i]->SetCamera( guid, colorMode, camRes, frameRate );
		if ( rawCams[i]->StartCamera() ) {
			//printf( "rawCams[%d]->StartCamera() return true\n", i );
		}
	}
}
// ----------------------------------------------

void CamsUtils::stop() {
	if ( rawCams != NULL ) {
		for ( int i = 0; i < camCount; ++i ) {
			rawCams[i]->StopCamera();
			delete rawCams[i];
			rawCams[i] = NULL;
		}
		delete rawCams;
		rawCams = NULL;
	}
}

// ----------------------------------------------

int CamsUtils::getCount() {
	return camCount;
}

// ----------------------------------------------

int CamsUtils::getXGrid() {
	return xGrid;
}

// ----------------------------------------------

int CamsUtils::getYGrid() {
	return yGrid;
}

// ----------------------------------------------

PS3* CamsUtils::getCam( int index ) {
	if ( displayCams != NULL && xGrid * yGrid > index) {
		// TODO
		return displayCams[index];
	}

	return NULL;	// error
}

// ----------------------------------------------

PS3* CamsUtils::getCam( int x, int y ) {
	int index = x + xGrid * y;

	return getCam( index );
}

// ----------------------------------------------

PS3* CamsUtils::getRawCam( int index ) {
	if ( rawCams != NULL && camCount > index ) {
		//printf( "CamsUtils::getRawCam()\t return\n" );
		return rawCams[index];
	}

	return NULL;
}

// ----------------------------------------------

PS3** CamsUtils::getCams() {
	return displayCams;
}

// ----------------------------------------------

PS3** CamsUtils::getRawCams() {
	return rawCams;
}

// ----------------------------------------------

void CamsUtils::setXY( int x, int y ) {
	this->xGrid = x;
	this->yGrid = y;
	createDisplayCams( xGrid, yGrid );
}

// ----------------------------------------------

void CamsUtils::setCam( int index, PS3* cam ) {
	displayCams[index] = cam;
}

// ----------------------------------------------

void CamsUtils::setCam( int x, int y, PS3* cam ) {
	int index = x + xGrid * y;
	setCam( index, cam );
}

// ----------------------------------------------

int CamsUtils::getDevicesCount() {
	return CLEyeGetCameraCount();
}

// ----------------------------------------------

GUID CamsUtils::getGUID( int camId ) {
	return CLEyeGetCameraUUID( camId );
}

// ----------------------------------------------

void CamsUtils::createDisplayCams( int x, int y ) {
	int count = x * y;

	if ( displayCams != NULL ) {
		delete displayCams;
	}
	displayCams = new PS3*[count];
	for ( int i = 0; i < count; ++i ) {
		displayCams[i] = NULL;
	}
}

// ----------------------------------------------