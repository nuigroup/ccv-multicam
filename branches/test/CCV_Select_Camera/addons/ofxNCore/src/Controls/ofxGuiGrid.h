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
#include "MultiCams/CamsUtils.h"

// ----------------------------------------------

class ofxGuiGrid : public ofxGuiObject {
public:
	ofxGuiGrid();
	~ofxGuiGrid();

	void init( int id, string name, int x, int y, int width, int height, int xGrid, int yGrid, int border, int spacing, int mode );

	void setXY( int x, int y );
	void setSelectedId( int index );
	void setCamsUtils( CamsUtils* utils );
	void setOffset( int offset );
	void setMode( int mode, bool value = true );
	void setDraggingRawIndex( int index );
	void setImages();
	void resetAll();
	void setActive( bool active = true );

	bool next();
	bool previous();

	float getGridX( int x );
	float getGridY( int y );
	float getGridWidth();
	float getGridHeight();
	float getDraggingXOffset();
	float getDraggingYOffset();
	int getIndexOffset();

	//! Return the first image
	ofxGuiImage* getFirstImage();

	ofxGuiObject* addImage( int id, string name, int targetId, unsigned char* image );

	bool update( int id, int task, void* data, int length );
	bool update();
	void draw();

	bool mouseDragged( int x, int y, int button );
	bool mousePressed( int x, int y, int button );
	bool mouseReleased( int x, int y, int button );

	void buildFromXml();
	void saveToXml();

	int mWidthScale, mHeightScale;
	float mGridWidth, mGridHeight;
	int mXGrid, mYGrid, mBorder, mSpacing;

	int mValue, mSelectedId;
	int mTotal;
	int mDisplayMode;

	float mColorR, mColorG, mColorB, mColorA;

	ofxGuiImage** gridImages;
	CamsUtils* utils;

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

	void setTitles();

	int mIndexOffset;
	int mCamIndex;	//! index of all raw cams

	float mDraggingXOffset;
	float mDraggingYOffset;

	//! Selected color calculation
	unsigned long mOldTime;
	unsigned long mNowTime;
	int mInterval;
	float mOffset;
	bool mRising;

	////////////////
	//! Dragging
	bool mIsSelectable;
	bool mDragging;
	ofxPoint2f clickingPoint;
	bool mValidSelection;
	int mDraggingRawIndex;

	//////////////////
	bool mIsActive;
};
// ----------------------------------------------
#endif