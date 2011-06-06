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
}

// ----------------------------------------------

void ofxGuiGrid::setXY( int x, int y ) {
	mXGrid			= x;
	mYGrid			= y;

	calculateWH();
}

// ----------------------------------------------

bool ofxGuiGrid::update( int id, int task, void* data, int length ) {
	bool handled = false;
	
	if ( id == mParamId ) {
		// TODO
		handled = true;
	}

	return handled;
}

// ----------------------------------------------

void ofxGuiGrid::draw() {
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
				ofNoFill();

				glColor4f( mGlobals->mFrameColor.r, mGlobals->mFrameColor.g, mGlobals->mFrameColor.b, mGlobals->mFrameColor.a );
				ofRect( mCtrX + mBorder + i * mSpacing + i * mGridWidth,
					mCtrY + mBorder + j * mSpacing + j * mGridHeight,
					mGridWidth, mGridHeight );

			}
		}

		ofNoFill();

		//! Frame
		glColor4f( mGlobals->mFrameColor.r, mGlobals->mFrameColor.g, mGlobals->mFrameColor.b, mGlobals->mFrameColor.a );
		ofRect( mCtrX, mCtrY, mObjWidth, mObjHeight );

	glPopMatrix();
}

// ----------------------------------------------

bool ofxGuiGrid::mouseDragged( int x, int y, int button ) {
	// TODO
	return mMouseIsDown;
}

// ----------------------------------------------

bool ofxGuiGrid::mousePressed( int x, int y, int button ) {
	// TODO
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