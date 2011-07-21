//! MultiCams.h
/*!
*  
*
*  Created by Yishi Guo on 05/31/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/
#ifndef MULTI_CAMS_H_
#define MULTI_CAMS_H_

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "Controls/ofxGuiTypes.h"
#include "Controls/ofxGui.h"
#include "MultiCams/SetDevices.h"
#include "MultiCams/CamsUtils.h"
// #include "Modules/ofxNCoreVision.h"
class ofxNCoreVision;

#define GENERAL_AREA_X 760
#define GENERAL_AREA_Y 30
#define GENERAL_AREA_WIDTH 160
#define GENERAL_AREA_HEIGHT 160
#define GENERAL_AREA_SLIDER_WIDTH 140
#define GENERAL_AREA_SLIDER_HEIGHT 10
#define GENERAL_AREA_LABEL_WIDTH 140
#define GENERAL_AREA_LABEL_HEIGHT 5
#define GENERAL_AREA_MATRIX_WIDTH 140
#define GENERAL_AREA_MATRIX_HEIGHT 60
#define GENERAL_AREA_PREV_X 10
#define GENERAL_AREA_PREV_Y 140
#define GENERAL_AREA_NEXT_X 110
#define GENERAL_AREA_NEXT_Y 140

#define RIGHT_PANEL_WIDTH 170
#define RIGHT_PANEL_X 740
#define RIGHT_PANEL_SLIDER_WIDTH 155
#define RIGHT_PANEL_SLIDER_HEIGHT 10
#define RIGHT_PANEL_Y_OFFSET 100

class MultiCams : public ofxGuiListener {

	//! ofxGUI stuff
	enum {
		//! Cameras Display Panel
		camerasDisplayPanel,
		camerasDisplayPanel_grid,
		camerasDisplayPanel_grid_reset,
		camerasDisplayPanel_grid_setting,
		camerasDisplayPanel_reset_all,

		//! Devices List Panel
		devicesListPanel,
		devicesListPanel_grid,
		devicesListPanel_arrow_right,
		devicesListPanel_arrow_left,

		//! Grid Settings Panel
		gridSettingsPanel,
		gridSettingsPanel_x,
		gridSettingsPanel_y,

		//! Devices Settings Panel
		devicesSettingsPanel,
		devicesSettingsPanel_set,

		//! Calibration Panel
		calibrationPanel,
		calibrationPanel_enter,

		//! General Settings Panel
		generalSettingsPanel,
		generalSettingsPanel_start,
		generalSettingsPanel_reset_all,
		generalSettingsPanel_save,
		generalSettingsPanel_exit,
		generalSettingsPanel_label1,

		dragging_image,

		informationPanel,
		informationPanel_image,
		informationPanel_uuid,
		informationPanel_type,
		informationPanel_hflip,
		informationPanel_vflip,
		informationPanel_auto_gain,
		informationPanel_gain,
		informationPanel_auto_exposure,
		informationPanel_exposure,
		informationPanel_auto_whitebalance,
		informationPanel_whitebalance_red,
		informationPanel_whitebalance_green,
		informationPanel_whitebalance_blue,


		step1Panel,
		step1Panel_tip,
		step1Panel_Xaxis,
		step1Panel_Yaxis,
		step1Panel_previous,
		step1Panel_next,

		step2Panel,
		step2Panel_tip,
		step2Panel_setDevices,
		step2Panel_previous,
		step2Panel_next,

		step3Panel,
		step3Panel_tip,
		step3Panel_matrix,
		step3Panel_reset_all,
		step3Panel_previous,
		step3Panel_next,

		step4Panel,
		step4Panel_tip,
		step4Panel_calibration,
		step4Panel_previous,
		step4Panel_finish,
	};
public:
	//! Constructor
	//MultiCams( bool bDebug = false );
	MultiCams();
	//! Destructor
	~MultiCams();


	/**********************************************************
	 *                 PUBLIC METHODS
	 **********************************************************/
	// SETUP
	void setup();
	void update( ofEventArgs &e );

	// MOUSE EVENTS
	void _mousePressed( ofMouseEventArgs &e );
	void _mouseDragged( ofMouseEventArgs &e );
	void _mouseReleased( ofMouseEventArgs &e );

	// KEY EVENTS
	void _keyPressed( ofKeyEventArgs &e );
	void _keyReleased( ofKeyEventArgs &e );

	// GUI
	void handleGui(int parameterId, int task, void* data, int length);
	void setupControls();
	void updateControls();
	void addPanels();
	void addPanel( int id );
	void removePanels();
	void removePanel( int id);
	void updateInfoPanel( int rawId);

	// PASS IN
	void passInCoreVision( ofxNCoreVision * core );
	void passInCamsUtils( CamsUtils* utils );

	// DRAWING
	void draw();

	void testDraw( ofEventArgs &e );

	// LOAD/SAVE XML SETTINGS
	void loadXMLSettings();
	void SaveXMLSettings();

	/**********************************************************
	 *                 PRIVATE METHODS
	 **********************************************************/
private:

	void _handleGui( int parameterId, int task, void* data, int length );
	void _draw();
	void switchSetDevicesGUI( bool showDevices = true );
	void _setXY( int x, int y );

	/**********************************************************
	 *                 PUBLIC STUFF
	 **********************************************************/
public:
	//! Controls
	ofxGui* controls;
	CamsUtils* utils;

	/**********************************************************
	 *                 PRIVATE STUFF
	 **********************************************************/
private:

	//! NCoreVision pointer
	ofxNCoreVision* _coreVision;

	int camCount;

	//////////////////////////////////
	// SET DEVICES
	//! SetDevices pointer
	SetDevices* setDevices;
	bool		bDevicesConfiguration;

	//! Horizontal and vertical camera count
	int XAxis;
	int YAxis;

	///////////////////
	// TEST STUFF
	int testInt;

	//! debug mode
	bool bDebugMode;
	///////////////////
	// CAMERAS GRID
	ofxGuiGrid* camsGrid;

	///////////////////
	// DEVICES LIST
	ofxGuiGrid* devGrid;

	///////////////////
	// DRAGGING IMAGE
	//! Dragging image
	ofxGuiImage* draggingImage;

	//! Is dragging now?
	bool bDraggingImage;

	//! Image draw offset
	float draggingXOffset;
	float draggingYOffset;

	//! Full display mode
	bool bFullMode;

	//! Show interface
	bool bShowInterface;
	bool bShowConfiguration;

	//! window
	string windowTitle;
	int winWidth;
	int winHeight;

	//! Fonts
	ofTrueTypeFont		testFont;

	///////////////////////////
	// XML Settings

	//! config file name
	string _xmlFileName;

	//! XML load/save
	ofxXmlSettings XML;

	///////////////////////////
	PS3* currentCamera;
	ofxGuiImage* previewImage;

};

#endif 