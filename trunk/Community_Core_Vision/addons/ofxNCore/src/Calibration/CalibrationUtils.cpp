/*
*  CalibrationUtils.cpp
*
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#include "CalibrationUtils.h"
#include <cmath>

CalibrationUtils::CalibrationUtils()
{
	screenPoints = NULL;
	cameraPoints = NULL;
	triangles = NULL;
	cameraWidth = NULL;
	cameraHeight = NULL;
	drawingPoints = NULL;
	selectedCameraIndex = NULL_CAMERA;
	bNextCamera = false;
}

CalibrationUtils::~CalibrationUtils()
{
    delete screenPoints;
	delete cameraPoints;
    delete triangles;
	delete cameraWidth;
	delete cameraHeight;
	delete drawingPoints;
}



void CalibrationUtils::setCameraProperties(unsigned int stitchedFrameWidth,unsigned int stitchedFrameHeight,unsigned int cameraGridWidth,unsigned int cameraGridHeight,unsigned int calibrationGridWidth,unsigned int calibrationGridHeight,unsigned int* cameraWidth,unsigned int* cameraHeight,bool isInterleaveMode)
{
	GRID_X = calibrationGridWidth;
	GRID_Y = calibrationGridHeight;
	GRID_POINTS = ((GRID_X+1) * (GRID_Y+1));
    GRID_INDICES = (GRID_X * GRID_Y * 3 * 2);
	screenWidth = stitchedFrameWidth;
	screenHeight = stitchedFrameHeight;
	this->cameraGridWidth = cameraGridWidth;
	this->cameraGridHeight = cameraGridHeight;
	this->isInterleaveMode = isInterleaveMode;
	if (screenPoints == NULL)
		screenPoints = (vector2df*)malloc(GRID_POINTS * sizeof(vector2df));
	if (cameraPoints == NULL)
		cameraPoints = (vector2df*)malloc(GRID_POINTS*sizeof(vector2df));
	if (drawingPoints == NULL)
		drawingPoints = (vector2df*)malloc(GRID_POINTS*sizeof(vector2df));
	if (triangles == NULL)
		triangles = (int*)malloc((GRID_X+1)*(GRID_Y+1)*6*sizeof(int));
	if (this->cameraWidth == NULL)
		this->cameraWidth = (unsigned int*)malloc(cameraGridWidth*cameraGridHeight*sizeof(unsigned int));
	if (this->cameraHeight == NULL)
		this->cameraHeight = (unsigned int*)malloc(cameraGridWidth*cameraGridHeight*sizeof(unsigned int));
	memcpy((void*)this->cameraWidth,cameraWidth,cameraGridWidth*cameraGridHeight*sizeof(unsigned int));
	memcpy((void*)this->cameraHeight,cameraHeight,cameraGridWidth*cameraGridHeight*sizeof(unsigned int));
	initTriangles();
}

void CalibrationUtils::updateGridSize(int newGridWidth,int newGridHeight)
{
	GRID_X = newGridWidth;
	GRID_Y = newGridHeight;
	GRID_POINTS = ((GRID_X+1) * (GRID_Y+1));
    GRID_INDICES = (GRID_X * GRID_Y * 3 * 2);
	if (screenPoints != NULL)
		free(screenPoints);
	if (cameraPoints != NULL)
		free(cameraPoints);
	if (triangles != NULL)
		free(triangles);
	if (drawingPoints != NULL)
		free(drawingPoints);
	screenPoints = (vector2df*)malloc(GRID_POINTS * sizeof(vector2df));
	cameraPoints = (vector2df*)malloc(GRID_POINTS*sizeof(vector2df));
	drawingPoints = (vector2df*)malloc(GRID_POINTS*sizeof(vector2df));
	triangles = (int*)malloc((GRID_X+1)*(GRID_Y+1)*6*sizeof(int));
	initTriangles();
	setCalibrationCamera(selectedCameraIndex);
}

void CalibrationUtils::initTriangles()
{
	int t = 0;
	for(int j = 0; j < GRID_Y; j++)
	{
		for(int i = 0; i < GRID_X; i++)
		{
			triangles[t+0] = (i+0) + ((j+0) * (GRID_X+1));
			triangles[t+1] = (i+1) + ((j+0) * (GRID_X+1));
			triangles[t+2] = (i+0) + ((j+1) * (GRID_X+1));

			t += 3;

			triangles[t+0] = (i+1) + ((j+0) * (GRID_X+1));
			triangles[t+1] = (i+1) + ((j+1) * (GRID_X+1));
			triangles[t+2] = (i+0) + ((j+1) * (GRID_X+1));

			t += 3;
		}
	}
}

void CalibrationUtils::setCalibrationCamera(unsigned int cameraIndex)
{
	if (cameraIndex >= cameraGridWidth * cameraGridHeight)
	{
		return;
	}
	selectedCameraIndex = cameraIndex;
	unsigned char** cameraMap = NULL;
	cameraMap = (unsigned char**)malloc(4 * sizeof(unsigned char*));
	for (int i=0;i<4;i++)
		cameraMap[i] = (unsigned char*)malloc(screenWidth * screenHeight * sizeof(unsigned char));
	computeCameraMaps(cameraMap,screenWidth,screenHeight,cameraGridWidth,cameraGridHeight,GRID_X,GRID_Y,isInterleaveMode);
	int size = screenWidth * screenHeight;
	int LeftX = 0;
	int TopY = 0;
	for (int i=0;i<size;i++)
	{
		if ((cameraMap[0][i] == cameraIndex) ||
			(cameraMap[1][i] == cameraIndex) ||
			(cameraMap[2][i] == cameraIndex) ||
			(cameraMap[3][i] == cameraIndex))
		{
			LeftX = i % screenWidth;
			TopY = i / screenWidth;
			break;
		}
	}

	int globalCalibrationGridWidth = (isInterleaveMode ?  GRID_X * cameraGridWidth - cameraGridWidth + 1 : GRID_X * cameraGridWidth);
	int globalCalibrationGridHeight = (isInterleaveMode ? GRID_Y * cameraGridHeight - cameraGridHeight + 1 : GRID_Y * cameraGridHeight);
	
	float calibrationGridCellWidth = ((float)screenWidth) / globalCalibrationGridWidth;
	float calibrationGridCellHeight = ((float)screenHeight) / globalCalibrationGridHeight;

	int cameraX = cameraIndex % cameraGridWidth;
	int cameraY = cameraIndex / cameraGridWidth;
				
	cameraX *= (isInterleaveMode  ? (GRID_X-1) : GRID_X);
	cameraY *= (isInterleaveMode  ? (GRID_Y-1) : GRID_Y);

	int P = 0;
	for (int j=0;j<=GRID_Y;j++)
	{
		for (int i=0;i<=GRID_X;i++)
		{
			drawingPoints[P].X = (float)(calibrationGridCellWidth * i + LeftX)/screenWidth;
			drawingPoints[P].Y = (float)(calibrationGridCellHeight * j + TopY)/screenHeight;
			screenPoints[P].X = (float)(calibrationGridCellWidth * i + LeftX);
			screenPoints[P].Y = (float)(calibrationGridCellHeight * j + TopY);
			P++;
		}
	}
	for (int i=0;i<cameraGridWidth * cameraGridHeight;i++)
		free(cameraMap[i]);
	free(cameraMap);
	initCameraPoints(cameraWidth[selectedCameraIndex],cameraHeight[selectedCameraIndex]);
}

void CalibrationUtils::initCameraPoints(int camWidth,int camHeight)
{
	int p = 0;
	for(int j = 0; j <= GRID_Y; j++)
	{
		for(int i = 0; i <= GRID_X; i++)
		{
			cameraPoints[p] = vector2df((i * camWidth) / (float)GRID_X, (j * camHeight) / (float)GRID_Y);
			p++;
		}
	}
}

void CalibrationUtils::initScreenPoints(int cameraPosition)
{
	unsigned char** cameraMap = NULL;
	cameraMap = (unsigned char**)malloc(4 * sizeof(unsigned char*));
	for (int i=0;i<4;i++)
		cameraMap[i] = (unsigned char*)malloc(screenWidth * screenHeight * sizeof(unsigned char));
	computeCameraMaps(cameraMap,screenWidth,screenHeight,cameraGridWidth,cameraGridHeight,GRID_X,GRID_Y,isInterleaveMode);
	int size = screenWidth * screenHeight;
	int LeftX = 0;
	int TopY = 0;
	for (int i=0;i<size;i++)
	{
		if ((cameraMap[0][i] == cameraPosition) ||
			(cameraMap[1][i] == cameraPosition) ||
			(cameraMap[2][i] == cameraPosition) ||
			(cameraMap[3][i] == cameraPosition))
		{
			LeftX = i % screenWidth;
			TopY = i / screenWidth;
			break;
		}
	}

	int globalCalibrationGridWidth = (isInterleaveMode ?  GRID_X * cameraGridWidth - cameraGridWidth + 1 : GRID_X * cameraGridWidth);
	int globalCalibrationGridHeight = (isInterleaveMode ? GRID_Y * cameraGridHeight - cameraGridHeight + 1 : GRID_Y * cameraGridHeight);
	
	float calibrationGridCellWidth = ((float)screenWidth) / globalCalibrationGridWidth;
	float calibrationGridCellHeight = ((float)screenHeight) / globalCalibrationGridHeight;

	int cameraX = cameraPosition % cameraGridWidth;
	int cameraY = cameraPosition / cameraGridWidth;
				
	cameraX *= (isInterleaveMode  ? (GRID_X-1) : GRID_X);
	cameraY *= (isInterleaveMode  ? (GRID_Y-1) : GRID_Y);

	int P = 0;
	for (int j=0;j<=GRID_Y;j++)
	{
		for (int i=0;i<=GRID_X;i++)
		{
			screenPoints[P].X = (float)(calibrationGridCellWidth * i + LeftX);
			screenPoints[P].Y = (float)(calibrationGridCellHeight * j + TopY);
			P++;
		}
	}
	for (int i=0;i<cameraGridWidth * cameraGridHeight;i++)
		free(cameraMap[i]);
	free(cameraMap);
}

void CalibrationUtils::cameraToScreenSpace(unsigned int &x, unsigned int &y)
{
	vector2df pt((float)x, (float)y);
	int t = findTriangleWithin(pt);
	if(t != -1)
	{
		vector2df A = screenPoints[triangles[t+0]];
		vector2df B = screenPoints[triangles[t+1]];
		vector2df C = screenPoints[triangles[t+2]];

		float total_area = (A.X - B.X) * (A.Y - C.Y) - (A.Y - B.Y) * (A.X - C.X);
		float area_A = (pt.X - B.X) * (pt.Y - C.Y) - (pt.Y - B.Y) * (pt.X - C.X);
		float area_B = (A.X - pt.X) * (A.Y - C.Y) - (A.Y - pt.Y) * (A.X - C.X);
		float bary_A = area_A / total_area;
		float bary_B = area_B / total_area;
		float bary_C = 1.0f - bary_A - bary_B;
		
		vector2df sA = cameraPoints[triangles[t+0]];
		vector2df sB = cameraPoints[triangles[t+1]];
		vector2df sC = cameraPoints[triangles[t+2]];

		vector2df transformedPos;

		transformedPos = (sA*bary_A) + (sB*bary_B) + (sC*bary_C);

		x = (unsigned int)transformedPos.X;
		y = (unsigned int)transformedPos.Y;
		return;
	}
	x = 0;
	y = 0;
}

bool CalibrationUtils::isPointInTriangle(vector2df p, vector2df a, vector2df b, vector2df c)
{
	if (vector2df::isOnSameSide(p,a, b,c) && vector2df::isOnSameSide(p,b, a,c) && vector2df::isOnSameSide(p, c, a, b))
		return true;
    else
		return false;
}

int CalibrationUtils::findTriangleWithin(vector2df pt)
{
	for(int t = 0; t < GRID_INDICES; t += 3)
	{
		if(isPointInTriangle(pt, screenPoints[triangles[t]], screenPoints[triangles[t+1]], screenPoints[triangles[t+2]]) )
			return t;
	}
	return -1;
}

void CalibrationUtils::nextCameraCalibration()
{
	selectedCameraIndex++;
	if (selectedCameraIndex >= cameraGridWidth * cameraGridHeight)
	{
		selectedCameraIndex = NULL_CAMERA;
	}
	setCalibrationCamera(selectedCameraIndex);
}

void CalibrationUtils::beginCalibration()
{
	 bCalibrating = true;
	 calibrationStep = 0;
}

void CalibrationUtils::nextCalibrationStep()
{
	if(bCalibrating)
	{
		calibrationStep++;
		if (calibrationStep >= GRID_POINTS) 
		{
			bNextCamera = true;
			calibrationStep = 0;
		}
	}
}
void CalibrationUtils::revertCalibrationStep()
{
	if(bCalibrating)
	{
		calibrationStep--;
		if(calibrationStep < 0)
		{
			calibrationStep = 0;
		}
	}
}


void CalibrationUtils::computeCameraMaps(unsigned char** cameraMap,unsigned int stitchedFrameWidth,unsigned int stitchedFrameHeight,unsigned int cameraGridWidth,unsigned int cameraGridHeight,unsigned int calibrationGridWidth,unsigned int calibrationGridHeight,bool isInterleaveMode)
{
	if (cameraMap == NULL)
		return;
	int size = stitchedFrameWidth*stitchedFrameHeight;
	int cameraCount = cameraGridWidth * cameraGridHeight;
	for (int i=0;i<4;i++)
		memset(cameraMap[i],NULL_CAMERA,stitchedFrameWidth*stitchedFrameHeight*sizeof(unsigned char));
	if (isInterleaveMode)
	{
		int globalCalibrationGridWidth = calibrationGridWidth * cameraGridWidth - cameraGridWidth + 1;
		int globalCalibrationGridHeight = calibrationGridHeight * cameraGridHeight - cameraGridHeight + 1;
		float calibrationGridCellWidth = ((float)stitchedFrameWidth) / globalCalibrationGridWidth;
		float calibrationGridCellHeight = ((float)stitchedFrameHeight) / globalCalibrationGridHeight;
		for (int i=0;i<size;i++)
		{
			int x = i % stitchedFrameWidth;
			int y = i / stitchedFrameWidth;
			int blockX = (int)(((float)x) / calibrationGridCellWidth);
			int blockY = (int)(((float)y) / calibrationGridCellHeight);
			for (int j=0;j<cameraCount;j++)
			{
				int cameraX = j % cameraGridWidth;
				int cameraY = j / cameraGridWidth;
				cameraX *= (calibrationGridWidth-1);
				cameraY *= (calibrationGridHeight-1);
				if ((blockX>=cameraX)&&(blockY>=cameraY)&&(blockX<cameraX+calibrationGridWidth) && (blockY<cameraY+calibrationGridHeight))
				{
					for (int k=0;k<4;k++)
					{
						if (cameraMap[k][i] == NULL_CAMERA)
						{
							cameraMap[k][i] = (unsigned char)j;
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		int blockWidthSize = stitchedFrameWidth / cameraGridWidth;
		int blockHeightSize = stitchedFrameHeight / cameraGridHeight;
		for (int i=0;i<size;i++)
		{
			int x = i % stitchedFrameWidth;
			int y = i / stitchedFrameWidth;
			int index = (y/blockHeightSize) * cameraGridWidth + (x/blockWidthSize);
			cameraMap[0][i]=(unsigned char)index;
		}
	}
}
void CalibrationUtils::computeWeightMap(float* weightMap,vector2df* calibrationPoints,unsigned int cameraPosition,unsigned int stitchedFrameWidth,unsigned int stitchedFrameHeight,unsigned int cameraFrameWidth, unsigned int cameraFrameHeight, unsigned int cameraGridWidth,unsigned int cameraGridHeight,unsigned int calibrationGridWidth,unsigned int calibrationGridHeight,bool isInterleaveMode)
{
	if (weightMap == NULL)
		weightMap = (float*)malloc(stitchedFrameWidth * stitchedFrameHeight * sizeof(float));
	int size = stitchedFrameWidth*stitchedFrameHeight;
	unsigned char** cameraMap = NULL;
	cameraMap = (unsigned char**)malloc(4 * sizeof(unsigned char*));
	for (int i=0;i<4;i++)
		cameraMap[i] = (unsigned char*)malloc(stitchedFrameWidth * stitchedFrameHeight * sizeof(unsigned char));
	computeCameraMaps(cameraMap,stitchedFrameWidth,stitchedFrameHeight,cameraGridWidth,cameraGridHeight,calibrationGridWidth,calibrationGridHeight,isInterleaveMode);
	for (int i=0;i<size;i++)
		weightMap[i] = ((cameraMap[0][i] == cameraPosition) || 
			(cameraMap[1][i] == cameraPosition) || 
			(cameraMap[2][i] == cameraPosition) || 
			(cameraMap[3][i] == cameraPosition)) ? 1.0f : 0.0f;		
	int globalCalibrationGridWidth = calibrationGridWidth * cameraGridWidth - cameraGridWidth + 1;
	int globalCalibrationGridHeight = calibrationGridHeight * cameraGridHeight - cameraGridHeight + 1;
	float calibrationGridCellWidth = ((float)stitchedFrameWidth) / globalCalibrationGridWidth;
	float calibrationGridCellHeight = ((float)stitchedFrameHeight) / globalCalibrationGridHeight;
	float horizontalOffset =  1.0f / calibrationGridCellWidth;
	horizontalOffset*=1.045;
	float verticalOffset = 1.0f / calibrationGridCellHeight;
	verticalOffset*=1.055;

	if (isInterleaveMode)
	{
		for (int i=0;i<size;i++)
		{
			if (((cameraMap[0][i] == cameraPosition) || 
				(cameraMap[1][i] == cameraPosition)) &&
				(cameraMap[1][i] != NULL_CAMERA) &&
				(cameraMap[2][i] == NULL_CAMERA) &&
				(cameraMap[3][i] == NULL_CAMERA))
			{
				unsigned int distance = INF;
				int closestX = 0;
				int closestY = 0;
				int X = i % stitchedFrameWidth;
				int Y = i / stitchedFrameWidth;
				for (int x=-calibrationGridCellWidth;x<=calibrationGridCellWidth;x++)
				{
					if ((X+x+(Y*stitchedFrameWidth)>=0) && (X+x+(Y*stitchedFrameWidth)<size))
					{
						if ((cameraMap[0][X+x+(Y*stitchedFrameWidth)] == cameraPosition) && 
							(cameraMap[1][X+x+(Y*stitchedFrameWidth)] == NULL_CAMERA) &&
							(cameraMap[2][X+x+(Y*stitchedFrameWidth)] == NULL_CAMERA) &&
							(cameraMap[3][X+x+(Y*stitchedFrameWidth)] == NULL_CAMERA))
							{
								if (abs(x)<distance)
								{
									distance = (unsigned int)abs(x);
									closestX = X+x;
									closestY = Y;
								}
							}
					}
				}
				
				for (int y=-calibrationGridCellHeight;y<calibrationGridCellHeight;y++)
				{
					if ((X+((Y+y)*stitchedFrameWidth)>=0) && (X+((Y+y)*stitchedFrameWidth)<size))
					{
						if ((cameraMap[0][X+((Y+y)*stitchedFrameWidth)] == cameraPosition) && 
							(cameraMap[1][X+((Y+y)*stitchedFrameWidth)] == NULL_CAMERA) &&
							(cameraMap[2][X+((Y+y)*stitchedFrameWidth)] == NULL_CAMERA) &&
							(cameraMap[3][X+((Y+y)*stitchedFrameWidth)] == NULL_CAMERA))
							{
								if (abs(y)<distance)
								{
									distance = (unsigned int)abs(y);
									closestX = X;
									closestY = Y+y;
								}
							}
					}
				}
				if (distance!=INF)
				{
					if (closestX == X)
						weightMap[i] = (verticalOffset * (calibrationGridCellHeight-distance)*0.97);
					else
						if (closestY == Y)
							weightMap[i] = (horizontalOffset * (calibrationGridCellWidth-distance)*0.97);
						else
							weightMap[i] = 0.0f;
				}
				else
					weightMap[i] = 0.0f;
			}
		}
		
		for (int i=0;i<size;i++)
		{
			if (((cameraMap[0][i] == cameraPosition) || 
				(cameraMap[1][i] == cameraPosition) ||
				(cameraMap[2][i] == cameraPosition) ||
				(cameraMap[3][i] == cameraPosition)) &&
				((cameraMap[0][i] != NULL_CAMERA) &&
				(cameraMap[1][i] != NULL_CAMERA) &&
				(cameraMap[2][i] != NULL_CAMERA) &&
				(cameraMap[3][i] != NULL_CAMERA)))
			{
				unsigned int distance = INF;
				int closestX = 0;
				int closestY = 0;
				int X = i % stitchedFrameWidth;
				int Y = i / stitchedFrameWidth;
				float result = 0.0f;
				for (int x=-calibrationGridCellWidth;x<=calibrationGridCellWidth;x++)
				{
					if ((X+x+(Y*stitchedFrameWidth)>=0) && (X+x+(Y*stitchedFrameWidth)<size))
					{
					if (((cameraMap[0][X+x+(Y*stitchedFrameWidth)] == cameraPosition) || 
						(cameraMap[1][X+x+(Y*stitchedFrameWidth)] == cameraPosition)) &&
						((cameraMap[0][X+x+(Y*stitchedFrameWidth)] != NULL_CAMERA) &&
						(cameraMap[1][X+x+(Y*stitchedFrameWidth)] != NULL_CAMERA) &&
						(cameraMap[2][X+x+(Y*stitchedFrameWidth)] == NULL_CAMERA) &&
						(cameraMap[3][X+x+(Y*stitchedFrameWidth)] == NULL_CAMERA)))
						{
							if (abs(x)<distance)
							{
								distance = (unsigned int)abs(x);
								closestX = X+x;
								closestY = Y;
							}
						}
					}
				}
				if (distance!=INF)
					result = weightMap[closestX+closestY*stitchedFrameWidth];
				distance = INF;
				for (int y=-calibrationGridCellHeight;y<calibrationGridCellHeight;y++)
				{
					if ((X+(Y+y)*stitchedFrameWidth>=0) && (X+(Y+y)*stitchedFrameWidth<size))
					{
					if (((cameraMap[0][X+((Y+y)*stitchedFrameWidth)] == cameraPosition) || 
						(cameraMap[1][X+((Y+y)*stitchedFrameWidth)] == cameraPosition)) &&
						((cameraMap[0][X+((Y+y)*stitchedFrameWidth)] != NULL_CAMERA) &&
						(cameraMap[1][X+((Y+y)*stitchedFrameWidth)] != NULL_CAMERA) &&
						(cameraMap[2][X+((Y+y)*stitchedFrameWidth)] == NULL_CAMERA) &&
						(cameraMap[3][X+((Y+y)*stitchedFrameWidth)] == NULL_CAMERA)))
						{
							if (abs(y)<distance)
							{
								distance = (unsigned int)abs(y);
								closestX = X;
								closestY = Y+y;
							}
						}
					}
				}
				weightMap[i] = result * weightMap[closestX+closestY*stitchedFrameWidth];
			}
		}
	}
	for (int i=0;i<4;i++)
		free(cameraMap[i]);
	free(cameraMap);
}

void CalibrationUtils::computeOffsetMap(unsigned int* offsetMap,vector2df* calibrationPoints,unsigned int cameraPosition,unsigned int stitchedFrameWidth,unsigned int stitchedFrameHeight,unsigned int cameraFrameWidth, unsigned int cameraFrameHeight, unsigned int cameraGridWidth,unsigned int cameraGridHeight,unsigned int calibrationGridWidth,unsigned int calibrationGridHeight,bool isInterleaveMode)
{
	//initCameraPoints(cameraFrameWidth,cameraFrameHeight);
	initScreenPoints(cameraPosition);
	memcpy((void*)cameraPoints,calibrationPoints,GRID_POINTS * sizeof(vector2df));
	memset(offsetMap,0,stitchedFrameWidth * stitchedFrameHeight * sizeof(unsigned int));
	unsigned char** cameraMap = NULL;
	cameraMap = (unsigned char**)malloc(4 * sizeof(unsigned char*));
	for (int i=0;i<4;i++)
		cameraMap[i] = (unsigned char*)malloc(stitchedFrameWidth * stitchedFrameHeight * sizeof(unsigned char));
	computeCameraMaps(cameraMap,stitchedFrameWidth,stitchedFrameHeight,cameraGridWidth,cameraGridHeight,calibrationGridWidth,calibrationGridHeight,isInterleaveMode);
	int T = 0;
	for (int j=0;j<stitchedFrameHeight;j++)
	{
		for (int i=0;i<stitchedFrameWidth;i++)
		{
			unsigned int transformedX = i;
			unsigned int transformedY = j;
			if ((cameraMap[0][i+j*stitchedFrameWidth] != cameraPosition)&&
				(cameraMap[1][i+j*stitchedFrameWidth] != cameraPosition)&&
				(cameraMap[2][i+j*stitchedFrameWidth] != cameraPosition)&&
				(cameraMap[3][i+j*stitchedFrameWidth] != cameraPosition))
				transformedX = transformedY = 0;
			else
				cameraToScreenSpace(transformedX,transformedY);
			if (transformedX == cameraFrameWidth)
				transformedX--;
			if (transformedY == cameraFrameHeight)
				transformedY--;
 			offsetMap[T] = transformedX+transformedY*cameraFrameWidth;
			T++;
		}
	}
	for (int i=0;i<4;i++)
		free(cameraMap[i]);
	free(cameraMap);
}

