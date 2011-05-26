//! CamSplice.h
/*! Creator: Yishi Guo
    Create Date: 2011-05-24 */

#ifndef _CAM_SPLICE_H_
#define _CAM_SPLICE_H_

#include <vector>
#include <string>
#include <cv.h>
#include <highgui.h>

#include "PS3.h"

//! Class CamSplice
/*! Splice the cameras */
class CamSplice {

	///////////////////////////////
	//! Public Methods
public:
	//! Constructor
	CamSplice();

	//! Destructor
	~CamSplice();

	//! Set x and y axis camera number
	void SetXY( int x, int y );

	//! Set each camera width and height
	void SetWidthHeight( int width, int height );

	//! Set index vector
	bool SetIndexVec( const vector<int> indexVec );

	//! Set cameras vector
	bool SetCamVec( const vector<PS3*> camVec );

	//! Set the title of window
	bool SetWindowTitle( const string title );

	//! Start
	bool Start();

	//! Stop
	bool Stop();

	///////////////////////////////
	// ! Private Methods
private:
	//! Run
	bool Run();

	//!
	static DWORD WINAPI DisplayThread( LPVOID instance );

	//! Splice the cameras' pixels to one image
	bool SetImagePixels( IplImage* img );

	//! Is color mode
	bool IsColorMode();

	//! Is frame new
	bool IsFrameNew();

	///////////////////////////////
	//! Private Fields
private:
	//! x axis camera number
	int _x;

	//! y axis camera number
	int _y;

	//! CAMERA width for display
	int _width;

	//! CAMERA height for display
	int _height;

	//! window title
	string _title;

	//! index vector
	vector<int> _indexVec;

	//! Cameras vector
	vector<PS3*> _camVec;

	//! Splice running
	bool _bRunning;

	//! Handle of capture thread
	HANDLE _hThread;

};

#endif