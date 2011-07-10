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
	currentCamera = NULL;

	//! do not display info at beginning
	bShowInfo = false;

	//! init the camera index offset to zero
	mCamIndex = 0;

	//! init the devices index offset to zero
	mDevIndexOffset = 0;
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
	camGrid->update();
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
		case devicesListPanel_grid:
			if ( length == sizeof( int ) ) {
				this->mCamIndex = *(int*)data;
				this->camGrid->setOffset( mCamIndex );
				if ( this->camGrid->getFirstImage() != NULL ) {
					this->currentCamera = this->camGrid->getFirstImage()->getCamera();
				} else {
					this->currentCamera = NULL;
				}

				removePanel( informationPanel );
				if ( this->currentCamera != NULL && bShowInfo ) {
					addPanel( informationPanel );
				}
			}
			break;
		case devicesListPanel_arrow_up:
			if ( length == sizeof(bool) ) {
				if ( *(bool*)data) {
					devGrid->previous();
					mDevIndexOffset = devGrid->getIndexOffset();
				}
			}
			break;
		case devicesListPanel_arrow_down:
			if ( length == sizeof(bool) ) {
				if ( *(bool*)data ) {
					devGrid->next();
					mDevIndexOffset = devGrid->getIndexOffset();
				}
			}
			break;
		//////////////////////////////////
		// Camera Display Panel
		case cameraDisplayPanel_info:
			if ( length == sizeof(bool) ) {
				if ( *(bool*)data && !bShowInfo ) {	// info panel should not display
					//! Get the current camera information
					if ( this->camGrid->getFirstImage() != NULL ) {
						this->currentCamera = this->camGrid->getFirstImage()->getCamera();
					} else {
						this->currentCamera = NULL;
					}

					if ( this->currentCamera != NULL ) {
						this->currentCamera->PrintInfo();
						addPanel( informationPanel );
						this->bShowInfo = true;
					} else {
						//! turn off the [show info] button
						controls->update( cameraDisplayPanel_info, kofxGui_Set_Bool, &bShowInfo, sizeof( bool ) );
					}
				} else {
					removePanel( informationPanel );
					this->bShowInfo = false;
				}
			}
			break;
		//////////////////////////////////
		// Information Panel
		case informationPanel_hflip:
			if ( length == sizeof(bool) ) {
				if ( this->currentCamera != NULL ) {
					this->currentCamera->SetHFlip( *(bool*)data );
				}
			}
			break;
		case informationPanel_vflip:
			if ( length == sizeof( bool ) ) {
				if ( this->currentCamera != NULL ) {
					this->currentCamera->SetVFlip( *(bool*)data );
				}
			}
			break;
		case informationPanel_auto_gain:
			if ( length == sizeof( bool ) ) {
				if ( this->currentCamera != NULL ) {
					this->currentCamera->SetAutoGain( *(bool*)data );
				}
			}
			break;
		case informationPanel_gain:
			if ( length == sizeof( float ) ) {
				if ( this->currentCamera != NULL ) {
					this->currentCamera->SetGainValue( (int)*(float*)data );
				}
			}
			break;
		case informationPanel_auto_exposure:
			if ( length == sizeof( bool ) ) {
				if ( this->currentCamera != NULL ) {
					this->currentCamera->SetAutoExposure( *(bool*)data );
				}
			}
			break;
		case informationPanel_exposure:
			if ( length == sizeof( float ) ) {
				if ( this->currentCamera != NULL ) {
					this->currentCamera->SetExposure( (int)*(float*)data );
				}
			}
			break;
		case informationPanel_auto_whitebalance:
			if ( length == sizeof( bool ) ) {
				if ( this->currentCamera != NULL ) {
					this->currentCamera->SetAutoWhiteBalance( *(bool*)data );
				}
			}
			break;
		case informationPanel_whitebalance_red:
			if ( length == sizeof( float ) ) {
				if ( this->currentCamera != NULL ) {
					this->currentCamera->SetWhiteBalanceRed( (int)*(float*)data );
				}
			}
			break;
		case informationPanel_whitebalance_green:
			if ( length == sizeof( float ) ) {
				if ( this->currentCamera != NULL ) {
					this->currentCamera->SetWhiteBalanceGreen( (int)*(float*)data );
				}
			}
			break;
		case informationPanel_whitebalance_blue:
			if ( length == sizeof( float ) ) {
				if ( this->currentCamera != NULL ) {
					this->currentCamera->SetWhiteBalanceBlue( (int)*(float*)data );
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
	
	//! Set the devices index offset
	this->devGrid->setOffset( this->mDevIndexOffset );

	this->addPanel( cameraDisplayPanel );

	//! Set the camera index offset
	this->camGrid->setOffset( this->mCamIndex );
	
	if ( bShowInfo && currentCamera != NULL ) {
		this->addPanel( informationPanel );
	}

	this->addPanel( settingsPanel );

	controls->forceUpdate( true );
	controls->activate( true );
}
//--------------------------------------------------------------
void SetDevices::addPanel( int id ) {
	ofxGuiPanel* pPanel;
	switch ( id ) {
		//////////////////////////////////
		case cameraDisplayPanel:
			pPanel = controls->addPanel( cameraDisplayPanel,
				"Camera Display", 30, 30,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			camGrid = (ofxGuiGrid*)pPanel->addGrid( cameraDisplayPanel_grid, "",
				640, 480, 1, 1, 0, 0, kofxGui_Grid_List );
			camGrid->setCamsUtils( utils );
			camGrid->setActive( false );	//! Disable the mouse event for this control
			camGrid->setDrawInfo( true );
			pPanel->addButton( cameraDisplayPanel_info, 
				"Show Info", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				bShowInfo, kofxGui_Button_Switch );

			//pPanel->mObjects[1]->mObjX = 380;	// [1]: "Show Info" button
			//pPanel->mObjects[1]->mObjY = 510;

			pPanel->mObjWidth = 660;
			pPanel->mObjHeight = 540;

			pPanel->adjustToNewContent( 640, 0 );

			break;

		//////////////////////////////////
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


		//////////////////////////////////
		case informationPanel:
			pPanel = controls->addPanel( informationPanel,
				"Information", 350, 200,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->addLabel( informationPanel_uuid, "UUID", 260, 10,
				"UUID: " + (this->currentCamera == NULL ? "NONE!" : PS3::GUID2String( this->currentCamera->GetGUID(), '_', true ) ),
				&(controls->mGlobals->mParamFont), controls->mGlobals->mTextColor );
			//! For this moment, the type is hard-code with "PS3"
			pPanel->addLabel( informationPanel_type, "Type", 260, 10,
				"Type: PS3", &(controls->mGlobals->mParamFont), controls->mGlobals->mTextColor );
			pPanel->addButton( informationPanel_hflip, "Horizontal Flip",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				this->currentCamera->GetHFlip(), kofxGui_Button_Switch );
			pPanel->addButton( informationPanel_vflip, "Vertical Flip",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				this->currentCamera->GetVFlip(), kofxGui_Button_Switch );
			//! Gain
			pPanel->addButton( informationPanel_auto_gain, "Auto Gain",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				this->currentCamera->GetAutoGain(), kofxGui_Button_Switch );
			pPanel->addSlider( informationPanel_gain, "Gain",
				150, 10, 0.0f, 79.0f,
				this->currentCamera->GetGainValue(), kofxGui_Display_Int, 0 );

			//! Exposure
			pPanel->addButton( informationPanel_auto_exposure, "Auto Exposure",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				this->currentCamera->GetAutoExposure(), kofxGui_Button_Switch );
			pPanel->addSlider( informationPanel_exposure, "Exposure",
				150, 10, 0.0f, 511.0f,
				this->currentCamera->GetExposure(), kofxGui_Display_Int, 0 );

			//! White balance
			pPanel->addButton( informationPanel_auto_whitebalance, "Auto Whitebalance",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				this->currentCamera->GetAutoWhiteBalance(), kofxGui_Button_Switch );
			pPanel->addSlider( informationPanel_whitebalance_red, "Whitebalance Red",
				150, 10, 0.0f, 255.0f,
				this->currentCamera->GetWhiteBalanceRed(), kofxGui_Display_Int, 0 );
			pPanel->addSlider( informationPanel_whitebalance_green, "Whitebalance Green",
				150, 10, 0.0f, 255.0f,
				this->currentCamera->GetWhiteBalanceGreen(), kofxGui_Display_Int, 0 );
			pPanel->addSlider( informationPanel_whitebalance_blue, "Whitebalance Blue",
				150, 10, 0.0f, 255.0f,
				this->currentCamera->GetWhiteBalanceBlue(), kofxGui_Display_Int, 0 );


			pPanel->mObjWidth = 270;
			pPanel->mObjHeight = 360;

			break;


		//////////////////////////////////
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