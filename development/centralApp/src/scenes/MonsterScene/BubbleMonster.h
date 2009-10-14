

#pragma once
#include "BaseMonster.h"


#define NUM_CONTOUR_PNTS 60

class GiggleBubble {
public:
	ofxVec2f pos;
	float    radius;
	float    radiusD;
	float    radiusV;
};


//--------------------------------------------------------------
class BubbleMonster : public BaseMonster {
	
	
public:
	
	int					monsterID;
	ofxVec2f			points;
	GiggleBubble		bubbles[NUM_CONTOUR_PNTS];
	
	vector <ofPoint>	contourSimple;
	vector <ofPoint>	contourSmooth;

	ofxVec2f			eyeR, eyeL;
	
	//--------------------------------------------------------------
	BubbleMonster() {
		monsterID = -1;
	}
	
	//--------------------------------------------------------------
	void init(ofCvTrackedBlob &blob) {
		
		
		pos = blob.centroid;
		monsterID  = blob.id;
		
		printf("	monster %i made\n", monsterID);
		
		// init the points
		updateContourPnts(blob.pts);
		
		// set initial radius
		for(int i=0; i<NUM_CONTOUR_PNTS; i++) {
			bubbles[i].radius  = 1.0;
			bubbles[i].radiusD = ofRandom(16.0, 50.0);
			bubbles[i].radiusV = 0;
			
		}
	}
	
	
	//--------------------------------------------------------------
	void updateContourPnts(vector <ofPoint> &cntPoints) {
		/*
		 for(int i=0; i<NUM_CONTOUR_PNTS; i++) {
		 
		 
		 float minDis = 999999.0;
		 int   minID  = -1;
		 for(int j=0; j<cntPoints.size(); j++) {
		 
		 
		 ofxVec2f diff = bubbles[i].pos - cntPoints[j];
		 float	 len  = diff.length();
		 
		 if(len < minDis) {
		 minID = j;
		 minDis = len;
		 }
		 
		 
		 }
		 
		 bubbles[i].pos = cntPoints[minID];
		 
		 }
		 */
		
		for(int j=0; j<cntPoints.size(); j++) {
			
			float pct    = (float)j / (float)(cntPoints.size()-1);
			int   ind = pct * NUM_CONTOUR_PNTS;
			if(ind < 0) ind = 0;
			if(ind > NUM_CONTOUR_PNTS-1) ind = NUM_CONTOUR_PNTS-1;
			
			
			bubbles[ind].pos.x = cntPoints[j].x;
			bubbles[ind].pos.y = cntPoints[j].y;
			
			
		}
		
	}
	//-------------------------------------------------------------- new radius
	void genNewRadius() {
		for(int i=0; i<NUM_CONTOUR_PNTS; i++) {
			bubbles[i].radiusD = ofRandom(16.0, 20.0);			
		}
	}
	
	//-------------------------------------------------------------- update
	void update() {
		
		ofPoint highestPnt;
		highestPnt.x = 0;
		highestPnt.y = 9999999;
		
		for(int i=0; i<NUM_CONTOUR_PNTS; i++) {
			// make the bubble bouncy
			bubbles[i].radiusV = (bubbles[i].radiusV * 0.9) + (bubbles[i].radiusD-bubbles[i].radius) / 20.0;
			bubbles[i].radius += bubbles[i].radiusV;
			
			if(bubbles[i].pos.y < highestPnt.y) {
				highestPnt = bubbles[i].pos;
			}
			
		}
		
		
		// Spring
		float M = 0.8;   // Mass
		float K = 0.2;   // Spring constant
		float D = 0.92;  // Damping
		
		/*
		ofxVec2f force = -K * (p - pntB);    // f=-ky
		a = force / M;							
		v = D * (v + a);						
		p = p + v;	
		
		*/
		
		eyeL = highestPnt;
		eyeR = highestPnt;
		
		eyeL.x -= 20;
		eyeR.x += 20;
		
	}
	
	
	/*
	 // normal
	 ofxVec2f	normal;
	 ofxVec2f	delta;
	 ofxPoint2f	mid;
	 ofxPoint2f	normalScaled;
	 
	 for(int k=1; k<monsters[j].contourSmooth.size(); k++) {
	 
	 delta = monsters[j].contourSmooth[k] - monsters[j].contourSmooth[k-1];
	 delta.normalize();
	 normal.set(delta.y, -delta.x);
	 
	 normalScaled = monsters[j].contourSmooth[k] + (normal * 40);
	 
	 monsters[j].contourSmooth[k] = normalScaled;
	 
	 
	 }
	 */
	
	//-------------------------------------------------------------- draw
	void draw() {
		
		/*
		ofEnableAlphaBlending();
		ofFill();
		ofSetColor(255, 25, 0, 100);
		
		
		ofCircle(pos.x, pos.y, 30);
		
		ofSetColor(255, 255, 250);
		ofDrawBitmapString(ofToString(monsterID), pos.x, pos.y);
		ofDisableAlphaBlending();
		
		*/
		
		ofSetColor(5, 255, 50);

		for(int i=0; i<NUM_CONTOUR_PNTS; i++) {
			ofCircle(bubbles[i].pos.x, bubbles[i].pos.y, bubbles[i].radius);	
		}
		
		
		
		// draw the shape of the body
		ofFill();
		ofBeginShape();
		for(int i=0; i<contourSmooth.size(); i++) {
			ofVertex(contourSmooth[i].x, contourSmooth[i].y);
		}
		ofEndShape(true);
		
		
		// Right Eye
		ofFill();
		ofSetColor(0xffffff);
		ofCircle(eyeR.x, eyeR.y, 30);
		ofSetColor(0x000000);
		ofCircle(eyeR.x, eyeR.y+10, 3);

		// Left Eye
		ofFill();
		ofSetColor(0xffffff);
		ofCircle(eyeL.x, eyeL.y, 30);
		ofSetColor(0x000000);
		ofCircle(eyeL.x, eyeL.y+10, 3);
		
		
		
		
	}
	
};