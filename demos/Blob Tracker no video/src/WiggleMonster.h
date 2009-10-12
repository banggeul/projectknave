

#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"

#define RES 30

class WiggleMonster {

public:
	
	int		 id;
	
	ofxVec2f root;
	
	ofxVec2f pnts[RES];
	ofxVec2f vels[RES];
	ofxVec2f dest[RES];
	
	WiggleMonster() {
	}
	
	// ---------------------------------
	void update() {
		
		for(int i=0; i<RES; i++) {
			
			float n = (float)i / (float)(RES-1);	
			float x = root.x + cos(n * TWO_PI) * 50;
			float y = root.y + sin(n * TWO_PI) * 50;
			
			dest[i].set(x, y);
			vels[i] = (vels[i] * 0.9) + (dest[i]-pnts[i]) / 8.0; 
			pnts[i] += vels[i];
			
			
			ofLine(root.x, root.y, x, y);
			ofCircle(x, y, 2);
		}
		
	}
	
	
	// ---------------------------------
	void draw() {
		
		
		for(int i=0; i<RES; i+=3) {
			ofLine(root.x, root.y, pnts[i].x, pnts[i].y);
			ofCircle(pnts[i].x, pnts[i].y, 2);
		}
		
		ofCircle(root.x, root.y, 3);
	}
	
};