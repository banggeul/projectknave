/*
 *  paintObject.h
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 10/16/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include "ofMain.h"



typedef struct {
	
	ofPoint		center;
	ofPoint		previousCenter;
	float		energy;
	int			id;
	int			whoThisFrame;
	bool		bFoundThisFrame;
	
	int			nFramesFound;
	
} contourObject;
	
	

class paintObject{
	
public: 
	
	
	paintObject(){
		pointCount = 0;
	}
	
	ofPoint		center;
	float		energy;
	int			id;
	int			whoThisFrame;
	bool		bFoundThisFrame;
	ofPoint		myColor;
	
	vector < contourObject > COBJ;

	// track contour info if possible.  fuck man this might work??  really?
	
	void paintObject::trackContour(ofPoint * pts, int _nPts);
	
	int pointCount;
	
};
