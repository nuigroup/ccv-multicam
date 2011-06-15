//! SetDevices.cpp
/*!
*  
*
*  Created by Yishi Guo on 05/31/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

//--------------------------------------------------------------
#include "SetDevices.h"

//--------------------------------------------------------------
SetDevices::SetDevices() {
	bShowInterface = false;
	utils = NULL;
}

//--------------------------------------------------------------
void SetDevices::setup() {
	testInt = 0;
	testFont.loadFont( "verdana.ttf", 200, true, true, false );

	controls = ofxGui::Instance( this );

	setupControls();
}

//--------------------------------------------------------------

void SetDevices::update() {
	devGrid->update();
}

//--------------------------------------------------------------

void SetDevices::passInCamsUtils( CamsUtils* utils ) {
	this->utils = utils;
}

//--------------------------------------------------------------
void SetDevices::handleGui( int parameterId, int task, void* data, int length ) {
	switch ( parameterId ) {
		//////////////////////////////////
		// Devices List Panel
		case devicesListPanel_arrow_up:
			if ( length == sizeof(bool) ) {
				if ( *(bool*)data) {
					devGrid->previous();
				}
			}
			break;
		case devicesListPanel_arrow_down:
			if ( length == sizeof(bool) ) {
				if ( *(bool*)data ) {
					devGrid->next();
				}
			}
			break;
		//////////////////////////////////
		// Camera Display Panel
		case cameraDisplayPanel_info:
			if ( length == sizeof(bool) ) {
				if ( *(bool*)data) {
					addPanel( informationPanel );
				} else {
					removePanel( informationPanel );
				}
			}
			break;
		//////////////////////////////////
		// Settings Panel
		case settingsPanel_reset:
			if ( length == sizeof(bool) ) {
				if ( *(bool*)data ) {
					// TODO
				}
			}
			break;
		case settingsPanel_save:
			if ( length == sizeof(bool) ) {
				if ( *(bool*)data ) {
					showInterface( false );
				}
			}
			break;
		case settingsPanel_cancel:
			if ( length == sizeof(bool) ) {
				if ( *(bool*)data ) {
					showInterface( false );
				}
			}
			break;
		default:
			break;
	}
}
//--------------------------------------------------------------
void SetDevices::setupControls() {
	// TODO
}
//--------------------------------------------------------------
void SetDevices::draw() {
	ofSetColor( 0x33aa33 );
	ofFill();
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );

	//ofSetColor( 0xFFFFFF );
	//testInt++;
	//string testStr = ofToString( testInt );
	//testFont.drawString( testStr,
	//	ofGetWidth()/2 - testFont.stringWidth(testStr)/2,
	//	ofGetHeight()/2 + testFont.stringHeight(testStr)/2 );

	controls->draw();
}

//--------------------------------------------------------------
void SetDevices::addPanels() {
	// TODO
	this->addPanel( devicesListPanel );
	this->addPanel( cameraDisplayPanel );
	// this->addPanel( informationPanel );
	this->addPanel( settingsPanel );

	controls->forceUpdate( true );
	controls->activate( true );
}
//--------------------------------------------------------------
void SetDevices::addPanel( int id ) {
	ofxGuiPanel* pPanel;
	switch ( id ) {
		case cameraDisplayPanel:
			pPanel = controls->addPanel( cameraDisplayPanel,
				"Camera Display", 30, 30,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->addGrid( cameraDisplayPanel_grid, "",
				640, 480, 1, 1, 0, 0, kofxGui_Grid_List );
			pPanel->addButton( cameraDisplayPanel_info, 
				"Show Info", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Switch );

			//pPanel->mObjects[1]->mObjX = 380;	// [1]: "Show Info" button
			//pPanel->mObjects[1]->mObjY = 510;

			pPanel->mObjWidth = 660;
			pPanel->mObjHeight = 540;

			pPanel->adjustToNewContent( 640, 0 );

			break;

		case devicesListPanel:
			pPanel = controls->addPanel( devicesListPanel,
				"Devices List", 720, 30,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->addArrow( devicesListPanel_arrow_up, "",
				180, 35, kofxGui_Arrow_Up, 5 );
			devGrid = (ofxGuiGrid*)pPanel->addGrid( devicesListPanel_grid, "",
				180, 270, 1, 2, 10, 10, kofxGui_Grid_List );
			devGrid->setCamsUtils( utils );
			pPanel->addArrow( devicesListPanel_arrow_down, "",
				180, 35, kofxGui_Arrow_Down, 5 );
			pPanel->mObjWidth = 200;
			pPanel->mObjHeight = 400;
			break;

		case informationPanel:
			pPanel = controls->addPanel( informationPanel,
				"Information", 400, 240,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->mObjWidth = 240;
			pPanel->mObjHeight = 320;

			break;

		case settingsPanel:
			pPanel = controls->addPanel( settingsPanel,
				"Settings", 720, 460,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->addButton( settingsPanel_reset,
				"Reset All", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->addButton( settingsPanel_save,
				"Save", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->addButton( settingsPanel_cancel,
				"Cancel", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->mObjWidth = 200;
			pPanel->mObjHeight = 110;
			break;
		default:
			break;
	}
}
//--------------------------------------------------------------
void SetDevices::removePanels() {
	removePanel( cameraDisplayPanel );
	removePanel( devicesListPanel );
	removePanel( informationPanel );
	removePanel( settingsPanel );
}
//--------------------------------------------------------------
void SetDevices::removePanel( int id ) {
	controls->removePanel( id );
}
//--------------------------------------------------------------
void SetDevices::showInterface( bool bShow ) {
	if ( bShow ) {
		bShowInterface = true;
		addPanels();
	} else {
		bShowInterface = false;
		removePanels();
	}
}
//--------------------------------------------------------------