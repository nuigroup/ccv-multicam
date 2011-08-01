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
	xmlCams = NULL;

	rawCamsSettings = NULL;
	xmlCamsSettings = NULL;

	camCount = 0;

	camsUsed = NULL;

	xGrid = 1;
	yGrid = 1;
}

// ----------------------------------------------

CamsUtils::~CamsUtils() {
	if ( rawCams != NULL ) {
		stop();
	}
}

// ----------------------------------------------

void CamsUtils::setup( CLEyeCameraColorMode colorMode, CLEyeCameraResolution camRes, float frameRate ) {
	//this->camCount = getDevicesCount();
	//if ( camCount <= 0 ) {
	//	return;
	//}

	//this->colorMode = colorMode;
	//this->camRes = camRes;
	//this->frameRate = frameRate;

	//rawCams = new PS3*[camCount];
	////! init the bool array.
	//camsSelected = new bool[camCount];
	//for ( int i = 0; i < camCount; ++i ) {
	//	camsSelected[i] = false;
	//}

	////! Load XML settings
	//loadXML();
	////! 
	//if ( xGrid * yGrid > camCount ) {
	//	xGrid = 1;
	//	yGrid = 1;
	//	xmlCams = NULL;
	//}
	//setXY( xGrid, yGrid );

	//start();
}

void CamsUtils::setup() {
	int i, j, index;
	camCount = getDevicesCount();
	printf( "\nCamsUtils::setup()\n\tcamCount = %d\n", camCount );
	if ( camCount <= 0 ) {
		return;
	}

	rawCams = new ofxCameraBase*[camCount];
	rawCamsSettings = new ofxCameraBaseSettings*[camCount];
	camsSelected = new bool[camCount];
	for ( i = 0; i < camCount; ++i ) {
		camsSelected[i] = false;
	}

	//! Load Settings from XML
	loadXML();

	//! Wrong settings
	if ( xGrid * yGrid > camCount ) {
		xGrid = 1;
		xGrid = 1;
		xmlCams = NULL;
	}

	setXY( xGrid, yGrid );

	index = 0;

	//! PS3 cameras settings apply
	for ( i = 0; i < getDevicesCount( false, PS3 ) && index < getDevicesCount(); ++i, ++index ) {
		GUID guid = getGUID( PS3, i );

		printf( "\n\ti=%d, index=%d, guid=%s\n", i, index, GUIDToString(guid).c_str() );

		rawCams[index] = (ofxCameraBase*)(new ofxPS3());
		rawCamsSettings[index] = new ofxCameraBaseSettings();
		rawCamsSettings[index]->cameraType = PS3;
		rawCamsSettings[index]->cameraGuid = guid;
		
		setupCameraSettings( rawCamsSettings[index] );

		printf( "\nCamsUtils::setup()\nrawCams[%d].GUID = %s\n", index, GUIDToString( rawCamsSettings[index]->cameraGuid ).c_str() );

		for ( j = 0; j < numCamTags; ++j ) {
			printf( "CamsUtils::start\tGUID1 = %s\n\t\t\tGUID2 = %s\n\n",
				GUIDToString(guid).c_str(),
				GUIDToString(xmlCamsSettings[j]->cameraGuid).c_str() );
			if ( EqualGUID( guid, xmlCamsSettings[j]->cameraGuid ) ) {
				rawCamsSettings[index] = xmlCamsSettings[j];
				setCam( xmlCamsSettings[j]->cameraX, xmlCamsSettings[j]->cameraY, rawCams[index] );
				setSelected( index );
			}
		}

		rawCams[index]->initializeWithGUID(
			rawCamsSettings[index]->cameraGuid,
			rawCamsSettings[index]->cameraWidth,
			rawCamsSettings[index]->cameraHeight,
			rawCamsSettings[index]->cameraLeft,
			rawCamsSettings[index]->cameraTop,
			rawCamsSettings[index]->cameraDepth,
			rawCamsSettings[index]->cameraFramerate );

	}

	//! CMU
	for ( i = 0; i < getDevicesCount( false, CMU ) && index < getDevicesCount(); ++i, ++index ) {
		// TODO
	}

	//! FFMV
	for ( i = 0; i < getDevicesCount( false, FFMV ) && index < getDevicesCount(); ++i, ++index ) {
		// TODO
	}

	//! DIRECTSHOW

	//! KINECT

	applyCameraSettings();

	startCameras();
}

// ----------------------------------------------

void CamsUtils::update() {
	// TODO
}

// ----------------------------------------------
void CamsUtils::start() {
	//! init each camera.
	//for ( int i = 0; i < camCount; ++i ) {
	//	GUID guid = getGUID( i );

	//	rawCams[i] = new PS3();

	//	if ( xmlCams != NULL ) {
	//		for ( int j = 0; j < xGrid * yGrid; ++j ) {
	//			if ( xmlCams[j] != NULL ) {
	//				printf( "CamsUtils::start\tGUID1 = %s\n\t\t\tGUID2 = %s\n\n",
	//					PS3::GUID2String(guid).c_str(),
	//					PS3::GUID2String(xmlCams[j]->GetGUID()).c_str() );

	//				if ( PS3::EqualGUID( guid, xmlCams[j]->GetGUID() ) ) {
	//					printf ( "\nGUID1 = GUID2\n" );
	//					setCam( j, rawCams[i] );
	//					setSelected( i );
	//					guid = xmlCams[j]->GetGUID();

	//					//! Setup the camera
	//					rawCams[i]->SetHFlip( xmlCams[j]->GetHFlip() );
	//					rawCams[i]->SetVFlip( xmlCams[j]->GetVFlip() );
	//					rawCams[i]->SetAutoGain( xmlCams[j]->GetAutoGain() );
	//					rawCams[i]->SetGainValue( xmlCams[j]->GetGainValue() );
	//					rawCams[i]->SetAutoExposure( xmlCams[j]->GetAutoExposure() );
	//					rawCams[i]->SetExposure( xmlCams[j]->GetExposure() );
	//					rawCams[i]->SetAutoWhiteBalance( xmlCams[j]->GetAutoWhiteBalance() );
	//					rawCams[i]->SetWhiteBalanceRed( xmlCams[j]->GetWhiteBalanceRed() );
	//					rawCams[i]->SetWhiteBalanceGreen( xmlCams[j]->GetWhiteBalanceGreen() );
	//					rawCams[i]->SetWhiteBalanceBlue( xmlCams[j]->GetWhiteBalanceBlue() );

	//					printf( "Now GUID = %s\n", PS3::GUID2String( guid ).c_str() );
	//					printf( "H = %d\tV = %d\n", rawCams[i]->GetHFlip(), rawCams[i]->GetVFlip() );

	//					break;
	//				}
	//			}
	//		}
	//	}


	//	rawCams[i]->SetCamera( guid, colorMode, camRes, frameRate );
	//	if ( rawCams[i]->StartCamera() ) {
	//		//printf( "rawCams[%d]->StartCamera() return true\n", i );
	//	}
	//}
}
// ----------------------------------------------

void CamsUtils::stop() {
	if ( rawCams != NULL ) {
		for ( int i = 0; i < camCount; ++i ) {
			rawCams[i]->deinitializeCamera();
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

//// ----------------------------------------------
//
//int CamsUtils::getRawId( PS3* cam ) {
//	if ( cam == NULL ) {
//		return -1;
//	}
//
//	for ( int i = 0; i < camCount; i++ ) {
//		if ( PS3::EqualGUID( cam->GetGUID(), rawCams[i]->GetGUID() ) ) {
//			return i;
//		}
//	}
//
//	return -1;
//}
//

int CamsUtils::getRawId( ofxCameraBase* cam ) {
	if ( cam == NULL ) {
		return -1;
	}

	for ( int i = 0; i < camCount; i++ ) {
		if ( EqualGUID( cam->getCameraGUID(), rawCams[i]->getCameraGUID() ) ) {
			return i;
		}
	}

	return -1;
}
//// ----------------------------------------------
//
//PS3* CamsUtils::getCam( int index ) {
//	if ( displayCams != NULL 
//		&& xGrid * yGrid > index
//		&& index >= 0
//		) {
//		// TODO
//		return displayCams[index];
//	}
//
//	return NULL;	// error
//}
//

ofxCameraBase* CamsUtils::getCam( int index ) {
	if ( displayCams != NULL 
		&& xGrid * yGrid > index
		&& index >= 0
		) {
		// TODO
		return displayCams[index];
	}

	return NULL;	// error
}

//// ----------------------------------------------
//
//PS3* CamsUtils::getCam( int x, int y ) {
//	int index = x + xGrid * y;
//
//	return getCam( index );
//}
//

ofxCameraBase* CamsUtils::getCam( int x, int y ) {
	int index = x + xGrid * y;

	return getCam( index );
}

//// ----------------------------------------------
//
//PS3* CamsUtils::getRawCam( int index ) {
//	if ( rawCams != NULL && camCount > index && index >= 0 ) {	//! index can not less than 0
//		//printf( "CamsUtils::getRawCam()\t return\n" );
//		return rawCams[index];
//	}
//
//	return NULL;
//}
//

ofxCameraBase* CamsUtils::getRawCam( int index ) {
	if ( rawCams != NULL
		&& camCount > index
		&& index >= 0
		) {	//! index can not less than 0
		return rawCams[index];
	}

	return NULL;
}

//// ----------------------------------------------
//
//PS3** CamsUtils::getCams() {
//	return displayCams;
//}
//
//// ----------------------------------------------
//
//PS3** CamsUtils::getRawCams() {
//	return rawCams;
//}

ofxCameraBase** CamsUtils::getCams() {
	return displayCams;
}

ofxCameraBase** CamsUtils::getRawCams() {
	return rawCams;
}

// ----------------------------------------------

bool CamsUtils::isSelected( int rawId ) {
	return camsSelected[rawId];
}

// ----------------------------------------------

void CamsUtils::setSelected( int rawId, bool reset ) {
	camsSelected[rawId] = !reset;
}

// ----------------------------------------------

bool CamsUtils::isUsed( int displayId ) {
	if ( camCount < 1 ) {
		return false;
	}
	if ( displayId < 0 || displayId > xGrid * yGrid ) {
		return false;
	}
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

//void CamsUtils::setCam( int index, PS3* cam ) {
//	//! if the cam == NULL, that will be RESET function
//	//if ( cam == NULL ) {
//	//	return;
//	//}
//
//	displayCams[index] = cam;
//	camsUsed[index] = cam == NULL ? false : true;
//}
//
//// ----------------------------------------------
//
//void CamsUtils::setCam( int x, int y, PS3* cam ) {
//	//if ( cam == NULL ) {
//	//	return;
//	//}
//
//	int index = x + xGrid * y;
//	setCam( index, cam );
//}

void CamsUtils::setCam( int index, ofxCameraBase* cam ) {
	//! if the cam == NULL, that will be RESET function

	displayCams[index] = cam;
	camsUsed[index] = cam == NULL ? false : true;
}

// ----------------------------------------------

void CamsUtils::setCam( int x, int y, ofxCameraBase* cam ) {
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

void CamsUtils::resetCam( int rawIndex ) {
	if ( rawIndex < camCount ) {
		//rawCams[rawIndex]->ResetCamParam();
		// TODO
	}
}

// ----------------------------------------------

void CamsUtils::resetCams() {
	for( int i = 0; i < camCount; ++i ) {
		//rawCams[i]->ResetCamParam();
		// TODO
	}
}

// ----------------------------------------------

void CamsUtils::saveXML( string filename ) {
	//! Clear previous data
	XML.clear();

	XML.setValue( "MULTICAMS:GRIDX", xGrid );
	XML.setValue( "MULTICAMS:GRIDY", yGrid );

	//! cameras information
	for ( int y = 0; y < yGrid; ++y ) {
		for ( int x = 0; x < xGrid; ++x ) {
			int index = x + y * xGrid;
			if ( camCount > 0
				&& displayCamsSettings[index] != NULL ) {
					ofxCameraBaseSettings* setting = displayCamsSettings[index];
					switch( setting->cameraType ) {
						case PS3:
							XML.setValue( "CAMERA:TYPE", "PS3", index );
							XML.setValue( "CAMERA:GUID", GUIDToString(setting->cameraGuid), index );
							break;
							// CMU
							// FFMV
							// DIRECTSHOW
							// KINECT

						default:
							break;
					}
					XML.setValue( "CAMERA:X", setting->cameraX, index );
					XML.setValue( "CAMERA:Y", setting->cameraY, index );
					XML.setValue( "CAMERA:WIDTH", setting->cameraWidth, index );
					XML.setValue( "CAMERA:HEIGHT", setting->cameraHeight, index );
					XML.setValue( "CAMERA:FRAMERATE", setting->cameraFramerate, index );
					// TODO
			}
			//if ( camCount > 0
			//	&& displayCams[index] != NULL ) {
			//	XML.setValue( "CAMERA:TYPE", "PS3", index );
			//	XML.setValue( "CAMERA:X", x, index );
			//	XML.setValue( "CAMERA:Y", y, index );
			//	XML.setValue( "CAMERA:UUID", displayCams[index]->GetGUIDStr(), index );
			//	XML.setValue( "CAMERA:WIDTH", displayCams[index]->GetWidth(), index );
			//	XML.setValue( "CAMERA:HEIGHT", displayCams[index]->GetHeight(), index );
			//	XML.setValue( "CAMERA:HFLIP", displayCams[index]->GetHFlip(), index );
			//	XML.setValue( "CAMERA:VFLIP", displayCams[index]->GetVFlip(), index );
			//	XML.setValue( "CAMERA:AUTOGAIN", displayCams[index]->GetAutoGain(), index );
			//	XML.setValue( "CAMERA:GAIN", displayCams[index]->GetGainValue(), index );
			//	XML.setValue( "CAMERA:AUTOEXPOSURE", displayCams[index]->GetAutoExposure(), index );
			//	XML.setValue( "CAMERA:EXPOSURE", displayCams[index]->GetExposure(), index );
			//	XML.setValue( "CAMERA:AUTOWHITEBALANCE", displayCams[index]->GetAutoWhiteBalance(), index );
			//	XML.setValue( "CAMERA:WHITEBALANCE:RED", displayCams[index]->GetWhiteBalanceRed(), index );
			//	XML.setValue( "CAMERA:WHITEBALANCE:GREEN", displayCams[index]->GetWhiteBalanceGreen(), index );
			//	XML.setValue( "CAMERA:WHITEBALANCE:BLUE", displayCams[index]->GetWhiteBalanceBlue(), index );
			//}
		}
	}
	XML.saveFile( filename );
}

// ----------------------------------------------

void CamsUtils::loadXML( string filename ) {
	//! Load the file
	XML.loadFile( filename );
	printf( "CamsUtils::loadXML\tLoaded the %s\n", filename.c_str() );

	xGrid = XML.getValue( "MULTICAMS:GRIDX", 1 );
	yGrid = XML.getValue( "MULTICAMS:GRIDY", 1 );

	printf( "CamsUtils::loadXML\txGrid = %d\tyGrid = %d\n", xGrid, yGrid );

	numCamTags = XML.getNumTags( "CAMERA" );
	printf( "CamsUtils::loadXML\tnumCamTags = %d\n", numCamTags );

	//! Leave the missing camera blank
	if ( xGrid * yGrid >= numCamTags && numCamTags > 0 ) {
		//! Create the XML cameras array
		xmlCamsSettings = new ofxCameraBaseSettings*[numCamTags];
		//! Set the cameras to blank
		for ( int i = 0; i < numCamTags; ++i ) {
			xmlCamsSettings[i] = NULL;
		}
		//! Load settings from each camera.
		for ( int i = 0; i < numCamTags; ++i ) {
			int x = XML.getValue( "CAMERA:X", -1, i );
			int y = XML.getValue( "CAMERA:Y", -1, i );

			int hFlip = XML.getValue( "CAMERA:HFLIP", 0, i );
			int vFlip = XML.getValue( "CAMERA:VFLIP", 0, i );

			int bAutoGain = XML.getValue( "CAMERA:AUTOGAIN", 1, i );	// Default true
			int gainValue = XML.getValue( "CAMERA:GAIN", 0, i );

			int bAutoExposure = XML.getValue( "CAMERA:AUTOEXPOSURE", 1, i );	// Default true
			int exposureValue = XML.getValue( "CAMERA:EXPOSURE", 0, i);

			int bAutoWhiteBalance = XML.getValue( "CAMERA:AUTOWHITEBALANCE", 1, i );	// Default true
			int WBRed = XML.getValue( "CAMERA:WHITEBALANCE:RED", 0, i );
			int WBGreen = XML.getValue( "CAMERA:WHITEBALANCE:GREEN", 0, i );
			int WBBlue = XML.getValue( "CAMERA:WHITEBALANCE:BLUE", 0, i );

			printf( "CamsUtils::loadXML\th = %d\tv = %d\n", hFlip, vFlip );

			//! Setting wrong!
			if ( x >= xGrid || y >= yGrid || x == -1 || y == -1 ) {
				continue;
			}
			// TODO guid code should be improved!
			GUID guid = StringToGUID( XML.getValue( "CAMERA:UUID", "00000000-0000-0000-0000-000000000000", i ) );
			printf( "CamsUtils::loadXML\ti = %d\tGUID = %s\n", i, GUIDToString(guid).c_str() );

			// TODO check the guid validation
			
			int index = x + y * xGrid;
			xmlCamsSettings[index] = new ofxCameraBaseSettings();
			//! Set the guid of camera
			//xmlCams[index]->SetGUID( guid );
			//xmlCams[index]->SetHFlip( hFlip == 0 ? false : true );
			//xmlCams[index]->SetVFlip( vFlip == 0 ? false : true );
			//xmlCams[index]->SetAutoGain( bAutoGain == 0 ? false : true );
			//xmlCams[index]->SetGainValue( gainValue );
			//xmlCams[index]->SetAutoExposure( bAutoExposure == 0 ? false : true );
			//xmlCams[index]->SetExposure( exposureValue );
			//xmlCams[index]->SetAutoWhiteBalance( bAutoWhiteBalance == 0 ? false : true );
			//xmlCams[index]->SetWhiteBalanceRed( WBRed );
			//xmlCams[index]->SetWhiteBalanceGreen( WBGreen );
			//xmlCams[index]->SetWhiteBalanceBlue( WBBlue );
		}
	}
}


// ----------------------------------------------

int CamsUtils::getDevicesCount( bool bAll, CAMERATYPE type ) {
	// TODO
	int count = 0;

	// PS3
	if ( bAll || type == PS3 ) {
		ofxPS3* ps3 = new ofxPS3();
		count += ps3->getCameraBaseCount();
		delete ps3;	ps3 = NULL;
	}

	// CMU

	// FFMV

	// DIRECTSHOW

	// KINECT

	return count;
}

// ----------------------------------------------

GUID CamsUtils::getGUID( CAMERATYPE type, int camId ) {
	GUID guid;

	switch( type ) {
		case PS3:
			guid = CLEyeGetCameraUUID( camId );
			break;
		case FFMV:
			// TODO
			break;
		case CMU:
			// TODO
			break;
		case DIRECTSHOW:
			// TODO
			break;
		case KINECT:
			// TODO
			break;
		default:
			break;
	}

	return guid;
}

// ----------------------------------------------

void CamsUtils::createDisplayCams( int x, int y ) {
	printf( "\nCamsUtils::createDisplayCams()\n" );
	int count = x * y;

	if ( displayCams != NULL ) {
		delete displayCams;
	}
	//displayCams = new PS3*[count];
	displayCams = new ofxCameraBase*[count];
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

	//! Display camera settings
	if ( displayCamsSettings != NULL ) {
		delete displayCamsSettings;
	}
	displayCamsSettings = new ofxCameraBaseSettings*[count];
	for ( int i = 0; i < count; ++i ) {
		displayCamsSettings[i] = NULL;
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

void CamsUtils::setupCameraSettings( ofxCameraBaseSettings *settings) {
	switch( settings->cameraType ) {
		case PS3:
			settings->cameraWidth = 320;
			settings->cameraHeight = 240;
			settings->cameraFramerate = 60;
			settings->cameraDepth = 1;
			settings->cameraLeft = 0;
			settings->cameraTop = 0;
			break;
		case CMU:
			// TODO
			break;
		case FFMV:
			// TODO
			break;
		case DIRECTSHOW:
			// TODO
			break;
		case KINECT:
			// TODO
			break;
		default:
			break;
	}
}

// ----------------------------------------------

void CamsUtils::applyCameraSettings() {
	for ( int i = 0; i < camCount; ++i ) {
		for ( int j = 0; j < rawCamsSettings[i]->propertyType.size(); ++j ) {
			rawCams[i]->setCameraFeature(
				rawCamsSettings[i]->propertyType[j],
				rawCamsSettings[i]->propertyFirstValue[j],
				rawCamsSettings[i]->propertySecondValue[j],
				rawCamsSettings[i]->isPropertyAuto[j],
				rawCamsSettings[i]->isPropertyOn[j] );
		}
	}
}

// ----------------------------------------------

void CamsUtils::startCameras() {
	//for (int i=0;i<camCount;i++)
	//	rawCams[i]->startCamera();
}