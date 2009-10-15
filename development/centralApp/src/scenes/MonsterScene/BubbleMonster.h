

#pragma once
#include "BaseMonster.h"
#include "MonsterEye.h"
#include "MonsterSVGParts.h"
#include "ofxDaito.h"

#define NUM_CONTOUR_PNTS	20
#define MAX_NUM_EYS			8
#define MAX_NUM_SPIKES		60
#define NUM_MONSTER_COLOR	6

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

//-------------------------------------------------------------- Bouncy Shape
typedef struct {
	int		 id;
	bool     bMatched;
	ofxVec2f pos;
	ofxVec2f vel;
	ofxVec2f acc;
} BouncyShape;

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
	MonsterSVGParts *   parts;
	
	// colors
	int				    monsterColor[NUM_MONSTER_COLOR];
	
	// contours
	vector <ofPoint>	normals;
	vector <ofPoint>	contourSimple;
	vector <ofPoint>	contourSmooth;
	vector <ofPoint>	contourConvex;
	
	BouncyShape			contour[NUM_CONTOUR_PNTS];
	
	
	// eyes
	int					eyeID; // <--- heheh get it
	int					colorID;
	int					spikeMode; // temp;
	
	int					numEyes;
	MonsterEye			eyes[MAX_NUM_EYS];
	
	// spikes
	float				spikeLength[MAX_NUM_SPIKES];
	
	//-------------------------------------------------------------- init
	BubbleMonster() {
		monsterID = -1;
		bDebug	  = false;
		parts	  = NULL;
		eyeID	  = 0;
		spikeMode = 0;
		
		monsterColor[0] = 0xd1007e;
		monsterColor[1] = 0xffc000;
		monsterColor[2] = 0x6ac539;
		monsterColor[3] = 0x00a3c4;
		monsterColor[4] = 0xff5500;
		monsterColor[5] = 0xff0009;
		
	}
	
	//-------------------------------------------------------------- init
	void init(ofCvTrackedBlob &blob) {
		
		spikeMode    = (int)ofRandom(0, 2);
		numEyes		 = (int)ofRandom(1, 3);//MAX_NUM_EYS);
		eyeID		 = 5;//(int)ofRandom(0, parts->eyes.size());
		colorID		 = (int)ofRandom(0, NUM_MONSTER_COLOR);
		
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
			
			// the contour
			contour[i].pos = 0;
			contour[i].vel = 0;
			contour[i].acc = 0;
			contour[i].id = -1;
			contour[i].bMatched = false;
		}
		
		for(int i=0; i<MAX_NUM_SPIKES; i++) {
			spikeLength[i] = ofRandom(10.0, 60.0);
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
			
			//if(cntPoints[i].bMatched) continue;
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
		
		
		// ---------------------------------- will come back to this
		
		/*
		 float minDistance = 1000000;
		 int minIndex = -1;
		 
		 for (int j = 0; j < nBlobs; j++){
		 
		 if (bFoundThisFrame[j]) continue; 
		 // try to find the closest "hand" to this object, withing a range
		 float diffx = HOBJ[i].center.x - packet.centroid[j].x;
		 float diffy =  HOBJ[i].center.y - packet.centroid[j].y;
		 float distance = sqrt(diffx * diffx + diffy*diffy);
		 
		 if (minDistance > distance){
		 minDistance = distance;
		 minIndex = j;
		 }
		 }
		 */		 
		/*
		 float minDis = 1000000; // start big
		 int	  minInd = -1;		// start false
		 
		 // find the contour point closest to me
		 for(int i=0; i<contourConvex.size(); i++) {
		 
		 if(contour[i].bMatched) continue;
		 
		 
		 
		 }
		 */
		
		
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
		
		
		/*
		int cLen = contourConvex.size();
		if(cLen >= MAX_CONTOUR_LENGTH)  cLen = MAX_CONTOUR_LENGTH-1;
		
		ofSetColor(5, 255, 50);
		for(int i=0; i<cLen; i++) {
			ofSetColor(25, 255, 0);
			ofCircle(contour[i].pos.x, contour[i].pos.y, 20);
			ofSetColor(255, 5, 255);
			ofDrawBitmapString(ofToString(contour[i].id), contour[i].pos.x, contour[i].pos.y);
		}
		
		*/
		
		
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
			
			ofSetColor(monsterColor[colorID]);
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
				
				normalScaled  = contourConvex[k] + (norml * spikeLength[k]);				
				normalScaleIn = (normlIn * 40);		
				
				if(spikeMode) {
					glLineWidth(10.0);
					ofLine(contourConvex[k].x, contourConvex[k].y,
						   normalScaled.x, normalScaled.y);
					glLineWidth(1.0);
				} else {
					
					
					float offset = 10;
					if(normalScaled.x > pos.x) offset = -10;
					
					ofTriangle(contourConvex[k].x+offset, contourConvex[k].y-20,
							   normalScaled.x, normalScaled.y,
							   contourConvex[k].x+offset, contourConvex[k].y+20);
				}
				
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
			// shit this feels weird
			//ofSetColor(255, 244, 244);
			eyes[i].draw();
			//parts->eyes[eyeID].draw(eyes[i].pos.x, eyes[i].pos.y);
		}
		
		
	}
	
};