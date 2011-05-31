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


	bDebugMode = bDebug;
	bFullMode = true;	//! show all controls
	_xmlFileName = "multicams.xml";
}

MultiCams::~MultiCams() {

}

void MultiCams::_setup( ofEventArgs &e ) {
	//! Load the settings first
	this->loadXMLSettings();

	// Set the title
	ofSetWindowTitle( windowTitle );

	//! Setup window properties
	ofSetWindowShape( winWidth, winHeight );
	ofSetVerticalSync( false );

	//! Load font
	testFont.loadFont( "verdana.ttf", 8, true, true );

	//! GUI controls
	controls = ofxGui::Instance( this );
	setupControls();


	if ( bDebugMode ) {
		// TODO
	}

	if ( bFullMode ) {
		bShowInterface = true;
	}

	ofLog( OF_LOG_VERBOSE, "Multiple Cameras Configuration is setup!\n\n" );

}

void MultiCams::_update( ofEventArgs &e ) {
	testInt++;
}

void MultiCams::_draw( ofEventArgs &e ) {
	if ( bShowConfiguration ) {
		if ( bShowInterface ) {
			testFont.drawString( ofToString( testInt ),
			ofGetWidth()/2,
			ofGetHeight()/2 );
		}
	}
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

void MultiCams::setupControls() {

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

	this->windowTitle	= XML.getValue( "CONFIG:APPLICATION:TITLE", "Multiple Camera Settings" );
	this->bDebugMode	= XML.getValue( "CONFIG:BOOLEAN:DEBUG", 0 );
	this->winWidth		= XML.getValue( "CONFIG:WINDOW:WIDTH", 950 );
	this->winHeight		= XML.getValue( "CONFIG:WINDOW:HEIGHT", 600 );

}

void MultiCams::SaveXMLSettings() {
	//! Set values
	XML.setValue( "CONFIG:APPLICATION:TITLE", windowTitle );

	//! Save the file
	XML.saveFile( _xmlFileName );
}