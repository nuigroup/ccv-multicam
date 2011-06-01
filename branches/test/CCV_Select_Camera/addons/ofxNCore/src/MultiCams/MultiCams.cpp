//! MultiCams.cpp
/*!
*  
*
*  Created by Yishi Guo on 31/05/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

#include "MultiCams.h"
#include "Modules/ofxNCoreVision.h"

//MultiCams::MultiCams( bool bDebug) {
//	// Add listeners
//	//ofAddListener( ofEvents.setup, this, &MultiCams::_setup );
//	//ofAddListener( ofEvents.update, this, &MultiCams::_update );
//	//ofAddListener( ofEvents.draw ,this, &MultiCams::_draw );
//	//ofAddListener( ofEvents.exit, this, &MultiCams::_exit );
//
//	ofAddListener( ofEvents.mousePressed, this, &MultiCams::_mousePressed );
//	ofAddListener( ofEvents.mouseDragged, this, &MultiCams::_mouseDragged );
//	ofAddListener( ofEvents.mouseReleased, this, &MultiCams::_mouseReleased );
//
//	ofAddListener( ofEvents.keyPressed, this, &MultiCams::_keyPressed );
//	ofAddListener( ofEvents.keyReleased, this, &MultiCams::_keyReleased );
//
//
//	bDebugMode = bDebug;
//	bFullMode = true;	//! show all controls
//	_xmlFileName = "multicams.xml";
//}

MultiCams::MultiCams() {
	ofAddListener( ofEvents.mousePressed, this, &MultiCams::_mousePressed );
	ofAddListener( ofEvents.mouseDragged, this, &MultiCams::_mouseDragged );
	ofAddListener( ofEvents.mouseReleased, this, &MultiCams::_mouseReleased );

	ofAddListener( ofEvents.keyPressed, this, &MultiCams::_keyPressed );
	ofAddListener( ofEvents.keyReleased, this, &MultiCams::_keyReleased );

	controls = ofxGui::Instance( this );
	setupControls();
}
//--------------------------------------------------------------

MultiCams::~MultiCams() {

}
//--------------------------------------------------------------

void MultiCams::setup() {
	testFont.loadFont( "verdana.ttf", 50, true, true );


}
//--------------------------------------------------------------


void MultiCams::_mousePressed( ofMouseEventArgs &e ) {

}
//--------------------------------------------------------------

void MultiCams::_mouseDragged( ofMouseEventArgs &e ) {

}

void MultiCams::_mouseReleased( ofMouseEventArgs &e ) {

}
//--------------------------------------------------------------

void MultiCams::_keyPressed( ofKeyEventArgs &e ) {

}
//--------------------------------------------------------------

void MultiCams::_keyReleased( ofKeyEventArgs &e ) {

}
//--------------------------------------------------------------

/*******************************************************
*      GUI
********************************************************/
void MultiCams::handleGui(int parameterId, int task, void* data, int length) {
	// DEBUG
	printf( "MultiCams::handGui\n" );
	switch( parameterId ) {
		case generalSettingsPanel_save:
			if ( length == sizeof(bool) ) {
				if (*(bool*)data) {
					// TODO save the settings to XML file
					if( _coreVision != NULL ) {
						_coreVision->switchMultiCamsGUI( false );
					}
				}
			}
			break;
		case generalSettingsPanel_cancel:
			if ( length == sizeof(bool) ) {
				if (*(bool*)data) {
					if ( _coreVision != NULL ) {
						_coreVision->switchMultiCamsGUI( false );
					}
				}
			}
			break;
		default:
			break;
	}
}
//--------------------------------------------------------------

void MultiCams::setupControls() {

	//panel border color
	controls->mGlobals->mBorderColor.r = 0;
	controls->mGlobals->mBorderColor.g = 0;
	controls->mGlobals->mBorderColor.b = 0;
	controls->mGlobals->mBorderColor.a = .3;
	//panel color
	controls->mGlobals->mCoverColor.r = 1;
	controls->mGlobals->mCoverColor.g = 1;
	controls->mGlobals->mCoverColor.b = 1;
	controls->mGlobals->mCoverColor.a = .4;
	//control outline color
	controls->mGlobals->mFrameColor.r = 0;
	controls->mGlobals->mFrameColor.g = 0;
	controls->mGlobals->mFrameColor.b = 0;
	controls->mGlobals->mFrameColor.a = .3;
	//text color
	controls->mGlobals->mTextColor.r = 0;
	controls->mGlobals->mTextColor.g = 0;
	controls->mGlobals->mTextColor.b = 0;
	controls->mGlobals->mTextColor.a = 1;
	//button color
	controls->mGlobals->mButtonColor.r = 1;
	controls->mGlobals->mButtonColor.g = 0;
	controls->mGlobals->mButtonColor.b = 0;
	controls->mGlobals->mButtonColor.a = .8;
	//slider tip color
	controls->mGlobals->mHandleColor.r = 0;
	controls->mGlobals->mHandleColor.g = 0;
	controls->mGlobals->mHandleColor.b = 0;
	//slider color
	controls->mGlobals->mSliderColor.r = 1;
	controls->mGlobals->mSliderColor.g = 0;
	controls->mGlobals->mSliderColor.b = 0;
	controls->mGlobals->mSliderColor.a = .8;
}
//--------------------------------------------------------------

void MultiCams::addPanels() {
	ofxGuiPanel* displayPanel = controls->addPanel(
		this->camerasDisplayPanel, "Cameras Display" , 30, 30,
		OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
	displayPanel->mObjWidth = 600;
	displayPanel->mObjHeight = 360;

	ofxGuiPanel* settingsPanel = controls->addPanel(
		this->generalSettingsPanel, "General Settings", 660, 30,
		OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
	settingsPanel->addButton(
		this->generalSettingsPanel_save, "Save", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
		kofxGui_Button_Off, kofxGui_Button_Trigger );
	settingsPanel->addButton(
		this->generalSettingsPanel_cancel, "Cancel", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
		kofxGui_Button_Off, kofxGui_Button_Trigger );
	settingsPanel->mObjWidth = 260;
	settingsPanel->mObjHeight = 160;

	ofxGuiPanel* devicesPanel = controls->addPanel(
		this->devicesListPanel, "Devices List", 30, 420,
		OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
	devicesPanel->mObjWidth = 600;
	devicesPanel->mObjHeight = 150;

	ofxGuiPanel* infoPanel = controls->addPanel(
		this->informationPanel, "Information Settings", 660, 220,
		OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
	infoPanel->mObjWidth = 260;
	infoPanel->mObjHeight = 350;

	//!
	controls->forceUpdate( true );
	controls->activate( true );
}
//--------------------------------------------------------------

void MultiCams::removePanels() {
	controls->removePanel( this->camerasDisplayPanel );
	controls->removePanel( this->generalSettingsPanel );
	controls->removePanel( this->devicesListPanel );
	controls->removePanel( this->informationPanel );
}
//--------------------------------------------------------------

void MultiCams::draw() {
	ofSetColor( 0x123456 );
	ofFill();
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );

	string testStr = ofToString( testInt++ );

	// Set the font color to white.
	ofSetColor( 0xFFFFFF );

	//! Draw the test string
	testFont.drawString(
		testStr,
		ofGetWidth()/2 - testFont.stringWidth( testStr )/2,
		ofGetHeight()/2 - testFont.stringHeight( testStr )/2
	);

	controls->draw();

}
//--------------------------------------------------------------


/*******************************************************
*      PassIn Methods
********************************************************/

void MultiCams::passInCoreVision( ofxNCoreVision* core ) {
	_coreVision = core;
}
//--------------------------------------------------------------


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
//--------------------------------------------------------------
void MultiCams::SaveXMLSettings() {
	//! Set values
	XML.setValue( "CONFIG:APPLICATION:TITLE", windowTitle );

	//! Save the file
	XML.saveFile( _xmlFileName );
}
//--------------------------------------------------------------