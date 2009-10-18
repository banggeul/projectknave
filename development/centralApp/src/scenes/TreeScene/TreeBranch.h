#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"

class TreeBranch {
	
public:
	float	alpha;	
	bool	bDone;
	bool	bMadeLeave;
	
	ofPoint		pos;
	ofxPoint2f  fern1P1, fern1P2, fern1des;
	ofxPoint2f  fern2P1, fern2P2, fern2des;
	float girth;
	
	TreeBranch() {
		girth = ofRandom(4, 10);
		alpha = 255;
		bDone = false;
		bMadeLeave = false;
	}
	
	void update() {
		
		fern1P2 += (fern1des - fern1P2) / 20.0;
		fern2P2 += (fern2des - fern2P2) / 24.0;
		
		float dis1 = fern1P2.distance(fern1des);
		float dis2 = fern2P2.distance(fern2des);
		
		if(dis1 <= 1.0 && dis2 <= 1.0) {
			bDone = true;
		}
	}
	
	void makeBranch(ofPoint branchPos, int step) {
		
		fern1P1  = branchPos;
		fern1P2  = branchPos;
		fern1des = branchPos;
		
		fern2P1  = branchPos;
		fern2P2  = branchPos;
		fern2des = branchPos;
		
		
		fern1des.x -= (step * ofRandom(3, 6));
		fern2des.x += (step * ofRandom(3, 6));
		
		fern1des.y -= 33;
		fern2des.y -= 33;
	}
	
	void draw() {
		ofFill();
		ofSetColor(255, 255, 255, alpha);
		glLineWidth(girth);
		
		ofLine(fern1P1.x, fern1P1.y, fern1P2.x, fern1P2.y);
		ofLine(fern2P1.x, fern2P1.y, fern2P2.x, fern2P2.y);
		
		glLineWidth(1.0);
		
		//drawThick();
	}
	
	
	void drawThick() {
		
		glBegin(GL_QUAD_STRIP);
		for(int i=0; i<20; i++) {
			
			float ni  = (float)i / 19.0;
			ofPoint p1;  
			ofPoint p2;
			
			p1.x = ni * fern1P2.x;
			
			glVertex2f(p1.x, p1.y);
			glVertex2f(p2.x, p2.y);
			
		}
		glEnd();
		
	}
	
	
	
};
