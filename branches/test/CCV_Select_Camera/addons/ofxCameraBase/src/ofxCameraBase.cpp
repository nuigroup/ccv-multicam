#include "ofxCameraBase.h"
// COMMIT BY YISHI GUO 2011-08-01
//void ofxCameraBase::getCameraSize(unsigned int* cameraWidth,unsigned int* cameraHeight,unsigned char* cameraDepth)
//{
//	if (!isInitialized)
//	{
//		*cameraWidth = *cameraHeight = *cameraDepth = 0;
//		return;
//	}
//	*cameraWidth = width;
//	*cameraHeight = height;
//	*cameraDepth = depth;
//}
//
//void ofxCameraBase::callCameraSettingsDialog()
//{
//	//AChurikov:camera settings logic should be added here/
//	//To Yishi
//}
//
//DWORD WINAPI ofxCameraBase::CaptureThread(LPVOID instance)
//{
//	ofxCameraBase *pThis = (ofxCameraBase*)instance;
//	pThis->Capture();
//	return 0;
//}
//
//void ofxCameraBase::StartThreadingCapture()
//{
//	InitializeCriticalSection(&criticalSection); 
//	isCaptureThreadRunning = true;
//	captureThread = CreateThread(NULL, 0, &ofxCameraBase::CaptureThread, this, 0, 0);
//}
//
//void ofxCameraBase::StopThreadingCapture()
//{
//	isCaptureThreadRunning = false;
//	Sleep(100);
//	DeleteCriticalSection(&criticalSection);
//}
//
//
//void ofxCameraBase::Capture()
//{
//	while (isCaptureThreadRunning)
//	{
//		if ((!isNewFrame) || (newFrameCurrentLifetime>=_MAX_FRAME_LIFETIME_))
//		{
//			EnterCriticalSection(&criticalSection); 
//			isNewFrame = true;
//			newFrameCurrentLifetime = 0;
//			updateCurrentFrame();
//			LeaveCriticalSection(&criticalSection);
//		}
//		else
//			newFrameCurrentLifetime++;
//		Sleep(1);
//	}
//}
//
//void ofxCameraBase::getCameraFrame(unsigned char* newFrameData)	
//{ 
//	if (isNewFrame)
//	{
//		EnterCriticalSection(&criticalSection); 
//		isNewFrame = false;
//		memcpy((void*)newFrameData,cameraFrame,width*height*sizeof(unsigned char));
//		LeaveCriticalSection(&criticalSection);
//	}
//}
//
////void ofxCameraBase::startRecording()
////{
////	if (isVideoPlaying)
////		return;
////	if (!isVideoRecording)
////	{
////		videoWriter = new ofxVideoWriter();
////		std::string fileName = "VIDEO";
////		switch (cameraType)
////		{
////		case PS3:
////			fileName = "PS3" + 	GUIDToString(guid);
////			break;
////		case CMU:
////			fileName = "CMU" + IntToString((int)guid.Data1);
////			break;
////		case FFMV:
////			fileName = "FFMV"+ IntToString((int)guid.Data1);
////			break;
////		case DIRECTSHOW:
////			fileName = "DIRECTSHOW" + IntToString((int)guid.Data1);
////			break;
////		case KINECT:
////			fileName = "KINECT" + IntToString((int)guid.Data1);
////			break;
////		}
////		std::string filePath = "data/record/" + fileName  + ".avi";
////		videoWriter->open(filePath.c_str(),width,height,24,30);
////	}
////	isVideoRecording = true;
////}
////
////void ofxCameraBase::stopRecording()
////{	
////	if (videoWriter!=NULL)
////	{
////		videoWriter->close();
////		videoWriter = NULL;
////	}
////	isVideoRecording = false;
////}
//
//void ofxCameraBase::initializeWithGUID(GUID cameraGuid,int cameraWidth,int cameraHeight,int cameraLeft,int cameraTop,unsigned char cameraDepth,int cameraFramerate,bool isEmulating,PIXEL_MODE pixelMode)
//{
//	guid = cameraGuid;
//	width = cameraWidth;
//	height = cameraHeight;
//	framerate = cameraFramerate;
//	left = cameraLeft;
//	top = cameraTop;
//	depth = cameraDepth;
//	cameraPixelMode = pixelMode;
//	isVideoPlaying = isEmulating;
//	index = -1;
//	setCameraType();
//	if (isVideoPlaying)
//	{
//		videoPlayer = new ofVideoPlayer();
//		std::string fileName = "VIDEO";
//		switch (cameraType)
//		{
//		case PS3:
//			fileName = "PS3" + 	GUIDToString(guid);
//			break;
//		case CMU:
//			fileName = "CMU" + IntToString((int)guid.Data1);
//			break;
//		case FFMV:
//			fileName = "FFMV"+ IntToString((int)guid.Data1);
//			break;
//		case DIRECTSHOW:
//			fileName = "DIRECTSHOW" + IntToString((int)guid.Data1);
//			break;
//		case KINECT:
//			fileName = "KINECT" + IntToString((int)guid.Data1);
//			break;
//		}
//        bool isLoaded = videoPlayer->loadMovie("data/record/" + fileName + ".avi");
//        videoPlayer->setLoopState(OF_LOOP_NORMAL);
//	}
//	else
//	{
//		cameraInitializationLogic();
//	}
//	cameraFrame = new unsigned char[depth*width*height];
//	videoFrame = new unsigned char[3*width*height];
//	isInitialized = true;
//}
//
//void ofxCameraBase::startCamera()
//{
//	if (isVideoPlaying)
//	{
//	    videoPlayer->play();
//	}
//	StartThreadingCapture();
//}
//
//void ofxCameraBase::deinitializeCamera()
//{
//		if (isInitialized)
//	{
//		StopThreadingCapture();
//		if (isVideoPlaying)
//		{
//			delete videoPlayer;	videoPlayer = NULL;
//		}
//		else
//		{
//			cameraDeinitializationLogic();
//		}
//		delete [] cameraFrame;
//		delete [] videoFrame;
//		isInitialized = false;
//	}
//}
//
//void ofxCameraBase::updateCurrentFrame()
//{
// 	if (isVideoPlaying)
//	{
//		videoPlayer->idleMovie();
//		bool bNewFrame = videoPlayer->isFrameNew();
//		if (bNewFrame && videoPlayer->isLoaded())
//		{
//			unsigned char* data = videoPlayer->getPixels();
//			for (int i=0;i<width*height;i++)
//				cameraFrame[i] = (unsigned char)(0.3f*(float)data[i*3+2] + 0.59f*(float)data[i*3+1] +0.11f*(float)data[i*3]) ;
//		}
//	}
//	else
//	{
//		getNewFrame(cameraFrame);
//		if (depth>1)
//		{
//			int size = width*height;
//			for (int i=0;i<size;i++)
//				cameraFrame[i] = (unsigned char)(0.3f*(float)cameraFrame[i*depth+2] + 0.59f*(float)cameraFrame[i*depth+1] +0.11f*(float)cameraFrame[i*depth]) ;
//		}
//		//if ((videoWriter!=NULL) && isVideoRecording)
//		//{
//		//	for (int i=0;i<width*height;i++)
//		//	{
//		//		videoFrame[3*i+0] = cameraFrame[i];
//		//		videoFrame[3*i+1] = cameraFrame[i];
//		//		videoFrame[3*i+2] = cameraFrame[i];
//		//	}
//		//	videoWriter->appendFrame(videoFrame,true);
//		//}
//	}
//}
