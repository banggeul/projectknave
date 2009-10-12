#ifndef _FINGER_FINDER
#define _FINGER_FINDER


#include 	"ofMain.h"
#include	"ofxOpenCv.h"
#include 	"handBlob.h"

#define		MAX_NUM_CONTOURS_TO_FIND 50


class fingerFinder : public ofxCvContourFinder{

	public:
		
		int 						nFingerBlobs;
		handBlob					* fingerBlobs[MAX_NUM_CONTOURS_TO_FIND];
		
		fingerFinder();
		~fingerFinder(){};
	
		void reset();
		void draw();
		void processBlobs(vector <ofxCvBlob> blobs, int nBlobs);
		
		float 			angleThreshold;
		int 			minRun;
		
};

#endif


