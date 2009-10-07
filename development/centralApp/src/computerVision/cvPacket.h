#ifndef _CV_PACKET_H
#define _CV_PACKET_H


#include "ofMain.h"
#include "ofxCvMain.h"


#define MAX_N_BLOBS					10
#define MAX_BLOB_LENGTH				512

// this is a packet which repreents computer vision info for this given frame. 

typedef struct {
	
	int			nBlobs;
	int			nPts[MAX_N_BLOBS];
	ofPoint		pts[MAX_N_BLOBS][MAX_BLOB_LENGTH];
	
	// add more stuff here....
	
	
}	computerVisionPacket;




#endif