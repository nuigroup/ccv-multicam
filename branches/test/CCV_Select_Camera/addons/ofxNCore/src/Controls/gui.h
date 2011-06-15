/*
*  GUI.h
*
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#ifndef GUI_CONTROLS_H
#define GUI_CONTROLS_H

#include "../Modules/ofxNCoreVision.h"
#include "../Camera/CameraUtils.h"

ofxNCoreVision  *appPtr;
CameraUtils		cameraCount;

void ofxNCoreVision::setupControls()
{
	appPtr = this;

	//panel border color
	controls->mGlobals->mBorderColor.r = 0;
	controls->mGlobals->mBorderColor.g = 0;
	controls->mGlobals->mBorderColor.b = 0;
	controls->mGlobals->mBorderColor.a = .3;
	//panel color
	controls->mGlobals->mCoverColor.r = 1;
	controls->mGlobals->mCoverColor.g = 1;
	controls->mGlobals->mCoverColor.b = 1;
	controls->mGlobals->mCoverColor.a = .4;
	//control outline color
	controls->mGlobals->mFrameColor.r = 0;
	controls->mGlobals->mFrameColor.g = 0;
	controls->mGlobals->mFrameColor.b = 0;
	controls->mGlobals->mFrameColor.a = .3;
	//text color
	controls->mGlobals->mTextColor.r = 0;
	controls->mGlobals->mTextColor.g = 0;
	controls->mGlobals->mTextColor.b = 0;
	controls->mGlobals->mTextColor.a = 1;
	//button color
	controls->mGlobals->mButtonColor.r = 1;
	controls->mGlobals->mButtonColor.g = 0;
	controls->mGlobals->mButtonColor.b = 0;
	controls->mGlobals->mButtonColor.a = .8;
	//slider tip color
	controls->mGlobals->mHandleColor.r = 0;
	controls->mGlobals->mHandleColor.g = 0;
	controls->mGlobals->mHandleColor.b = 0;
	//slider color
	controls->mGlobals->mSliderColor.r = 1;
	controls->mGlobals->mSliderColor.g = 0;
	controls->mGlobals->mSliderColor.b = 0;
	controls->mGlobals->mSliderColor.a = .8;

	this->addPanels();

	//do update while inactive
	controls->forceUpdate(false);
	controls->activate(true);

	/****************************
	* Set GUI values on startup
	****************************/
	controls->update(appPtr->propertiesPanel_flipV, kofxGui_Set_Bool, &appPtr->filter->bVerticalMirror, sizeof(bool));
	controls->update(appPtr->propertiesPanel_flipH, kofxGui_Set_Bool, &appPtr->filter->bHorizontalMirror, sizeof(bool));
	controls->update(appPtr->trackedPanel_outlines, kofxGui_Set_Bool, &appPtr->bDrawOutlines, sizeof(bool));
	controls->update(appPtr->trackedPanel_ids, kofxGui_Set_Bool, &appPtr->bShowLabels, sizeof(bool));
	controls->update(appPtr->trackedPanel_darkblobs, kofxGui_Set_Bool, &appPtr->filter->bTrackDark, sizeof(bool));
	//Source
	controls->update(appPtr->sourcePanel_cam, kofxGui_Set_Bool, &appPtr->bcamera, sizeof(bool));
	//Calibration
	controls->update(appPtr->calibrationPanel_calibrate, kofxGui_Set_Bool, &appPtr->bCalibration, sizeof(bool));
	//Dynamic Background
	controls->update(appPtr->backgroundPanel_dynamic, kofxGui_Set_Bool, &appPtr->filter->bDynamicBG, sizeof(bool));
	//Smooth
	controls->update(appPtr->smoothPanel_use, kofxGui_Set_Bool, &appPtr->filter->bSmooth, sizeof(bool));
	controls->update(appPtr->smoothPanel_smooth, kofxGui_Set_Bool, &appPtr->filter->smooth, sizeof(float));
	//Highpass
	controls->update(appPtr->highpassPanel_use, kofxGui_Set_Bool, &appPtr->filter->bHighpass, sizeof(bool));
	controls->update(appPtr->highpassPanel_blur, kofxGui_Set_Bool, &appPtr->filter->highpassBlur, sizeof(float));
	controls->update(appPtr->highpassPanel_noise, kofxGui_Set_Bool, &appPtr->filter->highpassNoise, sizeof(float));
	//Amplify
	controls->update(appPtr->amplifyPanel_use, kofxGui_Set_Bool, &appPtr->filter->bAmplify, sizeof(bool));
	controls->update(appPtr->amplifyPanel_amp, kofxGui_Set_Bool, &appPtr->filter->highpassAmp, sizeof(float));
	//Threshold
	controls->update(appPtr->trackedPanel_threshold, kofxGui_Set_Bool, &appPtr->filter->threshold, sizeof(float));
	//Min Movement
	controls->update(appPtr->trackedPanel_min_movement, kofxGui_Set_Bool, &appPtr->tracker.MOVEMENT_FILTERING, sizeof(float));
	//Min Blob Size
	controls->update(appPtr->trackedPanel_min_blob_size, kofxGui_Set_Bool, &appPtr->MIN_BLOB_SIZE, sizeof(float));
	//Max Blob Size
	controls->update(appPtr->trackedPanel_max_blob_size, kofxGui_Set_Bool, &appPtr->MAX_BLOB_SIZE, sizeof(float));
	//Template Area
	controls->update(appPtr->TemplatePanel_minArea, kofxGui_Set_Bool, &appPtr->minTempArea, sizeof(float));
	controls->update(appPtr->TemplatePanel_maxArea, kofxGui_Set_Bool, &appPtr->maxTempArea, sizeof(float));
	//Background Learn Rate
	controls->update(appPtr->backgroundPanel_learn_rate, kofxGui_Set_Bool, &appPtr->backgroundLearnRate, sizeof(float));
	//Track Panel
	controls->update(appPtr->trackingPanel_trackFingers, kofxGui_Set_Bool, &appPtr->contourFinder.bTrackFingers, sizeof(bool));
	controls->update(appPtr->trackingPanel_trackObjects, kofxGui_Set_Bool, &appPtr->contourFinder.bTrackObjects, sizeof(bool));
	controls->update(appPtr->trackingPanel_trackFiducials, kofxGui_Set_Bool, &appPtr->contourFinder.bTrackFiducials, sizeof(bool));
	//Send TUIO
	controls->update(appPtr->optionPanel_tuio_osc, kofxGui_Set_Bool, &appPtr->myTUIO.bOSCMode, sizeof(bool));
	controls->update(appPtr->optionPanel_tuio_tcp, kofxGui_Set_Bool, &appPtr->myTUIO.bTCPMode, sizeof(bool));
	controls->update(appPtr->optionPanel_bin_tcp, kofxGui_Set_Bool, &appPtr->myTUIO.bBinaryMode, sizeof(bool));
	//TUIO Height Width
//	controls->update(appPtr->optionPanel_tuio_height_width, kofxGui_Set_Bool, &appPtr->myTUIO.bHeightWidth, sizeof(bool));
}

void ofxNCoreVision ::handleGui(int parameterId, int task, void* data, int length)
{
	printf( "DEBUG: foxNCoreVision::handleGui\n" );
	if ( bMultiCamsInterface ) {
		//! Let MultiCams do the multiple cameras work
		multiCams->handleGui( parameterId, task, data, length );
	} else {
		switch(parameterId)
		{
		case sourcePanel_cam:
			if(length == sizeof(bool))
			{
				if(*(bool*)data)
				{
					if( vidPlayer != NULL ) {
						vidPlayer->close();
					}
					if( vidGrabber != NULL ) {
						vidGrabber->listDevices();
						vidGrabber->setDeviceID(deviceID);
						vidGrabber->setVerbose(false);
						vidGrabber->initGrabber(camWidth,camHeight);
						filter->exposureStartTime = ofGetElapsedTimeMillis();
						filter_fiducial->exposureStartTime = ofGetElapsedTimeMillis();
					}

					bcamera = true;
					//reset gpu textures and filters
					if(!cameraInited)
					{
						initDevice();
					}
					processedImg.clear();
					processedImg.allocate(camWidth, camHeight); //Processed Image
					processedImg.setUseTexture(false);

					processedImg_fiducial.clear();
					processedImg_fiducial.allocate(camWidth, camHeight);
					processedImg_fiducial.setUseTexture(false);

					sourceImg.clear();
					sourceImg.allocate(camWidth, camHeight);    //Source Image
					sourceImg.setUseTexture(false);
					filter->allocate(camWidth, camHeight);
					filter_fiducial->allocate(camWidth, camHeight);
				}
				else
				{
					bcamera = false;

					if( vidPlayer == NULL ) {
						vidPlayer = new ofVideoPlayer();
					}
					if( vidGrabber != NULL ) {
						vidGrabber->close();
					}

					vidPlayer->loadMovie(videoFileName);
					vidPlayer->play();
					vidPlayer->setLoopState(OF_LOOP_NORMAL);
					filter->exposureStartTime = ofGetElapsedTimeMillis();
					filter_fiducial->exposureStartTime = ofGetElapsedTimeMillis();
					printf("Video Mode\n");

					bool bReallocate = true;
					if(camWidth == vidPlayer->width && camHeight == vidPlayer->height)
					{
						bReallocate = false;
					}

					camHeight = vidPlayer->height;
					camWidth = vidPlayer->width;

					if(bReallocate){
						//reset gpu textures and filters
						processedImg.clear();
						processedImg.allocate(camWidth, camHeight); //Processed Image
						processedImg.setUseTexture(false);
						processedImg_fiducial.clear();
						processedImg_fiducial.allocate(camWidth, camHeight);
						processedImg_fiducial.setUseTexture(false);
						sourceImg.clear();
						sourceImg.allocate(camWidth, camHeight);    //Source Image
						sourceImg.setUseTexture(false);
						filter->allocate(camWidth, camHeight);
						filter_fiducial->allocate(camWidth, camHeight);
					}
				}
			}
			break;
		case sourcePanel_nextCam:
			if(length == sizeof(bool))
			{
				if(*(bool*)data)
				{
					if(bcamera && vidGrabber != NULL){
						deviceID += 1;
						vidGrabber->close();
						vidGrabber->setDeviceID(deviceID);
						vidGrabber->setVerbose(false);
						vidGrabber->initGrabber(camWidth,camHeight);
						filter->exposureStartTime = ofGetElapsedTimeMillis();
						filter_fiducial->exposureStartTime = ofGetElapsedTimeMillis();
					}
				}
			}
			break;
		case sourcePanel_previousCam:
			if(length == sizeof(bool))
			{
				if(*(bool*)data)
				{
					if(bcamera && vidGrabber != NULL)
					{
						deviceID -= 1;
						if(deviceID < 0) {
							deviceID = 0;
						}
						else{
							vidGrabber->close();
							vidGrabber->setDeviceID(deviceID);
							vidGrabber->setVerbose(false);
							vidGrabber->initGrabber(camWidth,camHeight);
							filter->exposureStartTime = ofGetElapsedTimeMillis();
							filter_fiducial->exposureStartTime = ofGetElapsedTimeMillis();
						}
					}
				}
			}
			break;
			/////////////////////////////////////////////
			// MultiCams Settings
		case propertiesPanel_settings:
			printf( "properitesPanel_settings\n" );
			if(length == sizeof(bool))
			{
				if(*(bool*)data/* && bcamera*/)
				{
					//              #ifdef TARGET_WIN32
					//if(PS3)
					//	PS3->showSettings();
					//else if(vidGrabber)
					//	vidGrabber->videoSettings();
					//               #else
					//               	vidGrabber->videoSettings();
					//               #endif

					//MultiCams cams = new MultiCams();
					switchMultiCamsGUI();
					printf ("data value: %d\n", *(bool*)data );
				}
			}
			break;
			//Calibration
		case calibrationPanel_calibrate:
			bShowInterface = false;
			//Enter/Exit Calibration
			bCalibration = true;
			calib.calibrating = true;
			tracker.isCalibrating = true;
			if (bFullscreen == false) ofToggleFullscreen();
			bFullscreen = true;
			break;
			//Source
		case propertiesPanel_flipH:
			printf( "propertiesPanel_flipH\n" );
			if(length == sizeof(bool))
				filter->bHorizontalMirror = *(bool*)data;
			filter_fiducial->bHorizontalMirror = *(bool*)data;
			break;
		case propertiesPanel_flipV:
			if(length == sizeof(bool))
				filter->bVerticalMirror = *(bool*)data;
			filter_fiducial->bVerticalMirror = *(bool*)data;
			break;

			//Tracking Panel
		case trackingPanel_trackFingers:
			if(length == sizeof(bool))
				contourFinder.bTrackFingers=*(bool*)data;
			break;
		case trackingPanel_trackObjects:
			if(length == sizeof(bool))
			{
				contourFinder.bTrackObjects=*(bool*)data;
				if(contourFinder.bTrackObjects)
				{
					templates.loadTemplateXml();
				}
				else
				{
					templates.saveTemplateXml();
				}
			}
			break;
		case trackingPanel_trackFiducials:
			if(length == sizeof(bool))
			{
				contourFinder.bTrackFiducials=*(bool*)data;
			}
			break;
			//Communication
		case optionPanel_tuio_osc:
			if(length == sizeof(bool))
				myTUIO.bOSCMode = *(bool*)data;
			//	bTUIOMode = *(bool*)data;
			//set tcp to opposite
			//	myTUIO.bTCPMode = false;
			//	controls->update(appPtr->optionPanel_tuio_tcp, kofxGui_Set_Bool, &appPtr->myTUIO.bTCPMode, sizeof(bool));
			//	myTUIO.bBinaryMode = false;
			//	controls->update(appPtr->optionPanel_bin_tcp, kofxGui_Set_Bool, &appPtr->myTUIO.bBinaryMode, sizeof(bool));
			//clear blobs
			//				myTUIO.blobs.clear();
			break;
		case optionPanel_tuio_tcp:
			if(length == sizeof(bool))
				myTUIO.bTCPMode = *(bool*)data;
			//	bTUIOMode = *(bool*)data;
			//set osc to opposite
			//	myTUIO.bOSCMode = false;
			//	controls->update(appPtr->optionPanel_tuio_osc, kofxGui_Set_Bool, &appPtr->myTUIO.bOSCMode, sizeof(bool));
			//	myTUIO.bBinaryMode = false;
			//	controls->update(appPtr->optionPanel_bin_tcp, kofxGui_Set_Bool, &appPtr->myTUIO.bBinaryMode, sizeof(bool));
			//clear blobs
			//				myTUIO.blobs.clear();
			break;
		case optionPanel_bin_tcp:
			if(length == sizeof(bool))
				myTUIO.bBinaryMode = *(bool*)data;
			bTUIOMode = *(bool*)data;
			//set tcp & osc to opposite
			myTUIO.bTCPMode = false;
			controls->update(appPtr->optionPanel_tuio_tcp, kofxGui_Set_Bool, &appPtr->myTUIO.bTCPMode, sizeof(bool));
			myTUIO.bOSCMode = false;
			controls->update(appPtr->optionPanel_tuio_osc, kofxGui_Set_Bool, &appPtr->myTUIO.bOSCMode, sizeof(bool));
			//clear blobs
			//				myTUIO.blobs.clear();
			break;
			// 		case optionPanel_tuio_height_width:
			// 			if(length == sizeof(bool))
			// 				myTUIO.bHeightWidth = *(bool*)data;
			// 			break;
			//Background
		case backgroundPanel_dynamic:
			if(length == sizeof(bool))
				filter->bDynamicBG = *(bool*)data;
			filter_fiducial->bDynamicBG = *(bool*)data;
			break;
		case backgroundPanel_remove:
			if(length == sizeof(bool))
				filter->bLearnBakground = *(bool*)data;
			filter_fiducial->bLearnBakground = *(bool*)data;
			break;
		case backgroundPanel_learn_rate:
			if(length == sizeof(float))
				backgroundLearnRate = *(float*)data;
			break;
			//Highpass
		case highpassPanel_use:
			if(length == sizeof(bool))
			{
				if(!bFidMode)
				{
					filter->bHighpass = *(bool*)data;
				}
				else
				{
					filter_fiducial->bHighpass = *(bool*)data;
				}
			}
			break;
		case highpassPanel_blur:
			if(length == sizeof(float))
			{
				if(!bFidMode)
				{
					filter->highpassBlur = *(float*)data;
				}
				else
				{
					filter_fiducial->highpassBlur = *(float*)data;
				}
			}
			break;
		case highpassPanel_noise:
			if(length == sizeof(float))
			{
				if(!bFidMode)
				{
					filter->highpassNoise = *(float*)data;
				}
				else
				{
					filter_fiducial->highpassNoise = *(float*)data;
				}
			}
			break;
			//Amplify
		case amplifyPanel_use:
			if(length == sizeof(bool))
			{
				if(!bFidMode)
				{
					filter->bAmplify = *(bool*)data;
				}
				else
				{
					filter_fiducial->bAmplify = *(bool*)data;
				}
			}
			break;
		case amplifyPanel_amp:
			if(length == sizeof(float))
			{
				if(!bFidMode)
				{
					filter->highpassAmp = *(float*)data;
				}
				else
				{
					filter_fiducial->highpassAmp = *(float*)data;
				}
			}
			break;
		case trackedPanel_darkblobs:
			if(length == sizeof(bool))
			{
				if(!bFidMode)
				{
					filter->bTrackDark = *(bool*)data;
				}
				else
				{
					filter_fiducial->bTrackDark = *(bool*)data;
				}
			}
			break;
		case trackedPanel_threshold:
			if(length == sizeof(float))
			{
				if(!bFidMode)
				{
					filter->threshold = *(float*)data;
				}
				else
				{
					filter_fiducial->threshold = *(float*)data;
				}
			}
			break;
		case trackedPanel_min_movement:
			if(length == sizeof(float))
				tracker.MOVEMENT_FILTERING = *(float*)data;
			break;
		case trackedPanel_min_blob_size:
			if(length == sizeof(float))
				MIN_BLOB_SIZE = *(float*)data;
			break;
		case trackedPanel_max_blob_size:
			if(length == sizeof(float))
				MAX_BLOB_SIZE = *(float*)data;
			break;
		case trackedPanel_outlines:
			if(length == sizeof(bool))
				bDrawOutlines = *(bool*)data;
			break;
		case trackedPanel_ids:
			if(length == sizeof(bool))
				bShowLabels = *(bool*)data;
			break;
			//smooth
		case smoothPanel_smooth:
			if(length == sizeof(float))
			{
				if(!bFidMode)
				{
					filter->smooth = *(float*)data;
				}
				else
				{
					filter_fiducial->smooth = *(float*)data;
				}
			}
			break;
		case smoothPanel_use:
			if(length == sizeof(bool))
			{
				if(!bFidMode)
				{
					filter->bSmooth = *(bool*)data;
				}
				else
				{
					filter_fiducial->bSmooth = *(bool*)data;
				}
			}
			break;
			//Template Area Sliders
		case TemplatePanel_minArea:
			if(length == sizeof(float))
			{
				minTempArea = *(float*)data;
				float smallArea = rect.height*rect.width-minTempArea; //The area of the small rectangle
				float _w = sqrt(smallArea*rect.width/rect.height); // Width of small rectangle, as the width and height
				//will be proportional to the original rectangle
				float _h = sqrt(smallArea*rect.height/rect.width);
				minRect.x =rect.x + (rect.width - _w)/2 ;
				minRect.y = rect.y + (rect.height - _h)/2 ;
				minRect.width = _w ;
				minRect.height = _h ;
			}
			break;
		case TemplatePanel_maxArea:
			if(length == sizeof(float))
			{
				maxTempArea = *(float*)data;
				float bigArea = rect.height*rect.width+maxTempArea; //The area of the big rectangle
				float _w = sqrt(bigArea*rect.width/rect.height); // Width of big rectangle, as the width and height
				//will be proportional to the original rectangle
				float _h = sqrt(bigArea*rect.height/rect.width);
				maxRect.x =rect.x - (_w - rect.width)/2 ;
				maxRect.y = rect.y - (_h - rect.height)/2 ;
				maxRect.width = _w ;
				maxRect.height = _h ;
			}
			break;
			//Save Settings
		case kParameter_SaveXml:
			if(length == sizeof(bool))
			{
				if(*(bool*)data)
				{
					controls->saveToXml(OFXGUI_XML);
					saveSettings();
				}
			}
			break;
		case kParameter_LoadTemplateXml:
			if(length == sizeof(bool))
			{
				if(*(bool*)data)
				{
					if(templates.loadTemplateXml())
					{
						printf("Templates Loaded\n");
					}
				}
			}
			break;
		case kParameter_SaveTemplateXml:
			if(length == sizeof(bool))
			{
				if(*(bool*)data)
				{
					templates.saveTemplateXml();
				}
			}
			break;
		}
	}
}
#endif //GUI_CONTROLS_H

