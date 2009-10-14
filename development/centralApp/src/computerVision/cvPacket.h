#ifndef _CV_PACKET_H
#define _CV_PACKET_H

#include "ofMain.h"
#include "ofxCvMain.h"

#define MAX_N_BLOBS					20
#define MAX_BLOB_LENGTH				384

// this is a packet which repreents computer vision info for this given frame.

class computerVisionPacket {
public:

	int			myId;			// which client am I?		1 = hands, 2 = people left 3 = people right, 4 = stars
	int			frameNumber;	// which frame am I -- for sanity.

	int			width;			// width and height of this video frame
	int			height;

	int			nBlobs;
	int			nPts[MAX_N_BLOBS];
	ofPoint		pts[MAX_N_BLOBS][MAX_BLOB_LENGTH];					// todo: OF POINT IS HEAVY HERE... let's do simpler & better.

	bool		bAmInner[MAX_N_BLOBS];
	// add more stuff here....

	int			checkSum;		// myId + frameNumber + nBlobs	?  that's not ideal but all I can think of at the moment.

	computerVisionPacket() {
		myId = 0;
		frameNumber = 0;
		width = 0;
		height = 0;
		nBlobs = 0;
		checkSum = 0;
	}
}	;

#endif
