//! ofxGuiGrid.cpp
/*!
*  
*
*  Created by Yishi Guo on 06/05/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/


// ----------------------------------------------

#include "ofxGuiGrid.h"

// ----------------------------------------------

ofxGuiGrid::ofxGuiGrid() {
	mParamType = kofxGui_Object_Grid;

	mWidthScale = GRID_WIDTH_SCALE;
	mHeightScale = GRID_HEIGHT_SCALE;

	mSelectedId = -1;
}

// ----------------------------------------------

void ofxGuiGrid::init( int id, string name, int x, int y, int width, int height, int xGrid, int yGrid, int border, int spacing ) {
	int textHeight = (name == "") ? 0 : mGlobals->mParamFontHeight;

	mParamId		= id;
	mParamName		= name;

	mObjX			= x;
	mObjY			= y;

	mObjWidth		= width;
	mObjHeight		= textHeight + height;

	mXGrid			= xGrid;
	mYGrid			= yGrid;

	mBorder			= border;
	mSpacing		= spacing;

	calculateWH();
	setControlRegion( 0, textHeight, width, height );

	clearSelectedColor();
}

// ----------------------------------------------

void ofxGuiGrid::setXY( int x, int y ) {
	mXGrid			= x;
	mYGrid			= y;

	calculateWH();
	setSelectedId( -1 );	// Clear the selected value
}

// ----------------------------------------------

void ofxGuiGrid::setSelectedId( int index ) {
	this->mSelectedId = index;
	clearSelectedColor();
}

// ----------------------------------------------

bool ofxGuiGrid::update( int id, int task, void* data, int length ) {
	bool handled = false;
	
	if ( id == mParamId ) {
		// TODO
		handled = true;
		mColorB += 0.1;
	}

	return handled;
}

// ----------------------------------------------

void ofxGuiGrid::draw() {
	selectedColor();

	glPushMatrix();
		glTranslatef( mObjX, mObjY, 0.0f );

		if ( mParamName != "" ) {
			drawParamString( 0.0, 0.0, mParamName, false );
		}

		ofFill();

		//! Background
		//glColor4f( mGlobals->mCoverColor.r, mGlobals->mCoverColor.g, mGlobals->mCoverColor.b, mGlobals->mCoverColor.a );

		ofNoFill();

		//! Grids
		for ( int j = 0; j < mYGrid; j++ ) {
			for ( int i = 0; i < mXGrid; i++ ) {
				int index = i + j * mXGrid;
				ofNoFill();

				if ( mSelectedId == index ) {
					drawSelectedRect( getGridX(i), getGridY(j), getGridWidth(), getGridHeight() );
					continue;
				}

				glColor4f( mGlobals->mFrameColor.r, mGlobals->mFrameColor.g, mGlobals->mFrameColor.b, mGlobals->mFrameColor.a );
				//ofRect( mCtrX + mBorder + i * mSpacing + i * mGridWidth,
				//	mCtrY + mBorder + j * mSpacing + j * mGridHeight,
				//	mGridWidth, mGridHeight );
				ofRect( getGridX(i), getGridY(j), getGridWidth(), getGridHeight() );

			}
		}

		ofNoFill();

		//! Frame
		glColor4f( mGlobals->mFrameColor.r, mGlobals->mFrameColor.g, mGlobals->mFrameColor.b, mGlobals->mFrameColor.a );
		ofRect( mCtrX, mCtrY, mCtrWidth, mCtrHeight );

	glPopMatrix();
}

// ----------------------------------------------

bool ofxGuiGrid::mouseDragged( int x, int y, int button ) {
	// TODO
	return mMouseIsDown;
}

// ----------------------------------------------

bool ofxGuiGrid::mousePressed( int x, int y, int button ) {
	ofxPoint2f inside = mouseToLocal( x, y );
	mMouseIsDown = isPointInsideMe( inside );

	if ( mMouseIsDown ) {
		int id = mouseToGridId( inside );
		
		setSelectedId( id );
	}
	return mMouseIsDown;
}

// ----------------------------------------------

bool ofxGuiGrid::mouseReleased( int x, int y, int button ) {
	// TODO
	return mMouseIsDown;
}

// ----------------------------------------------

void ofxGuiGrid::buildFromXml() {
	// TODO
	mGlobals->mListener->handleGui( mParamId, kofxGui_Set_Int, &mValue, sizeof(int) );
}

// ----------------------------------------------

void ofxGuiGrid::saveToXml() {
	// TODO
	saveObjectData();
}

// ----------------------------------------------

void ofxGuiGrid::calculateWH() {
	int w, h, unit;
	
	w = ( (mObjWidth - 2 * mBorder - (mXGrid - 1) * mSpacing ) / mXGrid ) / mWidthScale;
	h = ( (mObjHeight - 2 * mBorder - (mYGrid - 1) * mSpacing ) / mYGrid ) / mHeightScale;

	//! Get the unit length for width/height
	unit = w > h ? h : w;

	mGridWidth = unit * mWidthScale;
	mGridHeight = unit * mHeightScale;
}

// ----------------------------------------------

void ofxGuiGrid::drawSelectedRect( float x, float y, float width, float height ) {

	glShadeModel( GL_SMOOTH );
	glBegin( GL_LINE_LOOP );
		glColor4f( getColorR(), getColorG(), getColorB(), getColorA() );
		glVertex3f( x, y, 0 );
		glVertex3f( x, y + height, 0 );
		glVertex3f( x + width, y + height, 0 );
		glVertex3f( x + width, y, 0 );
	glEnd();
}

// ----------------------------------------------

int ofxGuiGrid::mouseToGridId( ofxPoint2f p ) {
	int index = 0;
	for ( int j = 0; j < mYGrid; ++j ) {
		for ( int i = 0; i < mXGrid; ++i ) {
			int index = i + j * mXGrid;
			if ( p.x >= getGridX(i) &&
				p.x <= getGridX(i) + getGridWidth() &&
				p.y >= getGridY(j) &&
				p.y <= getGridY(j) + getGridHeight() ) {
					return index;	// return the index of grid
			}
		}
	}

	return -1;	//! No id found
}

// ----------------------------------------------

float ofxGuiGrid::getGridX( int i ) {
	return mCtrX + mBorder + i * (mSpacing + mGridWidth );
}

// ----------------------------------------------

float ofxGuiGrid::getGridY( int i ) {
	return mCtrY + mBorder + i * (mSpacing + mGridHeight);
}

// ----------------------------------------------

float ofxGuiGrid::getGridWidth() {
	return this->mGridWidth;
}

// ----------------------------------------------

float ofxGuiGrid::getGridHeight() {
	return this->mGridHeight;
}

// ----------------------------------------------

void ofxGuiGrid::clearSelectedColor() {
	mColorR = 0;
	mColorG = 0;
	mColorB = 0;
	mColorA = 1;
}

// ----------------------------------------------

void ofxGuiGrid::selectedColor() {
	static unsigned long oldTime = 0;
	int interval = 50;	// ms
	float offset = 0.05;
	unsigned long now = ofGetSystemTime();
	static bool up = true;
	printf( "old: %ld\n now: %ld\n", oldTime, now );
	if ( now - oldTime >= interval ) {
		oldTime = now;
		if ( up ) {
			mColorR += offset;
			mColorG += offset;
			mColorB += offset;
		} else {
			mColorR -= offset;
			mColorG -= offset;
			mColorB -= offset;
		}

		if ( mColorR >= 1 ) {
			up = false;
		} else if ( mColorR <= 0 ) {
			up = true;
		}

		printf( "\nR: %f\n", mColorR );
	}


}

// ----------------------------------------------

float ofxGuiGrid::getColorR() {
	// TODO
	return mColorR;
}

// ----------------------------------------------

float ofxGuiGrid::getColorG() {
	// TODO
	return mColorG;
}

// ----------------------------------------------

float ofxGuiGrid::getColorB() {
	// TODO
	return mColorB;
}

// ----------------------------------------------

float ofxGuiGrid::getColorA() {
	// TODO
	return mColorA;
}

// ----------------------------------------------