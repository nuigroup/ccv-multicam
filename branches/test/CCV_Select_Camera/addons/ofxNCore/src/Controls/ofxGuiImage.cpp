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
	pCam = NULL;
	pCvImage = new ofxCvGrayscaleImage();

	bDrawInfo =false;
	bCanDrawInfo = false;

	bCamSelected = false;

	strSelectedText = "Selected!";
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

	setControlRegion( 0, 0, width, height );
}

// ----------------------------------------------

void ofxGuiImage::setImage( unsigned char* pImage, int width, int height ) {
	if ( pImage != NULL ) {
		//! DEBUG
		//printf( "\nofxGuiImage::setImage()\t pImage != NULL\n" );
		this->pImage = pImage;
		bImageSet = true;
		mCamWidth = width;
		mCamHeight = height;

		//cout << "ofxGuiImage::setImage()" << endl;

		//if ( pCvImage != NULL ) {	//! If exist, clear it!
		//	pCvImage->clear();
		//}
		if ( !pCvImage->bAllocated ) {
			pCvImage->allocate( mCamWidth, mCamHeight );
		}
		pCvImage->setFromPixels( this->pImage, this->mCamWidth, this->mCamHeight );
		pCvImage->setUseTexture( false );
	}
	//printf( "ofxGuiImage::setImage()\t out of if - p = %p\n", pImage );
}

// ----------------------------------------------

void ofxGuiImage::setBlank( bool bBlank ) {
	if ( bBlank ) {
		bImageSet = false;
		setCanDrawInfo( false );
	}
}

// ----------------------------------------------

void ofxGuiImage::setCamera( PS3* cam ) {
	if ( cam != NULL ) {
		//! DEBUG
		printf( "\nofxGuiImage::setCamera()\tcam = %p\n", cam );

		//printf( "pCam = %p\n", pCam );
		this->pCam = cam;
		//printf( "pCam!!!\n" );
		//printf( "pCam = %p\n", pCam );
		//cam->PrintInfo();
		setImage( pCam->GetPixels(), pCam->GetWidth(), pCam->GetHeight() );
		//printf( "pCam = %p\n", pCam );

		// cam->PrintInfo();
		setCanDrawInfo( true );
	} else {
		setBlank( true );	//! camera is NULL so we show blank image.
	}
}

// ----------------------------------------------

void ofxGuiImage::setTitle( string name ) {
	mParamName = name;
}

// ----------------------------------------------

void ofxGuiImage::setXYPos( int x, int y ) {
	this->mObjX = x;
	this->mObjY = y;
}

// ----------------------------------------------

void ofxGuiImage::setDrawInfo( bool draw ) {
	bDrawInfo = draw;
}

// ----------------------------------------------

void ofxGuiImage::setCamSelected( bool selected ) {
	bCamSelected = selected;
}

// ----------------------------------------------

PS3* ofxGuiImage::getCamera() {
	return pCam;
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
			//! draw the shadow
			color.r = 0.0f;	color.g = 0.0f;	color.b = 0.0f;	color.a = 1.0f;
			drawString( PARAM_TEXT_OFFSET_X + 1, 1.0f, mParamName, false, color );

			//! draw the text
			color.r = 1.0f;	color.g = 1.0f;	color.b = 1.0f;	color.a = 1.0f;
			drawString( PARAM_TEXT_OFFSET_X, 0.0f, mParamName, false, color );
		}

		if ( bDrawInfo && bCanDrawInfo && pCam != NULL ) {
			drawInfo();
		}

		if ( bCamSelected ) {
			drawSelectedText();
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
	if ( bImageSet ) {
		if ( pCam != NULL ) {
			setImage( pCam->GetPixels(), pCam->GetWidth(), pCam->GetHeight() );
		}
		if ( pCvImage != NULL ) {
			pCvImage->draw( mCtrX, mCtrY, mCtrWidth, mCtrHeight );
		}
	}
}

// ----------------------------------------------

void ofxGuiImage::drawString( int x, int y, string str, bool center, ofRGBA color ) {
	glColor4f( color.r, color.g, color.b, color.a );

	if ( center ) {
		x -= roundInt( mGlobals->mParamFont.stringWidth( str ) / 2.0f );
	} else {
		x += mGlobals->mParamFontXOffset;
	}

	y += mGlobals->mParamFontYOffset;
	mGlobals->mParamFont.drawString( str, x, y );
}

// ----------------------------------------------

void ofxGuiImage::drawInfo() {
	int i = 0;
	string info = "foo";
	int textHeight = mGlobals->mParamFont.stringHeight( info );
	if ( pCam != NULL ) {
		info += "\nFPS: ";
		info += ofToString( pCam->GetFPS() );
		info += "\nmeiyou.org\nmeiyou.oeg\nmeiyou.org\nType: ";
		info += "PS3";	// TODO
		info += "\nGUID: ";
		info += PS3::GUID2String( pCam->GetGUID(), '_' );
	}

	//! Display order: Bottom -> Top
	drawHighlightParamString( PARAM_TEXT_OFFSET_X, mCtrHeight - PARAM_TEXT_OFFSET_X - ++i * textHeight, "GUID: " + PS3::GUID2String( pCam->GetGUID(), '_' ), false );
	drawHighlightParamString( PARAM_TEXT_OFFSET_X, mCtrHeight - PARAM_TEXT_OFFSET_X - ++i * textHeight, "Res: " + ofToString( pCam->GetWidth() ) + "x" + ofToString( pCam->GetHeight() ), false );
	drawHighlightParamString( PARAM_TEXT_OFFSET_X, mCtrHeight - PARAM_TEXT_OFFSET_X - ++i * textHeight, "Type: PS3", false );
	drawHighlightParamString( PARAM_TEXT_OFFSET_X, mCtrHeight - PARAM_TEXT_OFFSET_X - ++i * textHeight, "FPS: " + ofToString( pCam->GetFPS() ), false );

	////int x = mCtrWidth - textWidth;
	////int y = 0;


	////! draw shadow
	//color.r = .0f;	color.g = .0f;	color.b = .0f;	color.a = 1.0f;
	//drawString( PARAM_TEXT_OFFSET_X + 1, roundInt(mCtrHeight - textHeight - PARAM_TEXT_OFFSET_X + 1), info, false, color );

	////! draw text
	//color.r = 1.0f;	color.g = 1.0f;	color.b = 1.0f;	color.a = 1.0f;
	//drawString( PARAM_TEXT_OFFSET_X + 0, roundInt(mCtrHeight - textHeight - PARAM_TEXT_OFFSET_X + 0), info, false, color );
}

// ----------------------------------------------

void ofxGuiImage::drawSelectedText() {
	int textWidth = mGlobals->mParamFont.stringWidth( strSelectedText );
	int textHeight = mGlobals->mParamFont.stringHeight( strSelectedText );

	//! Draw shadow
	color.r = .0f;	color.g = .0f;	color.b = .0f;	color.a = 1.0f;
	drawString( (mCtrWidth - textWidth) / 2 + 1, (mCtrHeight - textHeight) / 2 + 1, strSelectedText, false, color );

	//! Draw red text
	color.r = 1.0f;	color.g = .0f;	color.b = .0f;	color.a = 1.0f;
	drawString( (mCtrWidth - textWidth) / 2, (mCtrHeight - textHeight) / 2, strSelectedText, false, color );
}

// ----------------------------------------------

void ofxGuiImage::setCanDrawInfo( bool draw ) {
	bCanDrawInfo = draw;
}

// ----------------------------------------------