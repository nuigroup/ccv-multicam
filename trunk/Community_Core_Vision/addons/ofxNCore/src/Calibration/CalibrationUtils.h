/*
*  CalibrationUtils.h
*  
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#ifndef CALIBRATIONUTILS_H
#define CALIBRATIONUTILS_H

//Used other calibration
#include "rect2d.h"
#include "vector2d.h"
#include "windows.h"
#include "memory.h"
#include "ofMain.h"
//#include "ofxMultiplexer.h"

#define NULL_CAMERA 0xFF
#define INF 0xFFFFFFFF
class CalibrationUtils
{
	public:
		CalibrationUtils();
		~CalibrationUtils();
		void setCameraProperties(unsigned int stitchedFrameWidth,unsigned int stitchedFrameHeight,unsigned int cameraGridWidth,unsigned int cameraGridHeight,unsigned int calibrationGridWidth,unsigned int calibrationGridHeight,unsigned int* cameraWidth,unsigned int* cameraHeight,bool isInterleaveMode);
		void updateGridSize(int newGridWidth,int newGridHeight);
		void setCalibrationCamera(unsigned int cameraIndex);
		void computeCameraMaps(unsigned char** cameraMap,unsigned int stitchedFrameWidth,unsigned int stitchedFrameHeight,unsigned int cameraGridWidth,unsigned int cameraGridHeight,unsigned int calibrationGridWidth,unsigned int calibrationGridHeight,bool isInterleaveMode);
		void computeWeightMap(float* weightMap,vector2df* calibrationPoints,unsigned int cameraPosition,unsigned int stitchedFrameWidth,unsigned int stitchedFrameHeight,unsigned int cameraFrameWidth, unsigned int cameraFrameHeight, unsigned int cameraGridWidth,unsigned int cameraGridHeight,unsigned int calibrationGridWidth,unsigned int calibrationGridHeight,bool isInterleaveMode);
		void computeOffsetMap(unsigned int* offsetMap,vector2df* calibrationPoints,unsigned int cameraPosition,unsigned int stitchedFrameWidth,unsigned int stitchedFrameHeight,unsigned int cameraFrameWidth, unsigned int cameraFrameHeight, unsigned int cameraGridWidth,unsigned int cameraGridHeight,unsigned int calibrationGridWidth,unsigned int calibrationGridHeight,bool isInterleaveMode);
		bool isPointInTriangle(vector2df p, vector2df a, vector2df b, vector2df c);
		void initScreenPoints(int cameraPosition);
		void initCameraPoints(int camWidth,int camHeight);
		void cameraToScreenSpace(unsigned int &x, unsigned int &y);
		int findTriangleWithin(vector2df pt);		
		virtual void beginCalibration();
		virtual void nextCalibrationStep();
		virtual void revertCalibrationStep();
		void nextCameraCalibration();
		vector2df* screenPoints;
		vector2df* cameraPoints;
		vector2df* drawingPoints;
		bool bCalibrating;
		bool bGoToNextStep;
		bool bNextCamera;
		int  calibrationStep;
		int  GRID_POINTS;
		int  GRID_X;
		int  GRID_Y;
		unsigned int screenWidth,screenHeight;
		unsigned int* cameraWidth;
		unsigned int* cameraHeight;
		unsigned int selectedCameraIndex;

	private:
		void initTriangles();
		int GRID_INDICES;
		int* triangles;
	
		unsigned int cameraGridWidth,cameraGridHeight;
		bool bscreenPoints;
		bool bcameraPoints;
		bool isInterleaveMode;
};
#endif
