//! ofxGuiArrow.h
/*!
*  
*
*  Created by Yishi Guo on 06/11/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

// ----------------------------------------------

#ifndef OFX_GUI_ARROW
#define OFX_GUI_ARROW

// ----------------------------------------------

#include "ofxGuiTypes.h"
#include "ofxGuiObject.h"

// ----------------------------------------------

class ofxGuiArrow : public ofxGuiObject {
public:
	ofxGuiArrow();

	void init( int id, string name, int x, int y, int width, int height, bool rightward );
	
	void setDirection( bool rightward );

	bool update( int id, int task, void* data, int length );
	void draw();

	bool mouseDragged( int x, int y, int button );
	bool mousePressed( int x, int y, int button );
	bool mouseReleased( int x, int y, int button );

	void buildFromXml();
	void saveToXml();

	bool mRightward;
};

// ----------------------------------------------

#endif

// ----------------------------------------------