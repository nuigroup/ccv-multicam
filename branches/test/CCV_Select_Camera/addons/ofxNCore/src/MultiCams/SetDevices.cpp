//! SetDevices.cpp
/*!
*  
*
*  Created by Yishi Guo on 31/05/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

#include "SetDevices.h"

SetDevices::SetDevices( bool bDebug ) {

}

void SetDevices::setup() {
	testInt = 0;
	testFont.loadFont( "verdana.ttf", 20, true, true, false );
}

void SetDevices::update() {
	testInt++;
}

void SetDevices::draw() {
	testFont.drawString( ofToString(testInt), ofGetWidth()/2, ofGetHeight()/2 );
}

void SetDevices::exit() {

}