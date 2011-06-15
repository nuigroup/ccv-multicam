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

	mIndexOffset = 0;

	mOldTime = ofGetSystemTime();
	mInterval = 50;		// 50 ms
	mOffset = 0.05f;	// color changing offset
	mRising = true;

}

// ----------------------------------------------

void ofxGuiGrid::init( int id, string name, int x, int y, int width, int height, int xGrid, int yGrid, int border, int spacing, int mode ) {
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

	mDisplayMode	= mode;

	calculateWH();
	setControlRegion( 0, textHeight, width, height );

	clearSelectedColor();

	createImages();
}

// ----------------------------------------------

void ofxGuiGrid::setXY( int x, int y ) {
	//! Just display mode can be set X/Y
	if ( mDisplayMode == kofxGui_Grid_Display ) {
		clearImages();

		mXGrid			= x;
		mYGrid			= y;

		calculateWH();
		setSelectedId( -1 );	// Clear the selected value

		createImages();
	}
}

// ----------------------------------------------

void ofxGuiGrid::setSelectedId( int index ) {
	this->mSelectedId = index;
	clearSelectedColor();

	this->mCamIndex = this->mSelectedId + this->mIndexOffset;
}

// ----------------------------------------------

void ofxGuiGrid::setCamsUtils( CamsUtils* utils ) {
	if ( utils == NULL ) {
		return;
	}

	this->utils = utils;

	if ( mDisplayMode == kofxGui_Grid_List ) {
		setImages();
	}
}

// ----------------------------------------------

void ofxGuiGrid::setOffset( int offset ) {
	this->mIndexOffset = offset;
	this->setTitles();
}

// ----------------------------------------------

bool ofxGuiGrid::next() {
	if ( mDisplayMode == kofxGui_Grid_List ) {
		if ( mXGrid * mYGrid + mIndexOffset + 1 <= utils->getCount() ) {
			mIndexOffset++;
			setTitles();
			return true;
		}
	}

	return false;
}

// ----------------------------------------------

bool ofxGuiGrid::previous() {
	if (mDisplayMode == kofxGui_Grid_List ) {
		if ( mIndexOffset - 1 >= 0 ) {
			mIndexOffset--;
			setTitles();
			return true;
		}
	}

	return false;
}

// -------------------------------------------   ---

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

bool ofxGuiGrid::update() {
	this->setImages();
	return true;
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
				} else {
					glColor4f( mGlobals->mFrameColor.r, mGlobals->mFrameColor.g, mGlobals->mFrameColor.b, mGlobals->mFrameColor.a );
					//ofRect( mCtrX + mBorder + i * mSpacing + i * mGridWidth,
					//	mCtrY + mBorder + j * mSpacing + j * mGridHeight,
					//	mGridWidth, mGridHeight );
					ofRect( getGridX(i), getGridY(j), getGridWidth(), getGridHeight() );
				}

				if ( gridImages[index] != NULL ) {
					gridImages[index]->draw();
				}

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
		mGlobals->mListener->handleGui( mParamId, kofxGui_Set_Int, &mCamIndex, sizeof(int) );
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
	float w, h, unit;
	
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
	mNowTime = ofGetSystemTime();
	//printf( "old: %ld\n now: %ld\n", oldTime, now );
	if ( mNowTime - mOldTime >= mInterval ) {
		mOldTime = mNowTime;
		if ( mRising ) {
			mColorR += mOffset;
			mColorG += mOffset;
			mColorB += mOffset;
		} else {
			mColorR -= mOffset;
			mColorG -= mOffset;
			mColorB -= mOffset;
		}

		if ( mColorR >= 1 ) {
			mRising = false;
		} else if ( mColorR <= 0 ) {
			mRising = true;
		}

		// printf( "\nR: %f\n", mColorR );
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

void ofxGuiGrid::clearImages() {
	//! Clear all images
	for ( int i = 0; i < mXGrid * mYGrid; ++i ) {
		delete gridImages[i];
	}
}

// ----------------------------------------------

void ofxGuiGrid::createImages() {
	//! Allocate the space
	gridImages = new ofxGuiImage*[mXGrid*mYGrid];

	int index = 0;

	//! Setup each image
	for ( int j = 0; j < mYGrid; ++j ) {
		for ( int i = 0; i < mXGrid; ++i ) {
			index = i + j * mXGrid;
			gridImages[index] = new ofxGuiImage();

			gridImages[index]->init( CAMERAS_ID_OFFSET + index, "Cam " + ofToString(index), getGridX(i) + 1, getGridY(j) + 1, roundInt(mGridWidth - 2), roundInt(mGridHeight - 2)  );

		}
	}
}

// ----------------------------------------------

void ofxGuiGrid::setImages() {
	//! Only for list mode
	if ( mDisplayMode == kofxGui_Grid_List ) {
		for ( int i = 0; i < mXGrid * mYGrid; ++i ) {
			if ( i + mIndexOffset < utils->getCount() ) {
				//! DEBUG
				//printf( "\nofxGuiGrid:setImages()\t i < utils->getCount()\n" );
				try {
					gridImages[i]->setCamera( utils->getRawCam( i + mIndexOffset ) );
				} catch( ... ) {
					// TODO
				}
				//PS3* cam = utils->getRawCam(i);
			} else {
				gridImages[i]->setBlank();
			}
		}
	}
}

// ----------------------------------------------

void ofxGuiGrid::setTitles() {
	for ( int i = 0; i < mXGrid * mYGrid; ++i ) {
		gridImages[i]->setTitle( "Cam " + ofToString( i + mIndexOffset ) );
	}
}

// ----------------------------------------------