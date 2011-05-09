#ifndef OFXDSVL_H_INCLUDED
#define OFXDSVL_H_INCLUDED

#include "ofxCameraBase.h"
#include "dsvl.h"
#include "stdio.h"

//#define hibyte(x) (unsigned char)((x)>>8)


class ofxDSVL : ofxCameraBase
{
    public:
        ofxDSVL();
		~ofxDSVL();/*
		int getCameraBaseCount();
	    void setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled);
		void initializeWithGUID(GUID cameraGuid,int cameraWidth,int cameraHeight,int cameraLeft,int cameraTop,bool useROI,unsigned char cameraDepth,int cameraFramerate);
		void deinitializeCamera();
		void updateCurrentFrame();
		void getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled);
		void callCameraSettingsDialog();
		unsigned char* getCameraFrame();
		*/

        void initDSVL();
        void setDeviceID(int id);
		void showSettingsDialog();
		int getNumByes();
        int getDeviceID();
        bool grabFrame();
        int getCamWidth();
        int getCamHeight();
		bool isFrameNew();
		unsigned char * getPixels();

	private:
		//char* xml_config;
		DSVL_VideoSource *dsvl_vs;
		MemoryBufferHandle g_mbHandle;
		LONGLONG g_Timestamp;
		unsigned char *pbuffer;
		unsigned char *buffer;
		LONG	cap_width;
		LONG	cap_height;
		int		camWidth;
		int		camHeight;
		int		fps;
		int		bytes;
		double  cap_fps;
		bool	colour;
		bool	newFrame;
		int		sourceDepth_;
};
#endif
