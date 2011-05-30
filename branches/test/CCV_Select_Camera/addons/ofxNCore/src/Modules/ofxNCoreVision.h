
/*
 *  ofxNCoreVision.h
 *  NUI Group Community Core Vision
 *
 *  Created by NUI Group Dev Team A on 2/1/09.
 *  Copyright 2009 NUI Group/Inc. All rights reserved.
 *
 */
#ifndef _ofxNCoreVision_H
#define _ofxNCoreVision_H

//Main
#include "ofMain.h"
//Addons
#ifdef TARGET_WIN32
    #include "ofxffmv.h"
    #include "ofxPS3.h"
	#include "ofxDSVL.h"
#endif
#include "ofxOpenCv.h"
#include "ofxDirList.h"
#include "ofxVectorMath.h"
#include "ofxNetwork.h"
#include "ofxOsc.h"
#include "ofxThread.h"
#include "ofxXmlSettings.h"
#include "ofxFiducialTracker.h"

// Our Addon
#include "ofxNCore.h"

// height and width of the source/tracked draw window
#define MAIN_WINDOW_HEIGHT 240.0f
#define MAIN_WINDOW_WIDTH  320.0f

class ofxNCoreVision : public ofxGuiListener
{
	// ofxGUI setup stuff
	enum
	{
		propertiesPanel,
		propertiesPanel_flipV,
		propertiesPanel_flipH,
		propertiesPanel_settings,
		propertiesPanel_pressure,

		optionPanel,
		optionPanel_tuio_osc,
		optionPanel_tuio_tcp,
		optionPanel_bin_tcp,

		calibrationPanel,
		calibrationPanel_calibrate,
		calibrationPanel_warp,

		sourcePanel,
		sourcePanel_cam,
		sourcePanel_nextCam,
		sourcePanel_previousCam,
		
		TemplatePanel,
		TemplatePanel_minArea,
		TemplatePanel_maxArea,

		backgroundPanel,
		backgroundPanel_remove,
		backgroundPanel_dynamic,
		backgroundPanel_learn_rate,

		smoothPanel,
		smoothPanel_use,
		smoothPanel_smooth,

		amplifyPanel,
		amplifyPanel_use,
		amplifyPanel_amp,

		highpassPanel,
		highpassPanel_use,
		highpassPanel_blur,
		highpassPanel_noise,

		trackedPanel,
		trackedPanel_darkblobs,
		trackedPanel_use,
		trackedPanel_threshold,
		trackedPanel_min_movement,
		trackedPanel_min_blob_size,
		trackedPanel_max_blob_size,
		trackedPanel_outlines,
		trackedPanel_ids,

		trackingPanel, //Panel for selecting what to track-Fingers, Objects or Fiducials
		trackingPanel_trackFingers,
		trackingPanel_trackObjects,
		trackingPanel_trackFiducials,

		savePanel,
		kParameter_SaveXml,
		kParameter_File,
		kParameter_LoadTemplateXml,
		kParameter_SaveTemplateXml,
	};

public:
	ofxNCoreVision(bool debug)
	{
		ofAddListener(ofEvents.mousePressed, this, &ofxNCoreVision::_mousePressed);
		ofAddListener(ofEvents.mouseDragged, this, &ofxNCoreVision::_mouseDragged);
		ofAddListener(ofEvents.mouseReleased, this, &ofxNCoreVision::_mouseReleased);
		ofAddListener(ofEvents.keyPressed, this, &ofxNCoreVision::_keyPressed);
		ofAddListener(ofEvents.keyReleased, this, &ofxNCoreVision::_keyReleased);
		ofAddListener(ofEvents.setup, this, &ofxNCoreVision::_setup);
		ofAddListener(ofEvents.update, this, &ofxNCoreVision::_update);
		ofAddListener(ofEvents.draw, this, &ofxNCoreVision::_draw);
		ofAddListener(ofEvents.exit, this, &ofxNCoreVision::_exit);

		#ifdef TARGET_WIN32
            PS3  = NULL;
			ffmv = NULL;
			dsvl = NULL;
		#endif

		debugMode = debug;
		
		vidGrabber = NULL;
		vidPlayer = NULL;
		//initialize filter
		filter = NULL;
		filter_fiducial = NULL;
		//fps and dsp calculation
		frames		= 0;
		fps			= 0;
		lastFPSlog	= 0;
		differenceTime = 0;
		//bools
		bCalibration= 0;
		bFullscreen = 0;
		bcamera = 0;
		bShowLabels = 1;
		bMiniMode = 0;
		bDrawOutlines = 1;
		bGPUMode = 0;
		bTUIOMode = 0;
		bFidMode = 0;
		
		showConfiguration = 0;
		//camera
		camRate = 30;
		camWidth = 320;
		camHeight = 240;
		//ints/floats
		backgroundLearnRate = .01;
		MIN_BLOB_SIZE = 2;
		MAX_BLOB_SIZE = 100;

		contourFinder.bTrackFingers=false;
		contourFinder.bTrackObjects=false;
		contourFinder.bTrackFiducials=false;

        //if auto tracker is defined then the tracker automagically comes up
        //on startup..
        #ifdef STANDALONE
            bStandaloneMode = true;
        #else
            bStandaloneMode = false;
        #endif
	}

	~ofxNCoreVision()
	{
		// AlexP
		// C++ guarantees that operator delete checks its argument for null-ness
		delete filter;		filter = NULL;
		delete filter_fiducial;		filter_fiducial = NULL;
		delete vidGrabber;	vidGrabber = NULL;
		delete vidPlayer;	vidPlayer = NULL;
		#ifdef TARGET_WIN32
		delete PS3;		PS3 = NULL;
		delete ffmv; 	ffmv = NULL;
		delete dsvl;	dsvl = NULL;
		#endif
	}

	/****************************************************************
	 *						Public functions
	 ****************************************************************/
	//Basic Events called every frame
    void _setup(ofEventArgs &e);
    void _update(ofEventArgs &e);
	void _draw(ofEventArgs &e);
    void _exit(ofEventArgs &e);
    //Mouse Events
    void _mousePressed(ofMouseEventArgs &e);
    void _mouseDragged(ofMouseEventArgs &e);
    void _mouseReleased(ofMouseEventArgs &e);
    //Key Events
    void _keyPressed(ofKeyEventArgs &e);
    void _keyReleased(ofKeyEventArgs &e);

	//GUI
	void setupControls();
	void		handleGui(int parameterId, int task, void* data, int length);
	ofxGui*		controls;

	//image processing stuff
	void initDevice();
	void getPixels();
	void grabFrameToCPU();
	void grabFrameToGPU(GLuint target);

	//drawing
	void drawFingerOutlines();
	void drawMiniMode();
	void drawFullMode();

	void drawFiducials();



	//Load/save settings
	void loadXMLSettings();
	void saveSettings();

	//Getters
	std::map<int, Blob> getBlobs();
	std::map<int, Blob> getObjects();

	/***************************************************************
	 *					Video Capture Devices
	 ***************************************************************/
    #ifdef TARGET_WIN32
        ofxffmv*        ffmv; //for firefly mv
        ofxPS3*			PS3;  //for ps3
		ofxDSVL*		dsvl;
	#endif
	ofVideoGrabber*		vidGrabber;
    ofVideoPlayer*		vidPlayer;

	/****************************************************************
	 *            Variables in config.xml Settings file
	 *****************************************************************/
    int					deviceID;
	int 				frameseq;
	int 				threshold;
	int					wobbleThreshold;
	int 				camRate;
	int 				camWidth;
	int 				camHeight;
	int					winWidth;
	int					winHeight;
	int					MIN_BLOB_SIZE;
	int					MAX_BLOB_SIZE;
	float				backgroundLearnRate;

	bool				showConfiguration;
	bool				bcamera;
	bool  				bMiniMode;
	bool				bShowInterface;
	bool				bShowPressure;
	bool				bDrawOutlines;
	bool				bTUIOMode;
	bool  				bFullscreen;
	bool 				bCalibration;
	bool				bShowLabels;
	bool				bNewFrame;
	//filters
	bool				bAutoBackground;
	//modes
	bool				bGPUMode;

	//Area slider variables
	int					minTempArea;
	int					maxTempArea;

	//For the fiducial mode drawing
	bool				bFidMode;
	//auto ~ standalone/non-addon
	bool                bStandaloneMode;
	/*****************************************************
	*		Fiducial Finder
	*******************************************************/
	ofxFiducialTracker	fidfinder;

	float				fiducialDrawFactor_Width; //To draw the Fiducials in the right place we have to scale from camWidth to filter->camWidth
    float				fiducialDrawFactor_Height;

	Filters*			filter_fiducial;
	CPUImageFilter		processedImg_fiducial;



	/****************************************************
	 *End config.xml variables
	 *****************************************************/
	
	//Debug mode variables
	bool				debugMode;

	//Undistortion of Image - Required for some setups
	bool				bUndistort;
	ofxCvGrayscaleImage	undistortedImg;

	//FPS variables
	int 				frames;
	int  				fps;
	float				lastFPSlog;
	int					differenceTime;

	//Fonts
	ofTrueTypeFont		verdana;
	ofTrueTypeFont      sidebarTXT;
	ofTrueTypeFont		bigvideo;

	//Images
	ofImage				background;

	//Blob Tracker
	BlobTracker			tracker;

	//Template Utilities
	TemplateUtils		templates;

	//Template Registration
	ofRectangle			rect;
	ofRectangle			minRect;
	ofRectangle			maxRect;

	//Object Selection bools
	bool				isSelecting;

	//Area sliders


    /****************************************************************
	 *						Private Stuff
	 ****************************************************************/
	string				videoFileName;

	int					maxBlobs;

	// The variable which will check the initilisation of camera
	//to avoid multiple initialisation
	bool				cameraInited; 

	//Calibration
    Calibration			calib;

	//Blob Finder
	ContourFinder		contourFinder;

	//Image filters
	Filters*			filter;
	CPUImageFilter      processedImg;
	ofxCvColorImage		sourceImg;

	//XML Settings Vars
	ofxXmlSettings		XML;
	string				message;

	//Communication
	TUIO				myTUIO;
	string				tmpLocalHost;
    int					tmpPort;
	int					tmpFlashPort;

	//Logging
	char				dateStr [9];
	char				timeStr [9];
	time_t				rawtime;
	struct tm *			timeinfo;
	char				fileName [80];
	FILE *				stream ;
};
#endif
