

#pragma once
#include "BaseMonster.h"
#include "ofxMSASpline.h"
#include "MonsterEye.h"


#define NUM_CONTOUR_PNTS 20
#define MAX_NUM_EYS		 8

enum {
	
	BUBBLE_MONSTER,
	SPIKER_MONSTER
	
	
};


//-------------------------------------------------------------- a giggle bubble
class GiggleBubble {
public:
	ofxVec2f pos;
	float    radius;
	float    radiusD;
	float    radiusV;
};


//-------------------------------------------------------------- Monster
class BubbleMonster : public BaseMonster {
	
	
public:
	
	int					monsterMode;
	
	
	// settings
	bool				bDebug;
	
	
	// monster
	int					monsterID;
	ofxVec2f			points;
	GiggleBubble		bubbles[NUM_CONTOUR_PNTS];
	
	
	// contours
	vector <ofPoint>	normals;
	vector <ofPoint>	contourSimple;
	vector <ofPoint>	contourSmooth;
	vector <ofPoint>	contourConvex;
	ofxMSASpline2D		spline2D;
	
	
	// eyes
	int					numEyes;
	MonsterEye			eyes[MAX_NUM_EYS];
	
	//-------------------------------------------------------------- init
	BubbleMonster() {
		monsterID = -1;
		bDebug	  = false;
	}
	
	//-------------------------------------------------------------- init
	void init(ofCvTrackedBlob &blob) {
		
		numEyes		 = (int)ofRandom(1, 3);//MAX_NUM_EYS);
		monsterMode  = SPIKER_MONSTER; 
		pos			 = blob.centroid;
		monsterID    = blob.id;
		
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
	
	
	//-------------------------------------------------------------- normals  
	void getNormals() {
		/*
		 // normal
		 ofxVec2f	norml;
		 ofxVec2f	diff;
		 ofxPoint2f	mid;
		 ofxPoint2f	normalScaled;
		 
		 for(int k=1; k<monsters[j].contourSmooth.size(); k++) {
		 
		 diff = monsters[j].contourSmooth[k] - monsters[j].contourSmooth[k-1];
		 diff.normalize();
		 norml.set(delta.y, -delta.x);
		 
		 normalScaled = monsters[j].contourSmooth[k] + (norml * 40);
		 monsters[j].contourSmooth[k] = normalScaled;
		 
		 
		 }
		 */
		
	}
	
	
	//-------------------------------------------------------------- update contour pnts
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
		
		
		// update the eyes
		for(int i=0; i<numEyes; i++) {
			eyes[i].update();
		}
		
		if(numEyes == 2) {
			eyes[0].pos = highestPnt;
			eyes[1].pos = highestPnt;
			
			eyes[0].pos.x -= 30;
			eyes[1].pos.x += 30;
		}
		
		if(numEyes == 1) {
			eyes[0].pos = highestPnt;
			eyes[0].pos.x = pos.x;
		}
	}
	
	
	//-------------------------------------------------------------- draw
	void draw() {
		
		if(bDebug) {
			
			ofEnableAlphaBlending();
			ofFill();
			ofSetColor(255, 25, 0, 100);
			ofCircle(pos.x, pos.y, 30);
			ofSetColor(255, 255, 250);
			ofDrawBitmapString(ofToString(monsterID), pos.x, pos.y);
			ofDisableAlphaBlending();
			
		}
		
		// ------------------------------------  a bubble monster
		if(monsterMode == BUBBLE_MONSTER) {
			
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
			
		}
		
		
		// ------------------------------------  a spiker monster (convex)
		if(monsterMode == SPIKER_MONSTER) {
			ofSetColor(5, 255, 50);
			ofFill();
			ofBeginShape();
			for(int i=0; i<contourConvex.size(); i++) {
				ofVertex(contourConvex[i].x, contourConvex[i].y);
			}
			ofEndShape(true);
			
			
			
			// spikes
			
			ofFill();
			
			ofxVec2f	norml, normlIn;
			ofxVec2f	diff;
			ofxPoint2f	mid;
			ofxPoint2f	normalScaled, normalScaleIn;
			
			for(int k=1; k<contourConvex.size(); k++) {
				
				diff = contourConvex[k] - contourConvex[k-1];
				diff.normalize();
				norml.set(diff.y, -diff.x);
				
				normlIn.set(-diff.y, diff.x);
				
				normalScaled  = contourConvex[k] + (norml * 40);				
				normalScaleIn = (normlIn * 40);		
				
				glLineWidth(10.0);
				ofLine(contourConvex[k].x, contourConvex[k].y,
					   normalScaled.x, normalScaled.y);
				glLineWidth(1.0);
				/*
				 ofxVec2f pt = bubbles[k-1].pos;
				 float ang = pt.angle(normalScaled);
				 
				 glPushMatrix();
				 glTranslated(pt.x, pt.y, 0);
				 
				 if(normalScaled.x > pos.x) {
				 glRotatef(ang+90, 0, 0, 1);
				 }
				 else {
				 glRotatef(ang-90, 0, 0, 1);
				 }
				 
				 ofSetColor(0xffffff);
				 ofTriangle(-20, 0, 5, -140, 20, 0);
				 glPopMatrix();
				 */
			}
			
			
		}		
		
		
		
		// draw the eyes
		for(int i=0; i<numEyes; i++) {
			eyes[i].draw();	
		}
		
		
	}
	
};