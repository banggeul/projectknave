
#include "fingerFinder.h"


//-----------------------------------------------------------------
fingerFinder::fingerFinder(){
	nFingerBlobs 		= 0;
	angleThreshold 		= -1.0;
	minRun				= 10;
	
	for (int i = 0; i < MAX_NUM_CONTOURS_TO_FIND; i++){
		fingerBlobs[i] = new handBlob();
	}
}

//-----------------------------------------------------------------
void fingerFinder::reset(){
	nFingerBlobs = 0;
	
	for (int i = 0; i < MAX_NUM_CONTOURS_TO_FIND; i++){
		memset(fingerBlobs[i], 0,sizeof(handBlob));
	}
	
	//memset(fingerBlobs, 0, MAX_NUM_CONTOURS_TO_FIND * sizeof(handBlob));
}

//-----------------------------------------------------------------
void fingerFinder::processBlobs(vector <ofxCvBlob> blobs, int nBlobs){
	
	
	nFingerBlobs = nBlobs;
	
	
	for (int i = 0; i < nFingerBlobs; i++){
		fingerBlobs[i]->angleThreshold = angleThreshold;
		fingerBlobs[i]->minRun = minRun;
	}
	
	
	
	
	for (int i = 0; i < nFingerBlobs; i++){

		int nPts 				= blobs[i].nPts;
		ofxPoint2f 				pt;
				
		// compute arclength per blob
		float blobLen = 0;
		for (int j = 1; j < nPts; j++){
			float diffx = blobs[i].pts[j].x - blobs[i].pts[j-1].x;
			float diffy = blobs[i].pts[j].y - blobs[i].pts[j-1].y;
			blobLen += sqrt(diffx*diffx + diffy*diffy);
		}
		
		fingerBlobs[i]->lengthSection = 1.0; //((float)blobLen / 600.0f);		// useful to make the angle stuff make sense across diff. blob sizes.
		
		for (int j = 0; j < nPts; j++){

			pt.x = blobs[i].pts[j].x;
			pt.y = blobs[i].pts[j].y;
			
			if (j == 0){
				fingerBlobs[i]->start(pt);
			} else {
				fingerBlobs[i]->addPoint(pt);
			}
		}
		
		if (nPts > 0){
			fingerBlobs[i]->heavilySmoothContour();
			fingerBlobs[i]->processContour();
		}
	}
	
}


//-----------------------------------------------------------------
void fingerFinder::draw(){
	for (int i = 0; i < nFingerBlobs; i++){
		//fingerBlobs[i]->draw();
		fingerBlobs[i]->draw();
	}
}