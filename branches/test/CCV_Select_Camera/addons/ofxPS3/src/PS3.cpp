//! PS3.cpp
/*!
*  
*
*  Created by Yishi Guo on 06/13/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

#include "PS3.h"

PS3::PS3(){
	_bShow = true;
	_bRunning = false;
	_bInitialized = false;
	_bHFlip = false;
	_bVFlip = false;
	_bDebugMode = false;
	_bCapture = false;

	_frameCount = 0;
	_oldFrameCount = 0;
	_fps = 0;

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

GUID PS3::String2GUID( string str, char delimiter, bool uppercase ) {
	// TODO
	GUID guid;
	int Data4[8];
	string formatStr;

	if ( uppercase ) {
		// TODO use the delimiter rather then the "-"
		formatStr = "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X";
	} else {
		formatStr = "%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x";
	}
	sscanf( str.c_str(), formatStr.c_str(),
		&guid.Data1, &guid.Data2, &guid.Data3, &Data4[0], &Data4[1],
		&Data4[2], &Data4[3], &Data4[4], &Data4[5], &Data4[6], &Data4[7] );

	// Ref:
	//		http://hi.baidu.com/yapn/blog/item/a7acb40117e9810e738da5d1.html
	for ( int i = 0; i < 8; ++i ) {
		guid.Data4[i] = (unsigned char)Data4[i];
	}

	return guid;
}

std::string PS3::Int2String( int val ) {
	std::stringstream ss;
	ss << val;

	return ss.str();
}

bool PS3::EqualGUID( GUID guid1, GUID guid2 ) {
	if (
		guid1.Data1 == guid2.Data1 &&
		guid1.Data2 == guid2.Data2 &&
		guid1.Data3 == guid2.Data3 &&
		guid1.Data4[0] == guid2.Data4[0] &&
		guid1.Data4[1] == guid2.Data4[1] &&
		guid1.Data4[2] == guid2.Data4[2] &&
		guid1.Data4[3] == guid2.Data4[3] &&
		guid1.Data4[4] == guid2.Data4[4] &&
		guid1.Data4[5] == guid2.Data4[5] &&
		guid1.Data4[6] == guid2.Data4[6] &&
		guid1.Data4[7] == guid2.Data4[7]
	) {
		return true;
	}

	return false;
}

////! The thread for capture
///*! Copy from CLEyeMulticamTest.cpp */
//DWORD WINAPI PS3::CaptureThread( LPVOID instance ) {
//	PS3 *pThis = (PS3*)instance;
//	pThis->Run();
//
//	return 0;
//}

//! Run
void PS3::Run() {
	if ( !_bInitialized ) {
		return;
	}
	int width, height;
	//IplImage *pCapImage;

	_cam = CLEyeCreateCamera( _camGUID, _camColorMode, _camResolution, _frameRate );

	//Sleep( 300 );
	
	if ( _cam == NULL ) {
		printf( "\nPS3::Run()\tCould not create camera\n" );
		return;	//! Could not create camera
	}
	//printf( "\tPS3::Run()\t#1\n" );

	CLEyeCameraGetFrameDimensions( _cam, width, height );

	_pCapBuffer = new unsigned char[width * height * 4];
	//printf( "PS3::Run() p=%p\n", _pCapBuffer );

	//if ( _camColorMode == CLEYE_COLOR_PROCESSED || _camColorMode == CLEYE_COLOR_RAW ) {
	//	pCapImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 4 );
	//} else {
	//	pCapImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	//}

	//! Set some camera parameters
	CLEyeSetCameraParameter( _cam, CLEYE_GAIN, 0 );
	CLEyeSetCameraParameter( _cam, CLEYE_EXPOSURE, 511 );
	CLEyeSetCameraParameter( _cam, CLEYE_HFLIP, _bHFlip );
	CLEyeSetCameraParameter( _cam, CLEYE_VFLIP, _bVFlip );

	//! Start capturing
	if ( !CLEyeCameraStart( _cam ) ) {
		std::cout << "Could not start camera!\n" << std::endl;
		return;
	}
	//Sleep( 300 );

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

//! Get the GUID of the camera [return string]
std::string PS3::GetGUIDStr() const {
	return GUID2String( GetGUID() );
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
	_bCapture = true;

	////! Show window or not
	//if ( _bShow ) {
	//	if ( _windowTitle.empty() ) {
	//		_windowTitle = "GUID: " + GUID2String( _camGUID );
	//	}
	//	_sWindowName = _windowTitle.c_str();
	//	//cvNamedWindow( _sWindowName, CV_WINDOW_AUTOSIZE );
	//	//cvNamedWindow( _windowTitle.c_str(), CV_WINDOW_AUTOSIZE );
	//}

	//// Start the capture thread
	//_hThread = CreateThread( NULL, 0, &PS3::CaptureThread, this, 0, 0 );

	//if ( _hThread == NULL ) {
	//	printf( "Could not create capture thread\n" );
	//	//MessageBoxA( NULL, "Could not create capture thread", "PS3Capture", MB_ICONEXCLAMATION );
	//	return false;
	//}
	Run();

	return true;
}

void PS3::StopCamera() {
	//! Stop camera capture
	CLEyeCameraStop( _cam );

	//! Destroy camera object
	CLEyeDestroyCamera( _cam );

	////! Release OpenCV resources
	//cvReleaseImage( &pCapImage );

	_cam = NULL;
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

unsigned char* PS3::GetPixels() {
	//printf( "PS3::GetPixels()\t p = %p\n", _pCapBuffer );
	//cout << endl;
	return _pCapBuffer;
}

bool PS3::SetHFlip( bool flip ) {
	//! Camera must be created
	if ( _cam == NULL ) {
		return false;
	}
	if ( _bRunning ) {
		if ( CLEyeSetCameraParameter( _cam, CLEYE_HFLIP, flip ) ) {
			_bHFlip = flip;
			return true;
		}
	} else {
		_bHFlip = flip;
		return true;
	}


	return false;
}

bool PS3::SetVFlip( bool flip ) {
	//! Camera must be created
	if ( _cam == NULL ) {
		return false;
	}
	if ( _bRunning ) {
		if ( CLEyeSetCameraParameter( _cam, CLEYE_VFLIP, flip ) ) {
			_bVFlip = flip;
			return true;
		}
	} else {
		_bVFlip = flip;
		return true;
	}

	return false;
}

void PS3::SetDebugMode( bool debug ) {
	_bDebugMode = debug;
}

int PS3::GetFrameCount() const {
	return _frameCount;
}

void PS3::ClearFrameCount() {
	_frameCount = 0;
}

int PS3::GetWidth() {
	//! Camera must be init
	if ( _cam == NULL ) {
		return -1;
	}

	int width, height;
	CLEyeCameraGetFrameDimensions( _cam, width, height );

	return width;
}

int PS3::GetHeight() {
	//! Camera must be init
	if ( _cam == NULL ) {
		return -1;
	}

	int width, height;
	CLEyeCameraGetFrameDimensions( _cam, width, height );

	return height;
}

int PS3::GetFPS() {
	return _fps;
}

bool PS3::GetHFlip() {
	return _bHFlip;
}

bool PS3::GetVFlip() {
	return _bVFlip;
}

void PS3::PrintInfo() {
	std::cout << "GUID:\t" << GUID2String( GetGUID(), '-', true ) << std::endl;
	std::cout << "Framerate:\t" << _frameRate << std::endl;
	std::cout << "Running:\t" << _bRunning << std::endl;
	std::cout << "Capturing:\t" << _bCapture << std::endl;
	std::cout << "Window title:\t" << _windowTitle << std::endl;
	std::cout << "Horizontal flip:\t" << _bHFlip << std::endl;
	std::cout << "Vertical flip:\t" << _bVFlip << std::endl;
	std::cout << "FPS:\t" << GetFPS() << std::endl;
	std::cout << "Width:\t" << GetWidth() << std::endl;
	std::cout << "Height:\t" << GetHeight() << std::endl;
	std::cout << "FrameCount:\t" << GetFrameCount() << std::endl;
}

bool PS3::IsColorMode() {
	if ( _camColorMode == CLEYE_COLOR_PROCESSED ||
		_camColorMode == CLEYE_COLOR_RAW ) {
		return true;
	}

	return false;
}

bool PS3::IsFrameNew() {
	static int frame = 0;
	static double lastFPSlog = 0;
	double now = GetTickCount();
	bool frameNew = CLEyeCameraGetFrame( _cam, _pCapBuffer );

	//! Count the fps and framecount
	if ( frameNew ) {
		_frameCount++;
		frame++;
	}
	
	//! 1 sec ago
	if ( now >= lastFPSlog + 1000 ) {
		_fps = frame;
		frame = 0;
		lastFPSlog = now;
	}

	return frameNew;
}

bool PS3::DecrementParam( int  param ) {
	if ( !_cam ) {
		return false;
	}
	int nowValue = CLEyeGetCameraParameter( _cam, (CLEyeCameraParameter)param );
	printf( "CLEyeGetParameter %d\n", nowValue );
	
	//! We decrease value by 10
	CLEyeSetCameraParameter( _cam, (CLEyeCameraParameter)param, nowValue - 10);

	return true;
}

bool PS3::IncrementParam( int param ) {
	if ( !_cam ) {
		return false;
	}

	int nowValue = CLEyeGetCameraParameter( _cam , (CLEyeCameraParameter)param );
	printf( "CLEyeGetParameter %d\n", nowValue );
	
	//! We increase it by 10
	CLEyeSetCameraParameter( _cam, (CLEyeCameraParameter)param, nowValue + 10 );

	return true;
}