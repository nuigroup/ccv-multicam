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
	ofAddListener( ofEvents.mousePressed, this, &MultiCams::_mousePressed );
	ofAddListener( ofEvents.mouseDragged, this, &MultiCams::_mouseDragged );
	ofAddListener( ofEvents.mouseReleased, this, &MultiCams::_mouseReleased );

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

	bDraggingImage = false;
	draggingImage = new ofxGuiImage();

	// set the value explicit
	devGrid = NULL;
	camsGrid = NULL;

	currentCamera = new PS3();
	previewImage = new ofxGuiImage();
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
		setDevices->passInCamsUtils( utils );
		setDevices->setup();
	}

	//! init the X/Y
	XAxis = utils->getXGrid();
	YAxis = utils->getYGrid();
}
//--------------------------------------------------------------

void MultiCams::update( ofEventArgs &e ) {
	utils->update();
	if ( bDevicesConfiguration ) {
		setDevices->update();
	} else { 
		devGrid->update();
	}
}

//--------------------------------------------------------------


void MultiCams::_mousePressed( ofMouseEventArgs &e ) {
	// TODO
}

//--------------------------------------------------------------

void MultiCams::_mouseDragged( ofMouseEventArgs &e ) {
	//! Ref: http://www.youtube.com/watch?v=EZEWtmJ7WKg
	
	//! Hard mode solution
	//if ( bDraggingImage ) {
	//	e.x = e.x < 0 ? 0 : e.x;
	//	e.y = e.y < 0 ? 0 : e.y;
	//	draggingXOffset = e.x < draggingXOffset ? e.x : draggingXOffset;
	//	draggingYOffset = e.y < draggingYOffset ? e.y : draggingYOffset;

	//	draggingImage->setXYPos( e.x - draggingXOffset , e.y - draggingYOffset );
	//}

	//! Soft mode solution
	if ( bDraggingImage ) {
		draggingImage->setXYPos(
			e.x - draggingXOffset < 0 ? 0 : e.x - draggingXOffset,
			e.y - draggingYOffset < 0 ? 0 : e.y - draggingYOffset );
	}
}

//--------------------------------------------------------------

void MultiCams::_mouseReleased( ofMouseEventArgs &e ) {
	if ( bDraggingImage ) {
		this->bDraggingImage = false;
	}
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
			////////////////////////////////
			// CAMERA DISPLAY
			//
		case camerasDisplayPanel_grid:
			if ( length == sizeof(int) ) {
				if ( task == kofxGui_Set_Grid_Released ) {
					//! Refresh the grid control
					devGrid->setImages();
				} else if ( task == kofxGui_Set_Int ) {
					updateInfoPanel( *(int*)data );
				}
			}
			break;
		case camerasDisplayPanel_grid_reset:
			if ( length == sizeof( bool ) && task == kofxGui_Set_Bool ) {
				if ( *(bool*)data ) {
					utils->setSelected( utils->getRawId( utils->getCam( camsGrid->getSelectedId() ) ), true );
					utils->setCam( camsGrid->getSelectedId(), NULL );
					devGrid->setImages();
					camsGrid->setImages();
					camsGrid->removeControls();
				}
			}
			break;
		case camerasDisplayPanel_grid_setting:
			if ( length == sizeof( bool ) && task == kofxGui_Set_Bool ) {
				if ( *(bool*)data ) {
					// TODO
					setDevices->setShowCamRawId( utils->getRawId( utils->getCam( camsGrid->getSelectedId() ) ) );

					switchSetDevicesGUI( true );
				}
			}
			//////////////////////////////////
			// DEVICES LIST
			//! 
		case devicesListPanel_grid:
			if ( length == sizeof(int) ) {
				if ( task == kofxGui_Set_Grid_Dragging ) {
					int index = *(int*)data;
					//! the camera index must less than camera count
					if ( index < utils->getCount() ) {
						if ( bDraggingImage == false ) {
							bDraggingImage = true;
							draggingImage->init( dragging_image, "", 100, 100, 
								devGrid->getGridWidth(), devGrid->getGridHeight() );

							draggingImage->setCamera( this->utils->getRawCam( index ) );

							//! Set the X/Y coordinate offset
							draggingXOffset = devGrid->getDraggingXOffset();
							draggingYOffset = devGrid->getDraggingYOffset();

							//! Tell camsGrid the dragging index
							camsGrid->setDraggingRawIndex( index );
						}
					} else {
						bDraggingImage = false;
					}
				} else if ( task == kofxGui_Set_Int ) {
					updateInfoPanel( *(int*)data );
				}
			}
			break;
			//! Show previous camera.
		case devicesListPanel_arrow_left:
			if ( length == sizeof(bool) ) {
				if (*(bool*)data) {
					devGrid->previous();
				}
			}
			break;

			//! Show next camera.
		case devicesListPanel_arrow_right:
			if ( length == sizeof(bool) ) {
				if (*(bool*)data) {
					devGrid->next();
				}
			}
			break;

			///////////////////////
			// GRID SETTINGS
			//
		case gridSettingsPanel_x:
			if( length == sizeof(float) ) {
				//! Modify the value just when that are not equal
				if ( XAxis != (int)*(float*)data ) {
					XAxis = *(float*)data;
					_setXY( XAxis, YAxis );
				}
				//printf( "XAxis: %f\n", *(float*)data );
			}
			break;
			//! Y axis camera number
		case gridSettingsPanel_y:
			if ( length == sizeof(float) ) {
				if ( YAxis != (int)*(float*)data ) {
					YAxis = *(float*)data;
					_setXY( XAxis, YAxis );
				}
			}
			break;

			////////////////////////////////////
			// DEVICES SETTINGS
			//
		case devicesSettingsPanel_set:
			if ( length == sizeof( bool ) ) {
				if (*(bool*)data) {
					switchSetDevicesGUI( true );
				}
			}
			break;

			////////////////////////////////////
			// GENERAL SETTINGS

			//! Reset All
		case generalSettingsPanel_reset_all:
			if ( length == sizeof( bool ) ) {
				if ( *(bool*)data ) {
					if ( utils != NULL ) {
						utils->resetAll();
					}
					//! Refresh the images
					if ( camsGrid != NULL ) {
						camsGrid->resetAll();
					}

					//! Refresh the devices list
					if ( devGrid != NULL ) {
						devGrid->setImages();
					}
				}
			}
			break;
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
					// TODO check the cameras count
					if ( utils != NULL ) {
						utils->saveXML();
					}
				}
			}
			break;
			//! Exit (Not settings saved)
		case generalSettingsPanel_exit:
			if ( length == sizeof(bool) ) {
				if (*(bool*)data) {
					if ( _coreVision != NULL ) {
						_coreVision->switchMultiCamsGUI( false );
					}
				}
			}
			break;
			/////////////////////////////////////
			// INFORMATION PANEL
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

					if ( !*(bool*)data ) {	// use the manual value
						this->currentCamera->SetGainValue( currentCamera->GetGainValue() );
					}
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

					if ( !*(bool*)data ) {	// use the manual value
						this->currentCamera->SetExposure( currentCamera->GetExposure() );
					}
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

					if ( !*(bool*)data ) {
						this->currentCamera->SetWhiteBalanceRed( currentCamera->GetWhiteBalanceRed() );
						this->currentCamera->SetWhiteBalanceGreen( currentCamera->GetWhiteBalanceGreen() );
						this->currentCamera->SetWhiteBalanceBlue( currentCamera->GetWhiteBalanceBlue() );
					}
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
			// STEPS PANEL REMOVED 07/21/2011
		default:
			break;
	}
}
//--------------------------------------------------------------
void MultiCams::setupControls() {

	////panel border color
	//controls->mGlobals->mBorderColor.r = 0;
	//controls->mGlobals->mBorderColor.g = 0;
	//controls->mGlobals->mBorderColor.b = 0;
	//controls->mGlobals->mBorderColor.a = .3;
	////panel color
	//controls->mGlobals->mCoverColor.r = 1;
	//controls->mGlobals->mCoverColor.g = 1;
	//controls->mGlobals->mCoverColor.b = 1;
	//controls->mGlobals->mCoverColor.a = .4;
	////control outline color
	//controls->mGlobals->mFrameColor.r = 0;
	//controls->mGlobals->mFrameColor.g = 0;
	//controls->mGlobals->mFrameColor.b = 0;
	//controls->mGlobals->mFrameColor.a = .3;
	////text color
	//controls->mGlobals->mTextColor.r = 0;
	//controls->mGlobals->mTextColor.g = 0;
	//controls->mGlobals->mTextColor.b = 0;
	//controls->mGlobals->mTextColor.a = 1;
	////button color
	//controls->mGlobals->mButtonColor.r = 1;
	//controls->mGlobals->mButtonColor.g = 0;
	//controls->mGlobals->mButtonColor.b = 0;
	//controls->mGlobals->mButtonColor.a = .8;
	////slider tip color
	//controls->mGlobals->mHandleColor.r = 0;
	//controls->mGlobals->mHandleColor.g = 0;
	//controls->mGlobals->mHandleColor.b = 0;
	////slider color
	//controls->mGlobals->mSliderColor.r = 1;
	//controls->mGlobals->mSliderColor.g = 0;
	//controls->mGlobals->mSliderColor.b = 0;
	//controls->mGlobals->mSliderColor.a = .8;

	////! label color
	//controls->mGlobals->mLabelColor.r = 0;
	//controls->mGlobals->mLabelColor.g = 0;
	//controls->mGlobals->mLabelColor.b = .2;
	//controls->mGlobals->mLabelColor.a = .8;
}
//--------------------------------------------------------------

void MultiCams::updateControls() {
	controls->update( this->step1Panel_Xaxis, kofxGui_Set_Bool, &this->XAxis, sizeof(float) );
	controls->update( this->step1Panel_Yaxis, kofxGui_Set_Bool, &this->YAxis, sizeof(float) );
}
//--------------------------------------------------------------

void MultiCams::addPanels() {

	addPanel( camerasDisplayPanel );
	addPanel( devicesListPanel );
	addPanel( gridSettingsPanel );
	//addPanel( devicesSettingsPanel );
	addPanel( calibrationPanel );
	addPanel( generalSettingsPanel );

	addPanel( informationPanel );


	//! Update the grid controls
	if ( camsGrid != NULL ) {
		camsGrid->setImages();
	}
	if ( devGrid != NULL ) {
		devGrid->setImages();
	}

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
				this->camerasDisplayPanel, "Cameras Display" , 20, 20,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			camsGrid = (ofxGuiGrid*)pPanel->addGrid( camerasDisplayPanel_grid, "",
				653, 350, XAxis, YAxis,	// 515->680
				10, 5, kofxGui_Grid_Display );
			camsGrid->setCamsUtils( utils );
			camsGrid->setResetBtnId( camerasDisplayPanel_grid_reset );
			camsGrid->setSettingBtnId( camerasDisplayPanel_grid_setting );
			camsGrid->setShowResetBtn( true );
			camsGrid->setShowSettingBtn( false );
			camsGrid->enableDblClickMode( true );

			//pPanel->addButton( step3Panel_reset_all, "Reset All",
			//	OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->mObjWidth = 673;	//535->700
			pPanel->mObjHeight = 390;

			pPanel->adjustToNewContent( 500, 0 );

			break;

		//! Devices list panel
		case devicesListPanel:
			pPanel = controls->addPanel(
				this->devicesListPanel, "Devices List", 20, 430,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			devGrid = (ofxGuiGrid*)pPanel->addGrid( devicesListPanel_grid, "", 553, 109, 4, 1, 5, 5, kofxGui_Grid_List );
			devGrid->setCamsUtils( utils );
			devGrid->setMode( kofxGui_Grid_Selectable, true );
			devGrid->setDrawSelectedText( true );

			pPanel->addArrow( devicesListPanel_arrow_left, "", 40, 109, kofxGui_Arrow_Left );
			pPanel->addArrow( devicesListPanel_arrow_right, "", 40, 109, kofxGui_Arrow_Right );

			pPanel->mObjects[0]->mObjX = 60;	//! [0]: devices grid
			//pPanel->mObjects[0]->mObjY = 475;
			pPanel->mObjects[1]->mObjX = 10;		//! [1]: left arrow
			pPanel->mObjects[1]->mObjY = 32;
			pPanel->mObjects[2]->mObjX = 485+138;	//! [2]: right arrow
			pPanel->mObjects[2]->mObjY = 32;

			pPanel->mObjWidth = 673;	//535->680
			pPanel->mObjHeight = 150;

			pPanel->adjustToNewContent( 500, 0 );

			break;
		//! Grid Settings Panel
		case gridSettingsPanel:
			pPanel = controls->addPanel( gridSettingsPanel,
				"Grid Settings", RIGHT_PANEL_X, RIGHT_PANEL_Y_OFFSET,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->addSlider( gridSettingsPanel_x, "X Axis",
				RIGHT_PANEL_SLIDER_WIDTH, RIGHT_PANEL_SLIDER_HEIGHT,
				1, 8, XAxis, kofxGui_Display_Int, 0 );
			pPanel->addSlider( gridSettingsPanel_y, "Y Axis",
				RIGHT_PANEL_SLIDER_WIDTH, RIGHT_PANEL_SLIDER_HEIGHT,
				1, 8, YAxis, kofxGui_Display_Int, 0 );

			pPanel->mObjWidth = RIGHT_PANEL_WIDTH;
			pPanel->mObjHeight = 100;
			
			break;

		//! Devices Settings Panel
		case devicesSettingsPanel:
			pPanel = controls->addPanel( devicesSettingsPanel,
				"Devices Settings", RIGHT_PANEL_X, 130 + RIGHT_PANEL_Y_OFFSET,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->addButton( devicesSettingsPanel_set,
				"Set Devices", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger);

			pPanel->mObjWidth = RIGHT_PANEL_WIDTH;
			pPanel->mObjHeight = 50;

			break;

		//! Calibration Panel
		case calibrationPanel:
			pPanel = controls->addPanel( calibrationPanel,
				"Calibration", RIGHT_PANEL_X, RIGHT_PANEL_Y_OFFSET + 110,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			pPanel->addButton( calibrationPanel_enter,
				"Enter Calibration", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );

			pPanel->mObjWidth = RIGHT_PANEL_WIDTH;
			pPanel->mObjHeight = 50;

			break;

		//! General settings panel
		case generalSettingsPanel:
			pPanel = controls->addPanel(
				this->generalSettingsPanel, "General Settings",
				RIGHT_PANEL_X, 170 + RIGHT_PANEL_Y_OFFSET,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			//pPanel->addButton( this->generalSettingsPanel_start, "Start Settings",
			//	OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
			//	kofxGui_Button_Off, kofxGui_Button_Trigger );
			//! Alias
			pPanel->addButton( this->generalSettingsPanel_reset_all, "Reset All",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->addButton( this->generalSettingsPanel_save, "Save",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );
			pPanel->addButton( this->generalSettingsPanel_exit, "Back",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				kofxGui_Button_Off, kofxGui_Button_Trigger );

			pPanel->mObjWidth = RIGHT_PANEL_WIDTH;
			pPanel->mObjHeight = 90;

			//pPanel->mObjects[1]->mObjX = 10;	// [0]: "Save" button
			//pPanel->mObjects[1]->mObjY = 140;
			//pPanel->mObjects[2]->mObjX = 100;	// [1]: "Cancel" button
			//pPanel->mObjects[2]->mObjY = 140;

			//pPanel->adjustToNewContent( 100, 0 );

			break;

		//! Information Panel
		case informationPanel:
			pPanel = controls->addPanel(
				this->informationPanel, "Camera Settings",
				RIGHT_PANEL_X, 270 + RIGHT_PANEL_Y_OFFSET,
				OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
			//previewImage = (ofxGuiImage*)pPanel->addImage( informationPanel, "Present",
			//	RIGHT_PANEL_SLIDER_WIDTH,
			//	(RIGHT_PANEL_SLIDER_WIDTH / GRID_WIDTH_SCALE) * GRID_HEIGHT_SCALE );
			//previewImage->setCamera( currentCamera );
			//pPanel->addButton( informationPanel_hflip, "Horizontal Flip",
			//	OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
			//	this->currentCamera->GetHFlip(), kofxGui_Button_Switch );
			//pPanel->addButton( informationPanel_vflip, "Vertical Flip",
			//	OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
			//	this->currentCamera->GetVFlip(), kofxGui_Button_Switch );
			//! Gain
			pPanel->addButton( informationPanel_auto_gain, "Auto Gain",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				this->currentCamera->GetAutoGain(), kofxGui_Button_Switch );
			pPanel->addSlider( informationPanel_gain, "Gain",
				RIGHT_PANEL_SLIDER_WIDTH, RIGHT_PANEL_SLIDER_HEIGHT, 0.0f, 79.0f,
				this->currentCamera->GetGainValue(), kofxGui_Display_Int, 0 );

			//! Exposure
			pPanel->addButton( informationPanel_auto_exposure, "Auto Exposure",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				this->currentCamera->GetAutoExposure(), kofxGui_Button_Switch );
			pPanel->addSlider( informationPanel_exposure, "Exposure",
				RIGHT_PANEL_SLIDER_WIDTH, RIGHT_PANEL_SLIDER_HEIGHT, 0.0f, 511.0f,
				this->currentCamera->GetExposure(), kofxGui_Display_Int, 0 );

			//! White balance
			pPanel->addButton( informationPanel_auto_whitebalance, "Auto Whitebalance",
				OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
				this->currentCamera->GetAutoWhiteBalance(), kofxGui_Button_Switch );
			pPanel->addSlider( informationPanel_whitebalance_red, "Whitebalance Red",
				RIGHT_PANEL_SLIDER_WIDTH, RIGHT_PANEL_SLIDER_HEIGHT, 0.0f, 255.0f,
				this->currentCamera->GetWhiteBalanceRed(), kofxGui_Display_Int, 0 );
			pPanel->addSlider( informationPanel_whitebalance_green, "Whitebalance Green",
				RIGHT_PANEL_SLIDER_WIDTH, RIGHT_PANEL_SLIDER_HEIGHT, 0.0f, 255.0f,
				this->currentCamera->GetWhiteBalanceGreen(), kofxGui_Display_Int, 0 );
			pPanel->addSlider( informationPanel_whitebalance_blue, "Whitebalance Blue",
				RIGHT_PANEL_SLIDER_WIDTH, RIGHT_PANEL_SLIDER_HEIGHT, 0.0f, 255.0f,
				this->currentCamera->GetWhiteBalanceBlue(), kofxGui_Display_Int, 0 );


			pPanel->mObjWidth = RIGHT_PANEL_WIDTH;
			pPanel->mObjHeight = 290;

			break;
		////! Step 1 Panel
		//case step1Panel:
		//	pPanel = controls->addPanel(
		//		this->step1Panel, "Step 1", GENERAL_AREA_X, GENERAL_AREA_Y,
		//		OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
		//	pPanel->addLabel( step1Panel_tip, "",
		//		GENERAL_AREA_LABEL_WIDTH, GENERAL_AREA_LABEL_HEIGHT,
		//		"Set matrix:", controls->mGlobals->mLabelColor );
		//	pPanel->addSlider( this->step1Panel_Xaxis, "X axis", 
		//		GENERAL_AREA_SLIDER_WIDTH, GENERAL_AREA_SLIDER_HEIGHT,
		//		1, 8, XAxis, kofxGui_Display_Int, 0 );
		//	pPanel->addSlider( this->step1Panel_Yaxis, "Y axis",
		//		GENERAL_AREA_SLIDER_WIDTH, GENERAL_AREA_SLIDER_HEIGHT,
		//		1, 8, YAxis, kofxGui_Display_Int, 0 );
		//	pPanel->addButton( this->step1Panel_previous, "previous",
		//		OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
		//		kofxGui_Button_Off, kofxGui_Button_Trigger );
		//	pPanel->addButton( this->step1Panel_next, "next",
		//		OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
		//		kofxGui_Button_Off, kofxGui_Button_Trigger );
		//	pPanel->mObjWidth = GENERAL_AREA_WIDTH;
		//	pPanel->mObjHeight = GENERAL_AREA_HEIGHT;

		//	pPanel->mObjects[3]->mObjX = GENERAL_AREA_PREV_X;	// [3]: "previous"
		//	pPanel->mObjects[3]->mObjY = GENERAL_AREA_PREV_Y;
		//	pPanel->mObjects[4]->mObjX = GENERAL_AREA_NEXT_X; // [4]: "next"
		//	pPanel->mObjects[4]->mObjY = GENERAL_AREA_NEXT_Y;

		//	pPanel->adjustToNewContent( 100, 0 );

		//	break;
		////! Step 2 panel
		//case step2Panel:
		//	pPanel = controls->addPanel(
		//		this->step2Panel, "Step 2", GENERAL_AREA_X, GENERAL_AREA_Y,
		//		OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
		//	pPanel->addLabel( this->step2Panel_tip, "",
		//		GENERAL_AREA_LABEL_WIDTH, GENERAL_AREA_LABEL_HEIGHT,
		//		"Set Devices (optional)", controls->mGlobals->mLabelColor );
		//	pPanel->addButton( this->step2Panel_setDevices, "Set Devices",
		//		OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
		//		kofxGui_Button_Off, kofxGui_Button_Trigger );
		//	pPanel->addButton( this->step2Panel_previous, "previous",
		//		OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
		//		kofxGui_Button_Off, kofxGui_Button_Trigger );
		//	pPanel->addButton( this->step2Panel_next, "next",
		//		OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
		//		kofxGui_Button_Off, kofxGui_Button_Trigger );

		//	pPanel->mObjWidth = GENERAL_AREA_WIDTH;
		//	pPanel->mObjHeight = GENERAL_AREA_HEIGHT;

		//	pPanel->mObjects[2]->mObjX = GENERAL_AREA_PREV_X;	// [2]: "previous"
		//	pPanel->mObjects[2]->mObjY = GENERAL_AREA_PREV_Y;
		//	pPanel->mObjects[3]->mObjX = GENERAL_AREA_NEXT_X;	// [3]: "next"
		//	pPanel->mObjects[3]->mObjY = GENERAL_AREA_NEXT_Y;

		//	pPanel->adjustToNewContent( 100, 0 );

		//	break;
		////! Step 3 panel
		//case step3Panel:
		//	pPanel = controls->addPanel(
		//		this->step3Panel, "Step 3", GENERAL_AREA_X, GENERAL_AREA_Y,
		//		OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
		//	pPanel->addLabel( this->step3Panel_tip, "",
		//		GENERAL_AREA_LABEL_WIDTH, GENERAL_AREA_LABEL_HEIGHT,
		//		"Arrange the cameras", controls->mGlobals->mLabelColor );
		//	//pPanel->addMatrix( this->step3Panel_matrix, "Thumbnails",
		//	//	GENERAL_AREA_MATRIX_WIDTH, GENERAL_AREA_MATRIX_HEIGHT,
		//	//	XAxis, YAxis, kofxGui_Matrix_Clear, kofxGui_Button_Trigger, OFXGUI_MATRIX_SPACING );
		//	pPanel->addButton( this->step3Panel_reset_all, "Reset All",
		//		OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
		//		kofxGui_Button_Off, kofxGui_Button_Trigger );
		//	pPanel->addButton( this->step3Panel_previous,"Previous",
		//		OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
		//		kofxGui_Button_Off, kofxGui_Button_Trigger );
		//	pPanel->addButton( this->step3Panel_next, "Next",
		//		OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
		//		kofxGui_Button_Off, kofxGui_Button_Trigger );
		//	pPanel->mObjWidth = GENERAL_AREA_WIDTH;
		//	pPanel->mObjHeight = GENERAL_AREA_HEIGHT;

		//	//pPanel->mObjects[1]->mObjX = 10;	//! [1]: Matrix
		//	//pPanel->mObjects[1]->mObjY = 45;
		//	pPanel->mObjects[2]->mObjX = GENERAL_AREA_PREV_X;	//! [2]: Previous
		//	pPanel->mObjects[2]->mObjY = GENERAL_AREA_PREV_Y;
		//	pPanel->mObjects[3]->mObjX = GENERAL_AREA_NEXT_X;	//! [3]: Next
		//	pPanel->mObjects[3]->mObjY = GENERAL_AREA_NEXT_Y;

		//	//pPanel->mObjects[1]->mObjX = GENERAL_AREA_PREV_X;	//! [1]: Previous
		//	//pPanel->mObjects[1]->mObjY = GENERAL_AREA_PREV_Y;
		//	//pPanel->mObjects[2]->mObjX = GENERAL_AREA_NEXT_X;	//! [2]: Next
		//	//pPanel->mObjects[2]->mObjY = GENERAL_AREA_NEXT_Y;

		//	pPanel->adjustToNewContent( 140, 0 );

		//	break;

		//case step4Panel:
		//	pPanel = controls->addPanel(
		//		this->step4Panel, "Step 4", GENERAL_AREA_X, GENERAL_AREA_Y,
		//		OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING );
		//	pPanel->addLabel( this->step4Panel_tip, "",
		//		GENERAL_AREA_LABEL_WIDTH, GENERAL_AREA_LABEL_HEIGHT,
		//		"Finishing (optional)", controls->mGlobals->mLabelColor );
		//	pPanel->addButton( this->step4Panel_calibration, "Calibration",
		//		OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
		//		kofxGui_Button_Off, kofxGui_Button_Trigger );
		//	pPanel->addButton( this->step4Panel_previous, "Previous",
		//		OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
		//		kofxGui_Button_Off, kofxGui_Button_Trigger );
		//	pPanel->addButton( this->step4Panel_finish, "Finish",
		//		OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
		//		kofxGui_Button_Off, kofxGui_Button_Trigger );

		//	pPanel->mObjWidth = GENERAL_AREA_WIDTH;
		//	pPanel->mObjHeight = GENERAL_AREA_HEIGHT;

		//	pPanel->mObjects[2]->mObjX = GENERAL_AREA_PREV_X;
		//	pPanel->mObjects[2]->mObjY = GENERAL_AREA_PREV_Y;
		//	pPanel->mObjects[3]->mObjX = GENERAL_AREA_NEXT_X;
		//	pPanel->mObjects[3]->mObjY = GENERAL_AREA_NEXT_Y;

		//	pPanel->adjustToNewContent( 100, 0 );
		//	break;

		default:
			break;

	}
}
//--------------------------------------------------------------

void MultiCams::removePanels() {
	controls->removePanel( this->camerasDisplayPanel );
	controls->removePanel( this->devicesListPanel );
	controls->removePanel( this->gridSettingsPanel );
	controls->removePanel( this->devicesSettingsPanel );
	controls->removePanel( this->calibrationPanel );

	controls->removePanel( this->informationPanel );

	controls->removePanel( this->generalSettingsPanel );
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
void MultiCams::updateInfoPanel(int rawId) {
	currentCamera = utils->getRawCam( rawId );
	if ( currentCamera != NULL ) {
		removePanel( informationPanel );
		addPanel( informationPanel );
	}
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

void MultiCams::testDraw( ofEventArgs &e ) {
	ofSetColor( 0x654321 );
	ofFill();
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
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

	if ( bDraggingImage ) {
		this->draggingImage->draw();
	}
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
		////! Goto step 2
		//removePanel( generalSettingsPanel );
		//addPanel( step2Panel );
		bDevicesConfiguration = false;
	}
}
//--------------------------------------------------------------

void MultiCams::_setXY( int x, int y ) {
	if ( x * y > utils->getCount() ) {
		devGrid->setMode( kofxGui_Grid_Selectable, false );
	} else {
		devGrid->setMode( kofxGui_Grid_Selectable, true );
	}

	if ( utils != NULL ) {
		utils->setXY( XAxis, YAxis );
	}
	if ( camsGrid != NULL ) {
		camsGrid->setXY( XAxis, YAxis );
	}
	if ( devGrid != NULL ) {
		devGrid->setImages();
	}
}

//--------------------------------------------------------------