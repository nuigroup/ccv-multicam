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

#include "ofxNCore.h"

class MultiCams : public ofxGuiListener {

	//! ofxGUI stuff
	enum {
		
	};
public:
	//! Constructor
	MultiCams( bool bDebug = false );
	//! Destructor
	~MultiCams();


	/**********************************************************
	 *                 PUBLIC METHODS
	 **********************************************************/
	// BASIC EVENTS
	void _setup( ofEventArgs &e );
	void _update( ofEventArgs &e );
	void _draw( ofEventArgs &e );
	void _exit( ofEventArgs &e );

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
	ofxGui* controls;

	// DRAWING
	void drawFullMode();

	// LOAD/SAVE XML SETTINGS
	void loadXMLSettings();
	void SaveXMLSettings();

	/**********************************************************
	 *                 PRIVATE STUFF
	 **********************************************************/
private:

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