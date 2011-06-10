//! ofxGuiGrid.h
/*!
*  
*
*  Created by Yishi Guo on 06/05/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

// ----------------------------------------------

#ifndef OFX_GUI_GRID
#define OFX_GUI_GRID

// ----------------------------------------------

#define GRID_WIDTH_SCALE 4
#define GRID_HEIGHT_SCALE 3
#define CAMERAS_ID_OFFSET 1000

// ----------------------------------------------

#include "ofxGuiTypes.h"
#include "ofxGuiObject.h"
#include "ofxGuiImage.h"

// ----------------------------------------------

class ofxGuiGrid : public ofxGuiObject {
public:
	ofxGuiGrid();
	~ofxGuiGrid();

	void init( int id, string name, int x, int y, int width, int height, int xGrid, int yGrid, int border, int spacing );

	void setXY( int x, int y );
	void setSelectedId( int index );

	float getGridX( int x );
	float getGridY( int y );
	float getGridWidth();
	float getGridHeight();

	ofxGuiObject* addImage( int id, string name, int targetId, unsigned char* image );

	bool update( int id, int task, void* data, int length );
	void draw();

	bool mouseDragged( int x, int y, int button );
	bool mousePressed( int x, int y, int button );
	bool mouseReleased( int x, int y, int button );

	void buildFromXml();
	void saveToXml();

	int mWidthScale, mHeightScale;
	int mGridWidth, mGridHeight;
	int mXGrid, mYGrid, mBorder, mSpacing;

	int mValue, mSelectedId;

	float mColorR, mColorG, mColorB, mColorA;

	ofxGuiImage** gridImages;

private:
	void calculateWH();

	int  mouseToGridId( ofxPoint2f point );

	void drawSelectedRect( float x, float y, float width, float height );

	void clearSelectedColor();
	void selectedColor();
	float getColorR();
	float getColorG();
	float getColorB();
	float getColorA();

	void clearImages();
	void createImages();
	
};
// ----------------------------------------------
#endif