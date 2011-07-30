//! ofxGuiImage.h
/*!
*  
*
*  Created by Yishi Guo on 06/10/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

// ----------------------------------------------

#ifndef OFX_GUI_IMAGE
#define OFX_GUI_IMAGE

// ----------------------------------------------

#define PARAM_TEXT_OFFSET_X 5

// ----------------------------------------------

#include "ofxGuiTypes.h"
#include "ofxGuiObject.h"
//#include "PS3.h"
#include "ofxCameraBase.h"
#include "ofxCvGrayscaleImage.h"

// ----------------------------------------------

class ofxGuiImage : public ofxGuiObject {
public:
	ofxGuiImage();
	~ofxGuiImage();

	void init( int id, string name, int x, int y, int width, int height, unsigned char* pImage = NULL );

	void setImage( unsigned char* pImage, int width, int height );
	void setBlank( bool bBlank = true );
	//void setCamera( PS3* cam );
	void setCamera( ofxCameraBase *cam );
	void setTitle( string name );
	void setXYPos( int x, int y );
	void setDrawInfo( bool draw = true );
	void setCamSelected( bool selected = true );

	//PS3* getCamera();
	ofxCameraBase* getCamera();

	bool update( int parameterId, int task, void* data, int length);
	void draw();

	bool mouseDragged( int x, int y, int button );
	bool mousePressed( int x, int y, int button );
	bool mouseReleased( int x, int y, int button );

	void buildFromXml();
	void saveToXml();

	unsigned char* pImage;
	bool bImageSet;

private:
	void drawBlank();
	void drawImage();
	void drawString( int x, int y, string str, bool center, ofRGBA color );
	void drawInfo();
	void drawSelectedText();

	void setCanDrawInfo( bool draw );

	//PS3* pCam;
	ofxCameraBase* pCam;
	ofxCvGrayscaleImage* pCvImage;
	ofRGBA color;

	unsigned int mCamWidth, mCamHeight;
	unsigned char mCamDepth;

	bool bDrawInfo;
	bool bCanDrawInfo;

	bool bCamSelected;

	string strSelectedText;
};

// ----------------------------------------------

#endif