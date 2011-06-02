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
}
//--------------------------------------------------------------

MultiCams::~MultiCams() {

}
//--------------------------------------------------------------

void MultiCams::setup() {
	testInt = 0;
	testFont.loadFont( "verdana.ttf", 50, true, true );

	controls = ofxGui::Instance( this );
	setupControls();
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
		////////////////////////////////////
		// GENERAL SETTINGS
		//! Start settings
		case generalSettingsPanel_start:
			if ( length == sizeof(bool) ) {
				if (*(bool*)data) {
					removePanel( generalSettingsPanel );
					addPanel( step1Panel );
				}
			}
			break;
		//! Save settings and exit
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
		//! Exit (Not settings saved)
		case generalSettingsPanel_cancel:
			if ( length == sizeof(bool) ) {
				if (*(bool*)data) {
					if ( _coreVision != NULL ) {
						_coreVision->switchMultiCamsGUI( false );
					}
				}
			}
			break;
		////////////////////////////////////////
		// STEP 1
		//! Previous
		case step1Panel_previous:
			if ( length == sizeof(bool) ) {
				if (*(bool*)data) {
					removePanel( step1Panel );
					addPanel( generalSettingsPanel );
				}
			}
			break;
		//! Next - Go to step 2
		case step1Panel_next:
			if ( length == sizeof(bool) ) {
				if ( *(bool*)data) {
					removePanel( step1Panel );
					addPanel( step2Panel );
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

	addPanel( camerasDisplayPanel );
	addPanel( generalSettingsPanel );
	addPanel( devicesListPanel );
	addPanel( informationPanel );


	//!
	controls->forceUpdate( true );
	controls->activate( true );
}
//--------------------------------------------------------------
//! Add the panel and the controls of it
void MultiCams::addPanel( int id ) {
	ofxGuiPanel* pPanel;
	switch ( id ) {
		//! Cameras display panel
		case this->camerasDisplayPanel:
			pPanel = controls->addPanel(
				this->camerasDisplayPanel, "Cameras Display" , 30, 30,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->mObjWidth = 700;
			pPanel->mObjHeight = 360;

			break;
		//! General settings panel
		case generalSettingsPanel:
			pPanel = controls->addPanel(
				this->generalSettingsPanel, "General Settings", GENERAL_AREA_X,GENERAL_AREA_Y,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->addButton( this->generalSettingsPanel_start, "Start Settings",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->addButton( this->generalSettingsPanel_save, "Save",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->addButton( this->generalSettingsPanel_cancel, "Cancel",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->mObjWidth = GENERAL_AREA_WIDTH;
			pPanel->mObjHeight = GENERAL_AREA_HEIGHT;

			break;
		//! Devices list panel
		case devicesListPanel:
			pPanel = controls->addPanel(
				this->devicesListPanel, "Devices List", 30, 420,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->mObjWidth = 700;
			pPanel->mObjHeight = 150;

			break;
		//! Information Panel
		case informationPanel:
			pPanel = controls->addPanel(
				this->informationPanel, "Information Settings", 760, 220,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->mObjWidth = 160;
			pPanel->mObjHeight = 350;

			break;
		//! Step 1 Panel
		case step1Panel:
			pPanel = controls->addPanel(
				this->step1Panel, "Step 1", GENERAL_AREA_X, GENERAL_AREA_Y,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->addSlider( this->step1Panel_Xaxis, "X axis", 
				GENERAL_AREA_SLIDER_WIDTH, GENERAL_AREA_SLIDER_HEIGHT,
				1, 8, 1, kofxGui_Display_Int, 0 );
			pPanel->addSlider( this->step1Panel_Yaxis, "Y axis",
				GENERAL_AREA_SLIDER_WIDTH, GENERAL_AREA_SLIDER_HEIGHT,
				1, 8, 1, kofxGui_Display_Int, 0 );
			pPanel->addButton( this->step1Panel_previous, "previous",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->addButton( this->step1Panel_next, "next",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->mObjWidth = GENERAL_AREA_WIDTH;
			pPanel->mObjHeight = GENERAL_AREA_HEIGHT;

			break;
		//! Step 2 panel
		case step2Panel:
			pPanel = controls->addPanel(
				this->step2Panel, "Step 2", GENERAL_AREA_X, GENERAL_AREA_Y,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->addButton( this->step2Panel_previous, "previous",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->addButton( this->step2Panel_previous, "next",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->mObjWidth = GENERAL_AREA_WIDTH;
			pPanel->mObjHeight = GENERAL_AREA_HEIGHT;
			break;

	}
}
//--------------------------------------------------------------

void MultiCams::removePanels() {
	controls->removePanel( this->camerasDisplayPanel );
	controls->removePanel( this->generalSettingsPanel );
	controls->removePanel( this->devicesListPanel );
	controls->removePanel( this->informationPanel );
}
//--------------------------------------------------------------

void MultiCams::removePanel( int id ) {
	controls->removePanel( id );
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
		ofGetHeight()/2/* - testFont.stringHeight( testStr )/2*/
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