// --------------------------------------------------------
//! PS3.h
//
//
// Author: Yishi Guo
// Create Date: 2011-05-12
// --------------------------------------------------------

#ifndef _PS3_H_
#define _PS3_H_

#include "CLEyeMulticam.h"
#include <string>
#include <sstream>

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

	//! Get camera GUID
	static GUID GetGUID( int camId );

	static DWORD WINAPI CaptureThread( LPVOID instance );

private:
	//! GUID of camera
	GUID _cameraGUID;
};
#endif