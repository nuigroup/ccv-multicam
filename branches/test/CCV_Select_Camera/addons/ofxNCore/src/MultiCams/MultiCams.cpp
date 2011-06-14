//! MultiCams.cpp
/*!
*  
*
*  Created by Yishi Guo on 05/31/2011.
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
	//ofAddListener( ofEvents.mousePressed, this, &MultiCams::_mousePressed );
	//ofAddListener( ofEvents.mouseDragged, this, &MultiCams::_mouseDragged );
	//ofAddListener( ofEvents.mouseReleased, this, &MultiCams::_mouseReleased );

	ofAddListener( ofEvents.keyPressed, this, &MultiCams::_keyPressed );
	ofAddListener( ofEvents.keyReleased, this, &MultiCams::_keyReleased );

	XAxis = 1;
	YAxis = 1;

	//////////////////////////
	// SetDevices
	setDevices = NULL;
	bDevicesConfiguration = false;

	//////////////////////////
	// Cams Utils
	utils = NULL;
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

	//! SetDevices
	if (setDevices == NULL ) {
		setDevices = new SetDevices();
		setDevices->setup();
	}
}
//--------------------------------------------------------------

void MultiCams::update( ofEventArgs &e ) {
	utils->update();
	devGrid->update();
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
	if ( bDevicesConfiguration ) {
		setDevices->handleGui( parameterId, task, data, length );
	} else {
		_handleGui( parameterId, task, data, length );
	}
}
//--------------------------------------------------------------
void MultiCams::_handleGui( int parameterId, int task, void* data, int length ) {
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
				//! X axis camera number
			case step1Panel_Xaxis:
				if( length == sizeof(float) ) {
					XAxis = *(float*)data;
					_setXY( XAxis, YAxis );
					printf( "XAxis: %f\n", *(float*)data );
				}
				break;
				//! Y axis camera number
			case step1Panel_Yaxis:
				if ( length == sizeof(float) ) {
					YAxis = *(float*)data;
					_setXY( XAxis, YAxis );
				}
				break;
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

				////////////////////////////////////////
				// STEP 2
				//! Set Devices
			case step2Panel_setDevices:
				if ( length == sizeof( bool ) ) {
					if (*(bool*)data) {
						switchSetDevicesGUI( true );
					}
				}
				break;
				//! Previous
			case step2Panel_previous:
				if ( length == sizeof(bool) ) {
					if (*(bool*)data) {
						removePanel( step2Panel );
						addPanel( step1Panel );
					}
				}
				break;
				//! Next - Go to step 3
			case step2Panel_next:
				if ( length == sizeof(bool) ) {
					if (*(bool*)data) {
						removePanel( step2Panel );
						addPanel( step3Panel );
					}
				}
				break;
				////////////////////////////////////////
				// STEP 3
				//! Previous step
			case step3Panel_previous:
				if ( length == sizeof( bool ) ) {
					if (*(bool*)data) {
						removePanel( step3Panel );
						addPanel( step2Panel );
					}
				}
				break;
				//! Next - Go to step 4
			case step3Panel_next:
				if ( length == sizeof( bool ) ) {
					if (*(bool*)data) {
						removePanel( step3Panel );
						addPanel( step4Panel );
					}
				}
				break;
				///////////////////////////////////////
				// STEP 4
				//! Previous step
			case step4Panel_previous:
				if ( length == sizeof( bool ) ) {
					if (*(bool*)data) {
						removePanel( step4Panel );
						addPanel( step3Panel );
					}
				}
				break;
				//! Return to general settings panel
			case step4Panel_finish:
				if ( length == sizeof( bool ) ) {
					if (*(bool*)data) {
						removePanel( step4Panel );
						addPanel( generalSettingsPanel );
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

	//! label color
	controls->mGlobals->mLabelColor.r = 0;
	controls->mGlobals->mLabelColor.g = 0;
	controls->mGlobals->mLabelColor.b = .2;
	controls->mGlobals->mLabelColor.a = .8;
}
//--------------------------------------------------------------

void MultiCams::updateControls() {
	controls->update( this->step1Panel_Xaxis, kofxGui_Set_Bool, &this->XAxis, sizeof(float) );
	controls->update( this->step1Panel_Yaxis, kofxGui_Set_Bool, &this->YAxis, sizeof(float) );
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
			camsGrid = (ofxGuiGrid*)pPanel->addGrid( camerasDisplayPanel_grid, "",
				680, 320, XAxis, YAxis,
				10, 5, kofxGui_Grid_Display );
			pPanel->mObjWidth = 700;
			pPanel->mObjHeight = 360;

			pPanel->adjustToNewContent( 600, 0 );

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

			//pPanel->mObjects[1]->mObjX = 10;	// [0]: "Save" button
			//pPanel->mObjects[1]->mObjY = 140;
			//pPanel->mObjects[2]->mObjX = 100;	// [1]: "Cancel" button
			//pPanel->mObjects[2]->mObjY = 140;

			pPanel->adjustToNewContent( 100, 0 );

			break;
		//! Devices list panel
		case devicesListPanel:
			pPanel = controls->addPanel(
				this->devicesListPanel, "Devices List", 30, 420,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			devGrid = (ofxGuiGrid*)pPanel->addGrid( devicesListPanel_grid, "", 553, 109, 4, 1, 5, 5, kofxGui_Grid_List );
			devGrid->setCamsUtils( utils );
			pPanel->addArrow( devicesListPanel_arrow_left, "", 53, 109, kofxGui_Arrow_Left );
			pPanel->addArrow( devicesListPanel_arrow_right, "", 50, 109, kofxGui_Arrow_Right );

			pPanel->mObjWidth = 700;
			pPanel->mObjHeight = 150;

			pPanel->mObjects[0]->mObjX = 73;	//! [0]: devices grid
			//pPanel->mObjects[0]->mObjY = 475;
			pPanel->mObjects[1]->mObjX = 10;		//! [1]: left arrow
			pPanel->mObjects[1]->mObjY = 32;
			pPanel->mObjects[2]->mObjX = 637;	//! [2]: right arrow
			pPanel->mObjects[2]->mObjY = 32;


			pPanel->adjustToNewContent( 600, 0 );

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
			pPanel->addLabel( step1Panel_tip, "",
				GENERAL_AREA_LABEL_WIDTH, GENERAL_AREA_LABEL_HEIGHT,
				"Set matrix:", controls->mGlobals->mLabelColor );
			pPanel->addSlider( this->step1Panel_Xaxis, "X axis", 
				GENERAL_AREA_SLIDER_WIDTH, GENERAL_AREA_SLIDER_HEIGHT,
				1, 8, XAxis, kofxGui_Display_Int, 0 );
			pPanel->addSlider( this->step1Panel_Yaxis, "Y axis",
				GENERAL_AREA_SLIDER_WIDTH, GENERAL_AREA_SLIDER_HEIGHT,
				1, 8, YAxis, kofxGui_Display_Int, 0 );
			pPanel->addButton( this->step1Panel_previous, "previous",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->addButton( this->step1Panel_next, "next",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->mObjWidth = GENERAL_AREA_WIDTH;
			pPanel->mObjHeight = GENERAL_AREA_HEIGHT;

			pPanel->mObjects[3]->mObjX = GENERAL_AREA_PREV_X;	// [3]: "previous"
			pPanel->mObjects[3]->mObjY = GENERAL_AREA_PREV_Y;
			pPanel->mObjects[4]->mObjX = GENERAL_AREA_NEXT_X; // [4]: "next"
			pPanel->mObjects[4]->mObjY = GENERAL_AREA_NEXT_Y;

			pPanel->adjustToNewContent( 100, 0 );

			break;
		//! Step 2 panel
		case step2Panel:
			pPanel = controls->addPanel(
				this->step2Panel, "Step 2", GENERAL_AREA_X, GENERAL_AREA_Y,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->addLabel( this->step2Panel_tip, "",
				GENERAL_AREA_LABEL_WIDTH, GENERAL_AREA_LABEL_HEIGHT,
				"Set Devices (optional)", controls->mGlobals->mLabelColor );
			pPanel->addButton( this->step2Panel_setDevices, "Set Devices",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->addButton( this->step2Panel_previous, "previous",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->addButton( this->step2Panel_next, "next",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );

			pPanel->mObjWidth = GENERAL_AREA_WIDTH;
			pPanel->mObjHeight = GENERAL_AREA_HEIGHT;

			pPanel->mObjects[2]->mObjX = GENERAL_AREA_PREV_X;	// [2]: "previous"
			pPanel->mObjects[2]->mObjY = GENERAL_AREA_PREV_Y;
			pPanel->mObjects[3]->mObjX = GENERAL_AREA_NEXT_X;	// [3]: "next"
			pPanel->mObjects[3]->mObjY = GENERAL_AREA_NEXT_Y;

			pPanel->adjustToNewContent( 100, 0 );

			break;
		//! Step 3 panel
		case step3Panel:
			pPanel = controls->addPanel(
				this->step3Panel, "Step 3", GENERAL_AREA_X, GENERAL_AREA_Y,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->addLabel( this->step3Panel_tip, "",
				GENERAL_AREA_LABEL_WIDTH, GENERAL_AREA_LABEL_HEIGHT,
				"Arrange the cameras", controls->mGlobals->mLabelColor );
			pPanel->addMatrix( this->step3Panel_matrix, "Thumbnails",
				GENERAL_AREA_MATRIX_WIDTH, GENERAL_AREA_MATRIX_HEIGHT,
				XAxis, YAxis, kofxGui_Matrix_Clear, kofxGui_Button_Trigger, OFXGUI_MATRIX_SPACING );
			pPanel->addButton( this->step3Panel_previous,"Previous",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->addButton( this->step3Panel_next, "Next",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->mObjWidth = GENERAL_AREA_WIDTH;
			pPanel->mObjHeight = GENERAL_AREA_HEIGHT;

			pPanel->mObjects[1]->mObjX = 10;	//! [1]: Matrix
			pPanel->mObjects[1]->mObjY = 45;
			pPanel->mObjects[2]->mObjX = GENERAL_AREA_PREV_X;	//! [2]: Previous
			pPanel->mObjects[2]->mObjY = GENERAL_AREA_PREV_Y;
			pPanel->mObjects[3]->mObjX = GENERAL_AREA_NEXT_X;	//! [3]: Next
			pPanel->mObjects[3]->mObjY = GENERAL_AREA_NEXT_Y;

			pPanel->adjustToNewContent( 140, 0 );

			break;

		case step4Panel:
			pPanel = controls->addPanel(
				this->step4Panel, "Step 4", GENERAL_AREA_X, GENERAL_AREA_Y,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->addLabel( this->step4Panel_tip, "",
				GENERAL_AREA_LABEL_WIDTH, GENERAL_AREA_LABEL_HEIGHT,
				"Finishing (optional)", controls->mGlobals->mLabelColor );
			pPanel->addButton( this->step4Panel_calibration, "Calibration",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->addButton( this->step4Panel_previous, "Previous",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->addButton( this->step4Panel_finish, "Finish",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );

			pPanel->mObjWidth = GENERAL_AREA_WIDTH;
			pPanel->mObjHeight = GENERAL_AREA_HEIGHT;

			pPanel->mObjects[2]->mObjX = GENERAL_AREA_PREV_X;
			pPanel->mObjects[2]->mObjY = GENERAL_AREA_PREV_Y;
			pPanel->mObjects[3]->mObjX = GENERAL_AREA_NEXT_X;
			pPanel->mObjects[3]->mObjY = GENERAL_AREA_NEXT_Y;

			pPanel->adjustToNewContent( 100, 0 );
			break;

		default:
			break;

	}
}
//--------------------------------------------------------------

void MultiCams::removePanels() {
	controls->removePanel( this->camerasDisplayPanel );
	controls->removePanel( this->generalSettingsPanel );
	controls->removePanel( this->devicesListPanel );
	controls->removePanel( this->informationPanel );

	controls->removePanel( this->step1Panel );
	controls->removePanel( this->step2Panel );
	controls->removePanel( this->step3Panel );
	controls->removePanel( this->step4Panel );
}
//--------------------------------------------------------------

void MultiCams::removePanel( int id ) {
	controls->removePanel( id );
}
//--------------------------------------------------------------

/*******************************************************
*      Draw methods
********************************************************/
void MultiCams::draw() {
	//! Move this to "update" function
	if ( bDevicesConfiguration && setDevices->bShowInterface == false ) {
		switchSetDevicesGUI( false );	//! Close the SetDevices, show MultiCams
	}
	if ( bDevicesConfiguration ) {
		setDevices->draw();
	} else {
		_draw();	//! Draw the self interface
	}

}
//--------------------------------------------------------------

void MultiCams::_draw() {
	ofSetColor( 0x123456 );
	ofFill();
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );

	//string testStr = ofToString( testInt++ );

	//// Set the font color to white.
	//ofSetColor( 0xFFFFFF );

	////! Draw the test string
	//testFont.drawString(
	//	testStr,
	//	ofGetWidth()/2 - testFont.stringWidth( testStr )/2,
	//	ofGetHeight()/2/* - testFont.stringHeight( testStr )/2*/
	//);

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

void MultiCams::passInCamsUtils( CamsUtils* utils ) {
	this->utils = utils;
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

void MultiCams::switchSetDevicesGUI( bool showDevices ) {
	if ( setDevices == NULL ) {
		return;
	}
	if ( showDevices ) {
		removePanels();
		setDevices->showInterface( true );
		bDevicesConfiguration = true;
	} else {
		setDevices->showInterface( false );
		addPanels();
		//! Goto step 2
		removePanel( generalSettingsPanel );
		addPanel( step2Panel );
		bDevicesConfiguration = false;
	}
}
//--------------------------------------------------------------

void MultiCams::_setXY( int x, int y ) {
	if ( camsGrid != NULL ) {
		camsGrid->setXY( XAxis, YAxis );
	}
	if ( utils != NULL ) {
		utils->setXY( XAxis, YAxis );
	}
}

//--------------------------------------------------------------