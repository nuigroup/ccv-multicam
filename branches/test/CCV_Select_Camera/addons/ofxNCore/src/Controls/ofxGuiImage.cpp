//! ofxGuiImage.cpp
/*!
*  
*
*  Created by Yishi Guo on 06/10/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

// ----------------------------------------------

#include "ofxGuiImage.h"

// ----------------------------------------------

ofxGuiImage::ofxGuiImage() {
	// TODO
	bImageSet = false;
}

// ----------------------------------------------

ofxGuiImage::~ofxGuiImage() {
	// TODO
}

// ----------------------------------------------

void ofxGuiImage::init( int id, string name, int x, int y, int width, int height, unsigned char* pImage ) {

	mParamId			= id;
	mParamName			= name;

	mObjX				= x;
	mObjY				= y;

	mObjWidth			= width;
	mObjHeight			= height;

	setImage( pImage );
	setControlRegion( 0, 0, width, height );

}

// ----------------------------------------------

void ofxGuiImage::setImage( unsigned char* pImage ) {
	if ( pImage != NULL ) {
		this->pImage = pImage;
		bImageSet = true;
	}
}

// ----------------------------------------------

void ofxGuiImage::setBlank( bool bBlank ) {
	if ( bBlank ) {
		bImageSet = false;
	}
}

// ----------------------------------------------

bool ofxGuiImage::update( int id, int task, void* data, int length ) {
	// TODO
	bool handled = false;

	if ( id == mParamId ) {
		handled = true;
	}

	return handled;
}

// ----------------------------------------------

void ofxGuiImage::draw() {
	// TODO
	glPushMatrix();
		glTranslatef( mObjX, mObjY, 0.0f );

		ofFill();

		if ( bImageSet ) {
			drawImage();
		} else {
			drawBlank();
		}

		if ( mParamName != "" ) {
			drawParamString( PARAM_TEXT_OFFSET_X, 0.0f, mParamName, false );
		}
	glPopMatrix();
}

// ----------------------------------------------

bool ofxGuiImage::mouseDragged( int x, int y, int button ) {
	// TODO
	return mMouseIsDown;
}

// ----------------------------------------------

bool ofxGuiImage::mousePressed( int x, int y, int button ) {
	// TODO
	return mMouseIsDown;
}

// ----------------------------------------------

bool ofxGuiImage::mouseReleased( int x, int y, int button ) {
	// TODO
	return mMouseIsDown;
}

// ----------------------------------------------

void ofxGuiImage::buildFromXml() {
	// TODO
}

// ----------------------------------------------

void ofxGuiImage::saveToXml() {
	// TODO
	saveObjectData();
}

// ----------------------------------------------

void ofxGuiImage::drawBlank() {
	// blank image
	ofFill();

	glColor4f( 0.5f, 0.5f, 0.5f, 1 );
	ofRect( mCtrX, mCtrY, mCtrWidth, mCtrHeight );
}

// ----------------------------------------------

void ofxGuiImage::drawImage() {
	// TODO
}

// ----------------------------------------------