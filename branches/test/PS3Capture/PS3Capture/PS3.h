// --------------------------------------------------------
//! PS3.h
//
//
// Author: Yishi Guo
// Create Date: 2011-05-12
// Update:	2011-05-14	by Yishi Guo
//          Add capture and show functions
// --------------------------------------------------------

#ifndef _PS3_H_
#define _PS3_H_

#include "CLEyeMulticam.h"
#include <string>
#include <sstream>
#include <cv.h>
#include <highgui.h>

using namespace std;

class PS3 {
public:
	//! Constructor
	PS3();

	//! Destructor
	~PS3();

	//! Get the camera count
	/*!
		\return the camera count
	*/
	static int GetCameraCount();

	//! Convert GUID to string
	/*!
		\param guid a GUID structure param
		\return string of guid
	*/
	static std::string GUID2String( GUID guid, char delimiter = '-', bool uppercase = false );

	//! Get camera GUID by camera Id
	static GUID GetGUID( int camId );

	//! Get GUID of the camera
	GUID GetGUID() const;

	//! Set the GUID of camera
	void SetGUID( GUID guid );

	//!
	static DWORD WINAPI CaptureThread( LPVOID instance );

	//! Main method to running camera
	void Run();

	//! Set some values about camera
	void SetCamera( GUID camGUID, CLEyeCameraColorMode colorMode,
		CLEyeCameraResolution camRes, float frameRate);

	//! Start the camera
	bool StartCamera();

	//! Stop the camera
	void StopCamera();

	//! Start the capture
	bool StartCapture();

	//! Stop the capture
	bool StopCapture();

	//! Show window or not
	bool ShowWindow( bool bShow );

	//! Set window show value
	void SetShow( bool bShow );

	//! Set the window title
	void SetWindowTitle( string title );

	//! Get the pixels of frame
	unsigned char* GetPixels();

private:
	//! GUID of camera
	GUID _camGUID;

	//! Camera color mode
	CLEyeCameraColorMode _camColorMode;

	//! Camera resolution
	CLEyeCameraResolution _camResolution;

	//! Camera frame rate
	float _frameRate;

	//! PS3 camera instance
	CLEyeCameraInstance _cam;

	//! Is camera running
	bool _bRunning;

	//! Is capture
	bool _bCapture;

	//! Is show the window
	bool _bShow;

	//! The title of window
	string _windowTitle;

	//! Handle of capture thread
	HANDLE _hThread;

	unsigned char* _pCapBuffer;

	//! Is initialized
	bool _bInitialized;

};
#endif