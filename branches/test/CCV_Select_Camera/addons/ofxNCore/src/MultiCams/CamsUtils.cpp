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

	camsUsed = NULL;
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
	//! init the bool array.
	camsSelected = new bool[camCount];
	for ( int i = 0; i < camCount; ++i ) {
		camsSelected[i] = false;
	}

	start();
}

// ----------------------------------------------

void CamsUtils::update() {
	for ( int i = 0; i < camCount; ++i ) {
		//printf( "CamsUtils::update()\t i = %d\n", i );
		bool newF = rawCams[i]->IsFrameNew();
		//printf( "CamsUtils::update()\t new = %d\n", newF );
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
	if ( rawCams != NULL && camCount > index && index >= 0 ) {	//! index can not less than 0
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

bool CamsUtils::isSelected( int rawId ) {
	return camsSelected[rawId];
}

// ----------------------------------------------

void CamsUtils::setSelected( int rawId ) {
	camsSelected[rawId] = true;
}

// ----------------------------------------------

bool CamsUtils::isUsed( int displayId ) {
	return camsUsed[displayId];
}

// ----------------------------------------------

void CamsUtils::setXY( int x, int y ) {
	this->xGrid = x;
	this->yGrid = y;
	createDisplayCams( xGrid, yGrid );
	resetCamsSelected();
}

// ----------------------------------------------

void CamsUtils::setCam( int index, PS3* cam ) {
	if ( cam == NULL ) {
		return;
	}

	displayCams[index] = cam;
	camsUsed[index] = true;
}

// ----------------------------------------------

void CamsUtils::setCam( int x, int y, PS3* cam ) {
	if ( cam == NULL ) {
		return;
	}

	int index = x + xGrid * y;
	setCam( index, cam );
}

// ----------------------------------------------

void CamsUtils::resetAll() {
	//! Set X/Y as reset all
	setXY( xGrid, yGrid );
	
	//! Clean up selected cams
	resetCamsSelected();
}

// ----------------------------------------------

void CamsUtils::saveXML( string filename ) {
	// TODO
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
	printf( "\nCamsUtils::createDisplayCams()\n" );
	int count = x * y;

	if ( displayCams != NULL ) {
		delete displayCams;
	}
	displayCams = new PS3*[count];
	for ( int i = 0; i < count; ++i ) {
		displayCams[i] = NULL;
	}

	//! for camsUsed array
	if ( camsUsed != NULL ) {
		delete camsUsed;
	}
	camsUsed = new bool[count];
	for ( int i = 0; i < count; ++i ) {
		camsUsed[i] = false;
		printf( "\ncamsUsed[%d]=%d\n", i, camsUsed[i] );
	}

	printf( "\nCamsUtils::createDisplayCams()\tx=%d, y=%d\n", x, y );
}

// ----------------------------------------------

void CamsUtils::resetCamsSelected() {
	for ( int i = 0; i < camCount; ++i ) {
		camsSelected[i] = false;
	}
}

// ----------------------------------------------