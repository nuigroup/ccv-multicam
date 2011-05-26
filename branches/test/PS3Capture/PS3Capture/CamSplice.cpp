//! CamSplice.cpp
/*! Creator: Yishi Guo
    Create date: 2011-05-24 */

#include "CamSplice.h"

CamSplice::CamSplice() {
	_x = 0;
	_y = 0;

}

CamSplice::~CamSplice() {
	for ( int i = 0; i < _camVec.size(); ++i ) {
		_camVec[i] = NULL;
	}
}

void CamSplice::SetXY( int x, int y ) {
	_x = x;
	_y = y;
}

void CamSplice::SetWidthHeight( int width, int height ) {
	_width = width;
	_height = height;
}

bool CamSplice::SetIndexVec( const vector<int> indexVec ) {
	_indexVec.clear();
	for ( int i = 0; i < indexVec.size(); ++i ) {
		_indexVec.push_back( indexVec[i] );
	}

	return true;
}

bool CamSplice::SetCamVec( const vector<PS3*> camVec ) {
	_camVec.clear();
	for ( int i = 0; i < camVec.size(); ++i ) {
		_camVec.push_back( camVec[i] );
	}

	return true;
}

bool CamSplice::SetWindowTitle( const string title ) {
	_title = title;

	return true;
}

DWORD WINAPI CamSplice::DisplayThread( LPVOID instance ) {
	CamSplice *pThis = (CamSplice*)instance;
	pThis->Run();

	return 0;
}

bool CamSplice::Run() {
	//! The camera vector must have element, as we use the vec[0] below.
	if ( _camVec.size() <= 0 ) {
		return false;
	}
	int spliceWidth, spliceHeight;
	IplImage *pSpliceImg;

	//! The width of each camera must be same!
	spliceWidth = _x * _camVec[0]->GetWidth();
	spliceHeight = _y * _camVec[0]->GetHeight();

	// DEBUG
	printf( "SPLICE\n\twidth: %d\n\theight: %d\n", spliceWidth, spliceHeight );

	//! Create opencv image by the color mode of camera
	if ( IsColorMode() ) {
		pSpliceImg = cvCreateImage( cvSize( spliceWidth, spliceHeight ), IPL_DEPTH_8U, 4 );
	} else {
		pSpliceImg = cvCreateImage( cvSize( spliceWidth, spliceHeight ), IPL_DEPTH_8U, 1 );
	}

	///////////////////////// ------------------- 2011-05-24 17:23
	while ( _bRunning ) {
		if ( IsFrameNew() ) {
			//! Set the pixels for show
			if ( !SetImagePixels( pSpliceImg ) ) {
				break;
			}

			//! Display the spliced image
			cvShowImage( _title.c_str(), pSpliceImg );
		}
	}
	
	cvReleaseImage( &pSpliceImg );

	return true;
}

bool CamSplice::Start() {
	_bRunning = true;

	if ( _title.empty() ) {
		_title = PS3::Int2String( _camVec.size() ) + "Cameras Splice";
	}
	cvNamedWindow( _title.c_str(), CV_WINDOW_AUTOSIZE );

	_hThread = CreateThread( NULL, 0, &CamSplice::DisplayThread, this, 0, 0 );

	if ( _hThread == NULL ) {
		printf( "Could not create capture thread\n" );
		return false;
	}

	return true;
}

bool CamSplice::Stop() {
	if ( !_bRunning ) {
		return false;
	}

	_bRunning = false;

	WaitForSingleObject( _hThread, 1000 );

	cvDestroyAllWindows();

	return true;
}

bool CamSplice::SetImagePixels( IplImage *img ) {

	//! Must be running
	if ( !_bRunning ) {
		return false;
	}
	uchar* ptr;
	int vecIndex, i, j, index;
	for ( int h = 0; h < img->height; ++h ) {
		//! Get the first pointer of every line
		ptr = (uchar*)(img->imageData + h * img->widthStep);
		for ( int w = 0; w < img->width; ++w ) {
			//! The index of camera which will be copy from
			vecIndex = w/_width + _x * (h/_height);
			//! The point of x axis (camera)
			i = w % _width;
			//! The point of y axis (camera)
			j = h % _height;
			
			if ( img->nChannels == 1 ) {	//! Gray mode
				index = _width * j + i;
				ptr[w] = _camVec[vecIndex]->GetPixels()[index];
			} else if ( img->nChannels == 4 ) {	//! Color mode
				index = 4 * (_width * j + i );
				*ptr = _camVec[vecIndex]->GetPixels()[index+0];	ptr++;
				*ptr = _camVec[vecIndex]->GetPixels()[index+1];	ptr++;
				*ptr = _camVec[vecIndex]->GetPixels()[index+2];	ptr++;
				*ptr = _camVec[vecIndex]->GetPixels()[index+3];	ptr++;
			} else {	//! Other mode will be wrong
				return false;
			}
		}
	}

	return true;
}

bool CamSplice::IsColorMode() {

	//! Camera vector must be set
	if ( _camVec.size() <= 0 ) {
		return false;
	}
	//! For this moment, return the first camera's color mode
	return _camVec[0]->IsColorMode();
}

bool CamSplice::IsFrameNew() {
	for ( int i = 0; i < _camVec.size(); ++i ) {
		if ( _camVec[i]->IsFrameNew() ) {
			return true;
		}
	}

	return false;
}