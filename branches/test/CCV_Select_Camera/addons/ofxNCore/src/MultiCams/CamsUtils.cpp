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
	displayCamsSettings = NULL;
	xmlCamsSettings = NULL;

	camCount = 0;

	camsSelected = NULL;
	camsUsed = NULL;

	xGrid = 1;
	yGrid = 1;
	numCamTags = 0;
}

// ----------------------------------------------

CamsUtils::~CamsUtils() {
	saveXML();
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
	camCount = getDevicesCount(true, PS3, true);
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
		yGrid = 1;
		xmlCamsSettings = NULL;
	}

	setXY( xGrid, yGrid );

	start();
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
	int i, j, index = 0;

	//! DirectShow cameras settings apply
	for ( i = 0; i < getDevicesCount( false, DIRECTSHOW ) && index < getDevicesCount(); ++i, ++index ) {
		char* devicePath = getDevicePath( DIRECTSHOW, i );
		if ( devicePath == NULL ) {
			//printf( "DEBUG\ndevicePath == NULL\n" );
			index--;
			continue;
		}
		//printf( "DEBUG %d, devicePath = %s\n", index, devicePath );

		rawCams[index] = (ofxCameraBase*)(new ofxDShow());
		rawCams[index]->setDevicePath( devicePath );

		rawCamsSettings[index] = new ofxCameraBaseSettings();
		rawCamsSettings[index]->cameraType = DIRECTSHOW;
		rawCamsSettings[index]->devicePath = devicePath;

		//printf( "DEBUG directshow start\nrawCamsSettings[index]->devicePath=%s\n", rawCamsSettings[index]->devicePath );

		setupCameraSettings( rawCamsSettings[index] );

		//// DEBUG
		//rawCamsSettings[index]->print();

		if ( xmlCamsSettings != NULL ) {
			for ( j = 0; j < numCamTags; ++j ) {
				// DEBUG
				//printf( "CMP\nraw:\n%s\nxml:\n%s\n", devicePath, xmlCamsSettings[j]->devicePath );
				if ( xmlCamsSettings[j] != NULL
					&& strcmp( devicePath, xmlCamsSettings[j]->devicePath) == 0 ) {
						copySettingsFromXmlSettings( xmlCamsSettings[j], rawCamsSettings[index] );
						//printf( "DEBUG\n" );
						//printf( "========____=========\n" );
						//rawCamsSettings[index]->print();
						//printf( "________====_________\n" );
						//xmlCamsSettings[j]->print();
						//printf( "++++++++====+++++++++\n" );

						setCam( xmlCamsSettings[j]->cameraX, xmlCamsSettings[j]->cameraY, rawCams[index] );
						setSelected( index );
				}
			}
		}

		rawCamsSettings[index]->print();

		rawCams[index]->initializeWithDevicePath(
			rawCamsSettings[index]->devicePath,
			rawCamsSettings[index]->cameraWidth,
			rawCamsSettings[index]->cameraHeight,
			rawCamsSettings[index]->cameraLeft,
			rawCamsSettings[index]->cameraTop,
			rawCamsSettings[index]->cameraDepth,
			rawCamsSettings[index]->cameraFramerate );

		//rawCamsSettings[index]->print();
	}

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

		if ( xmlCamsSettings != NULL ) {
			for ( j = 0; j < numCamTags; ++j ) {
				printf( "CamsUtils::start\tGUID1 = %s\n\t\t\tGUID2 = %s\n\n",
					GUIDToString(guid).c_str(),
					GUIDToString(xmlCamsSettings[j]->cameraGuid).c_str() );

				if ( xmlCamsSettings[j] != NULL
					&& EqualGUID( guid, xmlCamsSettings[j]->cameraGuid ) ) {

						printf( "\nEqual BEGIN\n" );
						copySettingsFromXmlSettings( xmlCamsSettings[j], rawCamsSettings[index] );

						setCam( xmlCamsSettings[j]->cameraX, xmlCamsSettings[j]->cameraY, rawCams[index] );
						setSelected( index );
						printf( "\nEqual END\n" );
				}
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

		////! Print the setting for debug
		//rawCamsSettings[index]->print();
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

	printf( "\nBEFORE start\n" );
	startCameras();
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
	//printf( "DEBUG CamsUtils::getCount() %d\n", camCount );
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
		if ( cam->getCameraType() == DIRECTSHOW 
			&& rawCams[i]->getCameraType() == DIRECTSHOW ) {
			if ( cam->getDevicePath() != NULL 
				&& rawCams[i]->getDevicePath() != NULL
				&& strcmp( cam->getDevicePath(), rawCams[i]->getDevicePath() ) == 0 ) {
				return i;
			}
		} else {
			if ( EqualGUID( cam->getCameraGUID(), rawCams[i]->getCameraGUID() ) ) {
				return i;
			}
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

// ----------------------------------------------

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

ofxCameraBaseSettings* CamsUtils::getRawCamSettings( int index ) {
	if ( rawCamsSettings != NULL
		&& camCount > index
		&& index >= 0
		) {
			return rawCamsSettings[index];
	}

	return NULL;
}

// ----------------------------------------------

void CamsUtils::setRawCamFeature( int rawId, CAMERA_BASE_FEATURE feature, int firstValue, int secondValue, bool isAuto, bool isEnable ) {
	ofxCameraBase* camera = getRawCam( rawId );
	ofxCameraBaseSettings* settings = getRawCamSettings( rawId );

	if ( camera != NULL ) {
		camera->setCameraFeature( feature, firstValue, secondValue, isAuto, isEnable );
		printf( "DEBUG\nCamsUtils::setRawCamFeature()\tfirstValue\n", firstValue );
	}
	if ( settings != NULL ) {
		settings->setFeature( feature, firstValue, secondValue, isAuto, isEnable );
	}
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
	displayCamsSettings[index] = getRawCamSettings( getRawId( cam ) );
	printf( "DEBUG\nCamsUtils::setCam()\ngetRawId() = %d\n", getRawId(cam) );
	printf( "DEBUG\nCamsUtils::setCam()\ngetRawCamSettings() == NULL ? %d\n", getRawCamSettings(getRawId(cam)) == NULL ? true : false );
	//printf( "==================\n" );
	//if ( displayCamsSettings[index] != NULL ) {
	//	displayCamsSettings[index]->print();
	//}
	//printf( "-----========-----\n" );
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
	if ( camCount <= 0 ) {
		return;
	}
	for ( int i = 0; i < xGrid*yGrid; ++i ) {
		if ( displayCams[i] == NULL ) {
			printf( "displayCams[%d] == NULL\n", i );
			return;
		}
		if ( displayCamsSettings[i] == NULL ) {
			printf( "displayCamsSettings[%d] == NULL\n", i );
			return;
		}
	}
	printf( "DEBUG\nCamsUtils::saveXML() before from raw settings()\n" );
	//! Receive settings from rawCamsSettings
	receiveSettingsFromRawSettings();
	printf( "DEBUG\nCamsUtils::saveXML() after from raw settings()\n" );
	//! Receive settings from current cameras
	receiveSettingsFromCameras();
	printf( "DEBUG\nCamsUtils::saveXML() after from cameras()\n" );

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
					ofxCameraBaseSettings* settings = displayCamsSettings[index];
					//settings->print();
					switch( settings->cameraType ) {
						case PS3:
							XML.setValue( "CAMERA:TYPE", CameraTypeToStr( settings->cameraType ), index );
							XML.setValue( "CAMERA:GUID", GUIDToString(settings->cameraGuid), index );
							break;
							// CMU
							// FFMV
							// DIRECTSHOW
						case DIRECTSHOW:
							XML.setValue( "CAMERA:TYPE", CameraTypeToStr( settings->cameraType ), index );
							XML.setValue( "CAMERA:DEVICEPATH", string(settings->devicePath), index );
							break;
							// KINECT

						default:
							break;
					}
					XML.setValue( "CAMERA:X", settings->cameraX, index );
					XML.setValue( "CAMERA:Y", settings->cameraY, index );
					XML.setValue( "CAMERA:WIDTH", settings->cameraWidth, index );
					XML.setValue( "CAMERA:HEIGHT", settings->cameraHeight, index );
					XML.setValue( "CAMERA:FRAMERATE", settings->cameraFramerate, index );
					XML.setValue( "CAMERA:DEPTH", settings->cameraDepth, index );
					XML.setValue( "CAMERA:LEFT", settings->cameraLeft, index );
					XML.setValue( "CAMERA:TOP", settings->cameraTop, index );
					XML.setValue( "CAMERA:INDEX", settings->cameraIndex, index );
					XML.setValue( "CAMERA:VIDEOPLAYERON", settings->videoPlayerOn, index );
					XML.setValue( "CAMERA:VIDEORECORDERON", settings->videoRecorderOn, index );

					for ( int i = 0; i < settings->propertyType.size(); ++i ) {
						switch( settings->propertyType[i] ) {
							case BASE_AUTO_EXPOSURE:
								XML.setValue( "CAMERA:AUTOEXPOSURE", settings->propertyFirstValue[i], index );
								break;
							case BASE_AUTO_GAIN:
								XML.setValue( "CAMERA:AUTOGAIN", settings->propertyFirstValue[i], index );
								break;
							case BASE_AUTO_WHITE_BALANCE:
								XML.setValue( "CAMERA:AUTOWHITEBALANCE", settings->propertyFirstValue[i], index );
								break;
							case BASE_EXPOSURE:
								XML.setValue( "CAMERA:EXPOSURE", settings->propertyFirstValue[i], index );
								break;
							case BASE_GAIN:
								XML.setValue( "CAMERA:GAIN", settings->propertyFirstValue[i], index );
								//XML.setAttribute( "CAMERA:GAIN", "ON", settings->isPropertyOn[i], index );
								break;
							case BASE_HFLIP:
								XML.setValue( "CAMERA:HFLIP", settings->propertyFirstValue[i], index );
								break;
							case BASE_VFLIP:
								XML.setValue( "CAMERA:VFLIP", settings->propertyFirstValue[i], index );
								break;
							case BASE_WHITE_BALANCE_BLUE:
								XML.setValue( "CAMERA:WHITEBALANCEBLUE", settings->propertyFirstValue[i], index );
								break;
							case BASE_WHITE_BALANCE_GREEN:
								XML.setValue( "CAMERA:WHITEBALANCEGREEN", settings->propertyFirstValue[i], index );
								break;
							case BASE_WHITE_BALANCE_RED:
								XML.setValue( "CAMERA:WHITEBALANCERED", settings->propertyFirstValue[i], index );
								break;
							case BASE_WHITE_BALANCE:
								XML.setValue( "CAMERA:WHITE_BALANCE", settings->propertyFirstValue[i], index );
								break;
							case BASE_BRIGHTNESS:
								XML.setValue( "CAMERA:BRIGHTNESS", settings->propertyFirstValue[i], index );
								break;
							case BASE_SHARPNESS:
								XML.setValue( "CAMERA:SHARPNESS", settings->propertyFirstValue[i], index );
								break;
							case BASE_GAMMA:
								XML.setValue( "CAMERA:GAMMA", settings->propertyFirstValue[i], index );
								break;
							case BASE_CONTRAST:
								XML.setValue( "CAMERA:CONTRAST", settings->propertyFirstValue[i], index );
								break;
							case BASE_HUE:
								XML.setValue( "CAMERA:HUE", settings->propertyFirstValue[i], index );
								break;
							case BASE_SATURATION:
								XML.setValue( "CAMERA:SATURATION", settings->propertyFirstValue[i], index );
								break;
								 //TODO other features
							default:
								break;
						}
					}
					// TODO
			}
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
	if ( xGrid > 0 && yGrid > 0 /*&& xGrid * yGrid >= numCamTags*/ && numCamTags > 0 ) {
		//! Create the XML cameras array
		xmlCamsSettings = new ofxCameraBaseSettings*[numCamTags];

		//! Load settings from each camera.
		for ( int i = 0; i < numCamTags; ++i ) {
			xmlCamsSettings[i] = new ofxCameraBaseSettings();
			int settingValue;

			xmlCamsSettings[i]->cameraGuid = StringToGUID( XML.getValue( "CAMERA:GUID", "00000000-0000-0000-0000-000000000000", i ) );
			// TODO check the guid validation
			string devicePathStr = XML.getValue( "CAMERA:DEVICEPATH", "NULL", i );
			char* devicePath = new char[devicePathStr.length()+1];
			strcpy( devicePath, devicePathStr.c_str() );
			xmlCamsSettings[i]->devicePath = devicePath;
			//printf( "DEBUGxml\nxmlCamsSettings[%d]->devicePath=%s\n", i, xmlCamsSettings[i]->devicePath );
			//std::cout << "DEBUG\t" << devicePath << endl;
			//std::cout << XML.getValue( "CAMERA:DEVICEPATH", "NULL", i ).c_str() << endl;
			xmlCamsSettings[i]->cameraType = StrToCameraType( XML.getValue( "CAMERA:TYPE", "NULL", i ) );

			xmlCamsSettings[i]->cameraX = XML.getValue( "CAMERA:X", -1, i );
			xmlCamsSettings[i]->cameraY = XML.getValue( "CAMERA:Y", -1, i );

			settingValue = XML.getValue( "CAMERA:WIDTH", -1, i );
				xmlCamsSettings[i]->cameraWidth = settingValue;

			settingValue = XML.getValue( "CAMERA:HEIGHT", -1, i );
				xmlCamsSettings[i]->cameraHeight = settingValue;

			settingValue = XML.getValue( "CAMERA:FRAMERATE", -1, i );
				xmlCamsSettings[i]->cameraFramerate = settingValue;

			settingValue = XML.getValue( "CAMERA:DEPTH", -1, i );
				xmlCamsSettings[i]->cameraDepth = settingValue;

			settingValue = XML.getValue( "CAMERA:LEFT", -1, i );
				xmlCamsSettings[i]->cameraLeft = settingValue;

			settingValue = XML.getValue( "CAMERA:TOP", -1, i );
				xmlCamsSettings[i]->cameraTop = settingValue;

			settingValue = XML.getValue( "CAMERA:INDEX", -1, i );
				xmlCamsSettings[i]->cameraIndex = settingValue;
			settingValue = XML.getValue( "CAMERA:VIDEOPLAYERON", -1, i );
				xmlCamsSettings[i]->videoPlayerOn = settingValue == 1 ? true : false;

			settingValue = XML.getValue( "CAMERA:VIDEORECORDERON", -1, i );
				xmlCamsSettings[i]->videoRecorderOn = settingValue == 1 ? true : false;

			//! wrong data, removed
			if ( xmlCamsSettings[i]->cameraX >= xGrid
				|| xmlCamsSettings[i]->cameraY >= yGrid
				|| xmlCamsSettings[i]->cameraX == -1
				|| xmlCamsSettings[i]->cameraY == -1 ) {
					delete xmlCamsSettings[i];
					xmlCamsSettings[i] = NULL;
					continue;
			}

			settingValue = XML.getValue( "CAMERA:HFLIP", -1, i );
			if ( settingValue >= 0 ) {
				xmlCamsSettings[i]->propertyType.push_back( BASE_HFLIP );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );
			}

			settingValue = XML.getValue( "CAMERA:VFLIP", -1, i );
			if ( settingValue >= 0 ) {
				xmlCamsSettings[i]->propertyType.push_back( BASE_VFLIP );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );
			}

			settingValue = XML.getValue( "CAMERA:AUTOGAIN", -1, i );
			if ( settingValue >= 0 ) {
				xmlCamsSettings[i]->propertyType.push_back( BASE_AUTO_GAIN );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );
			}

			settingValue = XML.getValue( "CAMERA:GAIN", -1, i );
			if ( settingValue >= 0 ) {
				xmlCamsSettings[i]->propertyType.push_back( BASE_GAIN );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );
			}

			settingValue = XML.getValue( "CAMERA:AUTOEXPOSURE", -1, i );
			if ( settingValue >= 0 ) {
				xmlCamsSettings[i]->propertyType.push_back( BASE_AUTO_EXPOSURE );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );
			}
			settingValue = XML.getValue( "CAMERA:EXPOSURE", -1, i );
			if ( settingValue >= 0 ) {
				xmlCamsSettings[i]->propertyType.push_back( BASE_EXPOSURE );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );
			}
			settingValue = XML.getValue( "CAMERA:AUTOWHITEBALANCE", -1, i );
			if ( settingValue >= 0 ) {
				xmlCamsSettings[i]->propertyType.push_back( BASE_AUTO_WHITE_BALANCE );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );
			}
			settingValue = XML.getValue( "CAMERA:WHITEBALANCEBLUE", -1, i );
			if ( settingValue >= 0 ) {
				xmlCamsSettings[i]->propertyType.push_back( BASE_WHITE_BALANCE_BLUE );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );
			}
			settingValue = XML.getValue( "CAMERA:WHITEBALANCEGREEN", -1, i );
			if ( settingValue >= 0 ) {
				xmlCamsSettings[i]->propertyType.push_back( BASE_WHITE_BALANCE_GREEN );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );
			}
			settingValue = XML.getValue( "CAMERA:WHITEBALANCERED", -1, i );
			if ( settingValue >= 0 ) {
				xmlCamsSettings[i]->propertyType.push_back( BASE_WHITE_BALANCE_RED );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );
			}

			settingValue = XML.getValue( "CAMERA:BRIGHTNESS", -1, i );

				xmlCamsSettings[i]->propertyType.push_back( BASE_BRIGHTNESS );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );

			settingValue = XML.getValue( "CAMERA:CONTRAST", -1, i );

				xmlCamsSettings[i]->propertyType.push_back( BASE_CONTRAST );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );

			settingValue = XML.getValue( "CAMERA:HUE", -1, i );

				xmlCamsSettings[i]->propertyType.push_back( BASE_HUE );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );

			settingValue = XML.getValue( "CAMERA:GAMMA", -1, i );

				xmlCamsSettings[i]->propertyType.push_back( BASE_GAMMA );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );

			settingValue = XML.getValue( "CAMERA:SATURATION", -1, i );

				xmlCamsSettings[i]->propertyType.push_back( BASE_SATURATION );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );

			settingValue = XML.getValue( "CAMERA:SHARPNESS", -1, i );

				xmlCamsSettings[i]->propertyType.push_back( BASE_SHARPNESS );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );

			settingValue = XML.getValue( "CAMERA:WHITE_BALANCE", -1, i );

				xmlCamsSettings[i]->propertyType.push_back( BASE_WHITE_BALANCE );
				xmlCamsSettings[i]->propertyFirstValue.push_back( settingValue );
				xmlCamsSettings[i]->propertySecondValue.push_back( 0 );
				xmlCamsSettings[i]->isPropertyAuto.push_back( false );
				xmlCamsSettings[i]->isPropertyOn.push_back( true );

			// TODO
			// Others

			//// DEBUG
			//printf( ">>>>XML settings%d\n", i );
			//xmlCamsSettings[i]->print();
			//printf( ">>>XML path\n%s\n", xmlCamsSettings[i]->devicePath );
		}
	}
}


// ----------------------------------------------

int CamsUtils::getDevicesCount( bool bAll, CAMERATYPE type, bool bPure ) {
	// TODO
	int count = 0;

	// PS3
	if ( bAll || type == PS3 ) {
		ofxPS3* ps3 = new ofxPS3();
		count += ps3->getCameraBaseCount();
		delete ps3;	ps3 = NULL;
		//printf( "DEBUG\nPS3\tcount=%d\n", count );
	}

	// CMU

	// FFMV

	// DIRECTSHOW
	if ( bAll || type == DIRECTSHOW ) {
		videoInput* VI = new videoInput();
		count += VI->getDeviceCount( bPure );
		delete VI;	VI = NULL;
		//printf( "DEBUG\nVI\tcount=%d\n", count );
	}

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

char* CamsUtils::getDevicePath( CAMERATYPE type, int camId ) {
	char* result = NULL;

	switch( type ) {
		case DIRECTSHOW:
			{
				videoInput* VI = new videoInput();
				VI->listDevices( true );
				result = VI->getDevicePath( camId );
				delete VI; VI = NULL;
			}
			break;
		default:
			// TODO
			break;
	}

	return result;
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
			
			//! Horizontal Flip
			settings->propertyType.push_back( BASE_HFLIP );
			settings->propertyFirstValue.push_back( 0 );	// false
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			//! Vertical Flip
			settings->propertyType.push_back( BASE_VFLIP );
			settings->propertyFirstValue.push_back( 0 );	// false
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			//! Auto Gain
			settings->propertyType.push_back( BASE_AUTO_GAIN );
			settings->propertyFirstValue.push_back( 1 );	// true
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			//! Gain
			settings->propertyType.push_back( BASE_GAIN );
			settings->propertyFirstValue.push_back( 0 );	// 0
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			//! Auto Exposure
			settings->propertyType.push_back( BASE_AUTO_EXPOSURE );
			settings->propertyFirstValue.push_back( 1 );	// true
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			//! Exposure
			settings->propertyType.push_back( BASE_EXPOSURE );
			settings->propertyFirstValue.push_back( 0 );	// 0
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			//! Auto white balance
			settings->propertyType.push_back( BASE_AUTO_WHITE_BALANCE );
			settings->propertyFirstValue.push_back( 1 );	// true
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			//! White balance BLUE
			settings->propertyType.push_back( BASE_WHITE_BALANCE_BLUE );
			settings->propertyFirstValue.push_back( 0 );	// 0
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			//! White balance GREEN
			settings->propertyType.push_back( BASE_WHITE_BALANCE_GREEN );
			settings->propertyFirstValue.push_back( 0 );	// 0
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			//! White balance RED
			settings->propertyType.push_back( BASE_WHITE_BALANCE_RED );
			settings->propertyFirstValue.push_back( 0 );	// 0
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			break;
		case CMU:
			// TODO
			break;
		case FFMV:
			// TODO
			break;
		case DIRECTSHOW:
			settings->cameraWidth = 320;
			settings->cameraHeight = 240;
			settings->cameraFramerate = 60;
			settings->cameraDepth = 1;
			settings->cameraLeft = 0;
			settings->cameraTop = 0;

			settings->propertyType.push_back( BASE_BRIGHTNESS );	// -10,000 ~ 10,000
			settings->propertyFirstValue.push_back( 5000 );
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			settings->propertyType.push_back( BASE_CONTRAST );		// 0 ~ 10,000
			settings->propertyFirstValue.push_back( 5000 );
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			settings->propertyType.push_back( BASE_HUE );			// -180,000 ~ 180,000
			settings->propertyFirstValue.push_back( 0 );
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			settings->propertyType.push_back( BASE_GAMMA );			// 1 ~ 500
			settings->propertyFirstValue.push_back( 0 );
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			//settings->propertyType.push_back( BASE_GAIN );			// depends
			//settings->propertyFirstValue.push_back( 0 );
			//settings->propertySecondValue.push_back( 0 );
			//settings->isPropertyAuto.push_back( false );
			//settings->isPropertyOn.push_back( true );

			settings->propertyType.push_back( BASE_SATURATION );	// 0 ~ 10,000
			settings->propertyFirstValue.push_back( 0 );
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			settings->propertyType.push_back( BASE_SHARPNESS );		// 0 ~ 100
			settings->propertyFirstValue.push_back( 0 );
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

			settings->propertyType.push_back( BASE_WHITE_BALANCE );	// depends
			settings->propertyFirstValue.push_back( 0 );
			settings->propertySecondValue.push_back( 0 );
			settings->isPropertyAuto.push_back( false );
			settings->isPropertyOn.push_back( true );

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

void CamsUtils::receiveSettingsFromCameras() {
	//! We just receive the display cameras (not all cams)
	for ( int i = 0; i < xGrid * yGrid; ++i ) {
		if ( !displayCamsSettings[i]->videoPlayerOn ) {
			for ( int j = 0; j < displayCamsSettings[i]->propertyType.size(); ++j ) {

				int firstValue, secondValue;
				bool isAuto, isOn;
				int minValue, maxValue;

				displayCams[i]->getCameraFeature( 
					displayCamsSettings[i]->propertyType[j],
					&firstValue, &secondValue, &isAuto, &isOn, &minValue, &maxValue );

				displayCamsSettings[i]->propertyFirstValue[j] = firstValue;
				displayCamsSettings[i]->propertySecondValue[j] = secondValue;
				displayCamsSettings[i]->isPropertyAuto[j] = isAuto;
				displayCamsSettings[i]->isPropertyOn[j] = isOn;
			}
		}
	}
}

// ----------------------------------------------

void CamsUtils::receiveSettingsFromRawSettings() {
	if ( displayCams == NULL ) {
		return;
	}
	for ( int x = 0; x < xGrid; ++x ) {
		for ( int y = 0; y < yGrid; ++y ) {
			int i = x + y * xGrid;
			bool bGot = false;
			for ( int j = 0; j < camCount && !bGot; ++j ) {
				switch( displayCams[i]->getCameraType() ) {
					case DIRECTSHOW:
						printf( "DEBUG\nDDDD\n" );
						if ( strcmp( displayCams[i]->getDevicePath(), rawCams[j]->getDevicePath() ) == 0 ) {
							displayCamsSettings[i]->cameraX		= x;
							displayCamsSettings[i]->cameraY		= y;
							displayCamsSettings[i]->cameraIndex	= i;
							bGot = true;
						}
						break;
					case PS3:
					case FFMV:
					case CMU:
						if ( EqualGUID( displayCams[i]->getCameraGUID(), rawCams[j]->getCameraGUID() ) ) {
							displayCamsSettings[i]->cameraX				= x;
							displayCamsSettings[i]->cameraY				= y;
							displayCamsSettings[i]->cameraIndex			= i;
							bGot = true;
							break;
						}
						break;
					default:
						break;
				}
			}
		}
	}
}

// ----------------------------------------------

void CamsUtils::copySettingsFromXmlSettings( ofxCameraBaseSettings *src, ofxCameraBaseSettings *dst ) {
	dst->cameraGuid	= src->cameraGuid;
	dst->cameraType = src->cameraType;
	dst->pixelMode = src->pixelMode;
	if ( src->cameraDepth >= 0 ) {
		dst->cameraDepth = src->cameraDepth;
	}
	if ( src->cameraX >= 0 ) {
		dst->cameraX = src->cameraX;
	}
	if ( src->cameraY >= 0 ) {
		dst->cameraY = src->cameraY;
	}
	if ( src->cameraWidth >= 0 ) {
		dst->cameraWidth = src->cameraWidth;
	}
	if ( src->cameraHeight >= 0 ) {
		dst->cameraHeight = src->cameraHeight;
	}
	if ( src->cameraLeft >= 0 ) {
		dst->cameraLeft = src->cameraLeft;
	}
	if ( src->cameraTop >= 0 ) {
		dst->cameraTop = src->cameraTop;
	}
	if ( src->cameraIndex >= 0 ) {
		dst->cameraIndex = src->cameraIndex;
	}
	if ( src->cameraFramerate >= 0 ) {
		dst->cameraFramerate = src->cameraFramerate;
	}
		
	dst->videoPlayerOn = src->videoPlayerOn;
	dst->videoRecorderOn = src->videoRecorderOn;

	//! dst: rawCams src: xmlCams
	for ( int i = 0; i < dst->propertyType.size(); ++i ) {
		for ( int j = 0; j < src->propertyType.size(); ++j ) {
			if ( dst->propertyType[i] == src->propertyType[j] ) {
				dst->isPropertyOn[i]			= src->isPropertyOn[j];
				dst->isPropertyAuto[i]			= src->isPropertyAuto[j];
				dst->propertyFirstValue[i]		= src->propertyFirstValue[j];
				dst->propertySecondValue[i]		= src->propertySecondValue[j];
			}
		}
	}

	// TODO copy the calibration stuff
	//dst->calibrationPoints		= src->calibrationPoints;
}

// ----------------------------------------------

void CamsUtils::startCameras() {
	//for (int i=0;i<camCount;i++)
	//	rawCams[i]->startCamera();
	// DEBUG
	//for ( int i = 0; i < camCount; ++i ) {
	//	printf( "+++++++++++++++++\n" );
	//	rawCamsSettings[i]->print();
	//	printf( "-----------------\n" );
	//}
}

// ----------------------------------------------