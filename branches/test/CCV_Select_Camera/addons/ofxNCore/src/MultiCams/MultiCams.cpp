//! MultiCams.cpp
/*!
*  
*
*  Created by Yishi Guo on 31/05/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

#include "MultiCams.h"

MultiCams::MultiCams( bool bDebug) {
	// Add listeners
	ofAddListener( ofEvents.setup, this, &MultiCams::_setup );
	ofAddListener( ofEvents.update, this, &MultiCams::_update );
	ofAddListener( ofEvents.draw ,this, &MultiCams::_draw );
	ofAddListener( ofEvents.exit, this, &MultiCams::_exit );

	ofAddListener( ofEvents.mousePressed, this, &MultiCams::_mousePressed );
	ofAddListener( ofEvents.mouseDragged, this, &MultiCams::_mouseDragged );
	ofAddListener( ofEvents.mouseReleased, this, &MultiCams::_mouseReleased );

	ofAddListener( ofEvents.keyPressed, this, &MultiCams::_keyPressed );
	ofAddListener( ofEvents.keyReleased, this, &MultiCams::_keyReleased );


	_bDebugMode = bDebug;
}

MultiCams::~MultiCams() {

}

void MultiCams::_setup( ofEventArgs &e ) {
	// Set the title
	ofSetWindowTitle( "Multiple Camera Settings" );

	if ( _bDebugMode ) {
		// TODO
	}

}

void MultiCams::_update( ofEventArgs &e ) {

}

void MultiCams::_draw( ofEventArgs &e ) {

}

void MultiCams::_exit( ofEventArgs &e ) {

}

void MultiCams::_mousePressed( ofMouseEventArgs &e ) {

}

void MultiCams::_mouseDragged( ofMouseEventArgs &e ) {

}

void MultiCams::_mouseReleased( ofMouseEventArgs &e ) {

}

void MultiCams::_keyPressed( ofKeyEventArgs &e ) {

}

void MultiCams::_keyReleased( ofKeyEventArgs &e ) {

}

void MultiCams::handleGui(int parameterId, int task, void* data, int length) {

}


/*******************************************************
*      Load/Save Settings to XML File
********************************************************/
void MultiCams::loadXMLSettings() {
	ofLog( OF_LOG_VERBOSE, "Load MultiCams XML from" + _xmlFileName + "\n\n" );
	if ( XML.loadFile( _xmlFileName ) ) {
		ofLog( OF_LOG_VERBOSE, "Settings loaded!\n\n" );
	} else {
		ofLog( OF_LOG_VERBOSE, "No Settings Found...\n\n" );
	}
}

void MultiCams::SaveXMLSettings() {

}