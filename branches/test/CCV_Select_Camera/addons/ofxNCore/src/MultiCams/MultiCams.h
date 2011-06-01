//! MultiCams.h
/*!
*  
*
*  Created by Yishi Guo on 31/05/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/
#ifndef MULTI_CAMS_H_
#define MULTI_CAMS_H_

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "Controls/ofxGuiTypes.h"
#include "Controls/ofxGui.h"
// #include "Modules/ofxNCoreVision.h"
class ofxNCoreVision;

class MultiCams : public ofxGuiListener {

	//! ofxGUI stuff
	enum {
		camerasDisplayPanel,

		generalSettingsPanel,
		generalSettingsPanel_save,
		generalSettingsPanel_cancel,

		devicesListPanel,

		informationPanel,
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
	void addPanels();
	void removePanels();

	// PASS IN
	void passInCoreVision( ofxNCoreVision * core );

	// DRAWING
	void draw();

	// LOAD/SAVE XML SETTINGS
	void loadXMLSettings();
	void SaveXMLSettings();

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

	///////////////////
	// TEST STUFF
	int testInt;

	//! debug mode
	bool bDebugMode;

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