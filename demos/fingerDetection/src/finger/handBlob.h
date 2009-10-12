#ifndef _HAND_BLOB
#define _HAND_BLOB

#include "cv.h"

#include "ofMain.h"
#include "ofxVectorMath.h"
#include "cvLineFitter.h"

#define MAX_NUM_FINGER_CONTOUR_POINTS		2048
#define MAX_NUM_FINGERS						15
#define NPTS_FOR_LINE_FIT 					40



typedef struct {
	
	
	ofPoint		pos;
	float 		fingerness;
	float 		angle;
	float 		chisqr;
	float 		area;
	float 		arcl;
	float 		fingerAngle;
	float 		angleAverage;
	float 		angleStdDev;
	
	bool 		bIsAGoodFinger;			// does this finger pass different tests 
										// to remove good fingers?
	
	
}  fingerInfo;


class handBlob {
	
	public:
	
		handBlob();
		
		
		void 	draw();
		
	
		//--------------------------------------------------------------
		//
		// 			(step A) resample the contour into equally shaped sections -- this helps with smoothing and angle stuff.
		// 
		void 			addPoint(ofxPoint2f position);
		void 			start (ofxPoint2f position);
		float 			lengthSection;
		ofxPoint2f 		startPosition;
		ofxPoint2f 		currentPosition;
		float 			currentAngle;
		//
		// 			(step B) smooth alot...
		//
		void 	heavilySmoothContour();
		//
		// 			(step C) look at the angles, identify runs of curvature...
		//
		void 				processContour();
		ofxPoint2f 			ptsRun[MAX_NUM_FINGER_CONTOUR_POINTS];
		float 				anglesRun[MAX_NUM_FINGER_CONTOUR_POINTS];
		int 				nPtsRun;	


		//bool 				bIsThisRunAFinger( ofxPoint2f * ptsForRun, float * angsForRun, int nPts, fingerInfo & fingersInfo);
		bool 				bIsThisRunAFinger( int startRun, int endRun, fingerInfo & fingersInfo);
		bool				bIsThisRunAFinger( ofxPoint2f * ptsForRun, float * angsForRun, int nPtsForRun, fingerInfo & fingersInfo);
		
		
		
		cvLineFitter	CVLF;
		float 			angleThreshold;
		
		
		//--------------------------------------------------------------
		//
		// pts, angles and runs:
		//
		int 			nPts;
		ofxPoint2f		pts[MAX_NUM_FINGER_CONTOUR_POINTS];
		float 			angles[MAX_NUM_FINGER_CONTOUR_POINTS];
		bool 			bFingerPointRuns[MAX_NUM_FINGER_CONTOUR_POINTS];
		bool 			bFingerPointsConsidered[MAX_NUM_FINGER_CONTOUR_POINTS];
		
		//--------------------------------------------------------------
		//
		// fingers:
		//
		int 			nFingers;
		fingerInfo 		fingers[MAX_NUM_FINGERS];
		ofxPoint2f		lineFitPts[MAX_NUM_FINGER_CONTOUR_POINTS]; // once we have a finger, let's fit a line through it....
		
		int 			maxNumPts;
		int 			minRun;
		
		
		
};

#endif // _HAND_BLOB
