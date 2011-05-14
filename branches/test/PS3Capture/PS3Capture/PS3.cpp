//! PS3.cpp
/*!	Update: 2011-05-14 by Yishi Guo
            Add the capture and show functions */
// -------------------------------------------------

#include "PS3.h"

PS3::PS3(){
	_bShow = true;
	_bRunning = false;
	_bInitialized = false;

	_pCapBuffer = NULL;
};

PS3::~PS3() {
	_cam = NULL;
	_pCapBuffer = NULL;

};

int PS3::GetCameraCount() {
	return CLEyeGetCameraCount();
}

// Get GUID by camera Id
GUID PS3::GetGUID( int camId ) {
	return CLEyeGetCameraUUID( camId );
}

//! Convert the GUID to string
std::string PS3::GUID2String( GUID guid, char delimiter, bool uppercase ) {
	std::stringstream ss;
	if ( uppercase ) {
		ss << std::uppercase;
	} else {
		ss << std::nouppercase;
	}

	//! Get the hex format GUID string
	/*! Like: XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXXXX 
	*/
	ss.fill( '0' );	//! Fill the space with "0"
	ss << std::hex;	//! Output as Hexadecimal

	//! 8 Bytes	|	Data1
	ss.width( 8 );	//! Set the width to 8
	ss << guid.Data1 << delimiter;

	//! 4 Bytes	|	Data2
	ss.width( 4 );	ss << guid.Data2 << delimiter;

	//! 4 Bytes	|	Data3
	ss.width( 4 );	ss << guid.Data3 << delimiter;

	//! 4 Bytes	|	Initial two bytes from Data4
	ss.width( 2 );	ss << (int)guid.Data4[0];
	ss.width( 2 );	ss << (int)guid.Data4[1] << delimiter;

	//! 12 Bytes|	Remaining six bytes from Data4
	ss.width( 2 );	ss << (int)guid.Data4[2];
	ss.width( 2 );	ss << (int)guid.Data4[3];
	ss.width( 2 );	ss << (int)guid.Data4[4];
	ss.width( 2 );	ss << (int)guid.Data4[5];
	ss.width( 2 );	ss << (int)guid.Data4[6];
	ss.width( 2 );	ss << (int)guid.Data4[7];

	return ss.str();
}

//! The thread for capture
/*! Copy from CLEyeMulticamTest.cpp */
DWORD WINAPI PS3::CaptureThread( LPVOID instance ) {
	PS3 *pThis = (PS3*)instance;
	pThis->Run();

	return 0;
}

//! Run
void PS3::Run() {
	if ( !_bInitialized ) {
		return;
	}
	int width, height;
	IplImage *pCapImage;

	_cam = CLEyeCreateCamera( _camGUID, _camColorMode, _camResolution, _frameRate );
	
	if ( _cam == NULL ) {
		return;	//! Could not create camera
	}

	CLEyeCameraGetFrameDimensions( _cam, width, height );

	if ( _camColorMode == CLEYE_COLOR_PROCESSED || _camColorMode == CLEYE_COLOR_RAW ) {
		pCapImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 4 );
	} else {
		pCapImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	}

	//! Set some camera parameters
	CLEyeSetCameraParameter( _cam, CLEYE_GAIN, 0 );
	CLEyeSetCameraParameter( _cam, CLEYE_EXPOSURE, 511 );

	//! Start capturing
	CLEyeCameraStart( _cam );

	//! Get the image from captured buffer
	cvGetImageRawData( pCapImage, &_pCapBuffer );

	//! Image capturing loop
	while ( _bRunning ) {
		if ( _bCapture ) {
			CLEyeCameraGetFrame( _cam, _pCapBuffer );
			if ( _bShow ) {
				cvShowImage( _windowTitle.c_str(), pCapImage );
			}
		}
	}

	//! Stop camera capture
	CLEyeCameraStop( _cam );

	//! Destroy camera object
	CLEyeDestroyCamera( _cam );

	//! Release OpenCV resources
	cvReleaseImage( &pCapImage );

	_cam = NULL;
}

//! Set PS3 camera
void PS3::SetCamera( GUID camGUID, CLEyeCameraColorMode colorMode, CLEyeCameraResolution camRes, float frameRate ) {
	_camGUID = camGUID;
	_camColorMode = colorMode;
	_camResolution = camRes;
	_frameRate = frameRate;

	_bInitialized = true;
}

//! Get the GUID of camera
GUID PS3::GetGUID() const{
	return _camGUID;
}

//! Set the GUID of camera
void PS3::SetGUID( GUID guid ) {
	_camGUID = guid;
}

bool PS3::StartCamera() {
	//! The camera must be initialized
	if ( !_bInitialized ) {
		return false;
	}

	_bRunning = true;

	//! Show window or not
	if ( _bShow ) {
		if ( _windowTitle.empty() ) {
			_windowTitle = "GUID: " + GUID2String( _camGUID );
		}
		cvNamedWindow( _windowTitle.c_str(), CV_WINDOW_AUTOSIZE );
	}

	// Start the capture thread
	_hThread = CreateThread( NULL, 0, &PS3::CaptureThread, this, 0, 0 );

	if ( _hThread == NULL ) {
		printf( "Could not create capture thread\n" );
		//MessageBoxA( NULL, "Could not create capture thread", "PS3Capture", MB_ICONEXCLAMATION );
		return false;
	}

	return true;
}

void PS3::StopCamera() {
	if ( !_bRunning ) {
		return;
	}

	//! Stop the capturing loop
	_bRunning = false;

	WaitForSingleObject( _hThread, 1000 );

	//! Destroy window
	if ( _bShow ) {
		cvDestroyWindow( _windowTitle.c_str() );
	}
}

bool PS3::StartCapture() {
	if ( !_bRunning || _bCapture ) {
		return false;
	}

	if ( !CLEyeCameraStart( _cam ) ) {
		return false;
	}
	_bCapture = true;

	return true;
}

bool PS3::StopCapture() {
	if ( !_bRunning || !_bCapture ) {
		return false;
	}

	if ( !CLEyeCameraStop( _cam ) ) {
		return false;
	}

	_bCapture = false;

	return true;
}


bool PS3::ShowWindow( bool bShow ) {
	//! The camera must be capturing now!
	if ( !_bRunning || !_bCapture ) {
		return false;
	}

	if ( bShow && !_bShow ) {
		_bShow = true;
		// WaitForSingleObject(_hThread, 1000);
		cvNamedWindow( _windowTitle.c_str(), CV_WINDOW_AUTOSIZE );

		return true;
	} else if ( !bShow && _bShow ) {
		_bShow = false;
		// WaitForSingleObject(_hThread, 1000);
		cvDestroyWindow( _windowTitle.c_str() );

		return true;
	}

	return true;
}

void PS3::SetShow( bool bShow ) {
	_bShow = bShow;
}

void PS3::SetWindowTitle( string title ) {
	_windowTitle = title;
}

unsigned char* PS3::GetPixels() {
	return _pCapBuffer;
}