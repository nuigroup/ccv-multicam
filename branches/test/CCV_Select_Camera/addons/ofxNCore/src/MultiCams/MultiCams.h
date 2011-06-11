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

class MultiCams : public ofxGuiListener {

	//! ofxGUI stuff
	enum {
		camerasDisplayPanel,
		camerasDisplayPanel_grid,

		generalSettingsPanel,
		generalSettingsPanel_start,
		generalSettingsPanel_save,
		generalSettingsPanel_cancel,
		generalSettingsPanel_label1,

		devicesListPanel,
		devicesListPanel_grid,
		devicesListPanel_arrow_right,
		devicesListPanel_arrow_left,

		informationPanel,

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

	// PASS IN
	void passInCoreVision( ofxNCoreVision * core );

	// DRAWING
	void draw();

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

	/**********************************************************
	 *                 PUBLIC STUFF
	 **********************************************************/
public:
	//! Controls
	ofxGui* controls;

	/**********************************************************
	 *                 PRIVATE STUFF
	 **********************************************************/
private:

	//! NCoreVision pointer
	ofxNCoreVision* _coreVision;

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

};

#endif 