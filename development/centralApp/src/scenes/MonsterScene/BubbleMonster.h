

#pragma once
#include "BaseMonster.h"
#include "MonsterEye.h"
#include "MonsterSVGParts.h"
#include "ofxDaito.h"
#include "MonsterConst.h"
#include "MonsterHair.h"

enum {
	
	BUBBLE_MONSTER,
	SPIKER_MONSTER,
	MEGA_SPIKER,
	MEGA_SPIKER_SOFT
	
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
	
	
	
	float				age, area;
	int					monsterMode;
	
	
	// settings
	bool				bMonster;
	bool				bDebug;
	
	
	// monster
	int					monsterID;
	ofxVec2f			points;
	GiggleBubble		bubbles[NUM_BUBBLE_PNTS];
	MonsterSVGParts *   parts;
	
	
	// contours
	vector <ofPoint>	normals;
	vector <ofPoint>	contourSimple;
	vector <ofPoint>	contourSmooth;
	vector <ofPoint>	contourConvex;
	
	BouncyShape			contour[NUM_CONTOUR_PNTS];
	MonsterHair			hair[NUM_CONTOUR_PNTS];
	
	
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
		bMonster  = false;
		monsterID = -1;
		bDebug	  = false;
		parts	  = NULL;
		eyeID	  = 0;
		spikeMode = 0;
		
		vel = 0;
		pos = 0;
		prePos = 0;
		age = 0;
	}
	
	//-------------------------------------------------------------- init
	void init(ofCvTrackedBlob &blob) {
		
		age = 0; 
		area = 1.0;
		spikeMode    = (int)ofRandom(0, 2);
		numEyes		 = (int)ofRandom(1, 3);//MAX_NUM_EYS);
		eyeID		 = 5;//(int)ofRandom(0, parts->eyes.size());
		colorID		 = (int)ofRandom(0, NUM_MONSTER_COLOR);
		
		//monsterMode  = (int)ofRandom(0, 2); 
		pos			 = blob.centroid * SCALE;
		monsterID    = blob.id;
		
		printf("	monster %i made\n", monsterID);
		
		// init the points
		updateContourPnts(blob.pts);
		
		// set initial radius
		for(int i=0; i<NUM_BUBBLE_PNTS; i++) {
			bubbles[i].radius  = 1.0;
			bubbles[i].radiusD = ofRandom(16.0, 20.0);
			bubbles[i].radiusV = 0;
		}
		
		for(int i=0; i<NUM_CONTOUR_PNTS; i++) {
			
			// the contour
			contour[i].pos = 0;
			contour[i].vel = 0;
			contour[i].acc = 0;
			contour[i].id = -1;
			contour[i].bMatched = false;
			
			
			// star hair in center
			hair[i].pos1 = pos;
			hair[i].pos2 = pos;
			hair[i].p = pos;
		}
		
		for(int i=0; i<MAX_NUM_SPIKES; i++) {
			spikeLength[i] = ofRandom(10.0, 60.0);
		}
		
		// set the eyes
		for(int i=0; i<numEyes; i++) {
			eyes[i].init(pos.x, pos.y);
			
			
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
			int   ind = pct * NUM_BUBBLE_PNTS;
			if(ind < 0) ind = 0;
			if(ind >= NUM_BUBBLE_PNTS) ind = NUM_BUBBLE_PNTS-1;
			
			
			bubbles[ind].pos.x = cntPoints[j].x;// * 2.0;
			bubbles[ind].pos.y = cntPoints[j].y;// * 2.0;
			
			
		}
		
	}
	//-------------------------------------------------------------- new radius
	void genNewRadius() {
		for(int i=0; i<NUM_BUBBLE_PNTS; i++) {
			bubbles[i].radiusD = ofRandom(16.0, 30.0);			
		}
	}
	
	//-------------------------------------------------------------- update
	void update() {
		
		
		age ++;
		
		if(age < 30) {
			
			for(int i=0; i<NUM_BUBBLE_PNTS; i++) {
				bubbles[i].radiusD = 10;
				bubbles[i].radius = 10;
				bubbles[i].pos = pos;
			}
		}
		
		// ITS MONSTER TIME
		else {
			bMonster = true;
		}
		
		
		
		ofPoint highestPnt;
		if(bMonster) {
			
			highestPnt.x = 0;
			highestPnt.y = 9999999;
			for(int i=0; i<NUM_BUBBLE_PNTS; i++) {
				
				// make the bubble bouncy
				bubbles[i].radiusV = (bubbles[i].radiusV * 0.5) + (bubbles[i].radiusD-bubbles[i].radius) / 10.0;
				bubbles[i].radius += bubbles[i].radiusV;
				
				if(bubbles[i].pos.y < highestPnt.y) {
					highestPnt = bubbles[i].pos;
				}
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
		
		if(bMonster) {
			float dy = rect.y+pos.y;
			highestPnt.y = dy/2;
			
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
		
		
		// history of centroid
		vel    = pos-prePos;
		prePos = pos;
		
		
		/*
		 // make the hari
		 ofxVec2f	norml, normlIn;
		 ofxVec2f	diff;
		 ofxPoint2f	mid;
		 ofxPoint2f	normalScaled;
		 
		 if(monsterMode == SPIKER_MONSTER) {
		 
		 for(int k=1; k<NUM_CONTOUR_PNTS; k++) {
		 
		 hair[k].update();
		 
		 float ni = (float)k / (float)(NUM_CONTOUR_PNTS-1.0);
		 int ind = ni * (int)contourConvex.size();
		 
		 
		 diff = contourConvex[ind] - contourConvex[ind-1];
		 diff.normalize();
		 norml.set(diff.y, -diff.x);
		 
		 
		 normalScaled  = contourConvex[k] + (norml * 40.0);				
		 
		 hair[k].pos1 = contourConvex[k];
		 hair[k].pos2 = normalScaled;
		 
		 }				
		 }
		 */
		
	}
	
	
	//-------------------------------------------------------------- draw
	void draw() {
		
		if(!bMonster) {
			
			// draw the shape of the body
			ofSetColor(monsterColor[colorID]);
			ofFill();
			ofBeginShape();
			for(int i=0; i<contourSimple.size(); i++) {
				ofVertex(contourSimple[i].x, contourSimple[i].y);
			}
			ofEndShape(true);
			
			return;
			
		}
		
		
		
		
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
			for(int i=0; i<NUM_BUBBLE_PNTS; i++) {
				ofCircle(bubbles[i].pos.x, bubbles[i].pos.y, bubbles[i].radius);	
			}
			
			
			// draw the shape of the body
			ofFill();
			ofBeginShape();
			for(int i=0; i<contourSmooth.size(); i++) {
				ofCurveVertex(contourSmooth[i].x, contourSmooth[i].y);
			}
			ofEndShape(true);
			
		}
		
		/*
		// ------------------------------------ MEGA a spiker monster (SMOOTHER)
		if(monsterMode == MEGA_SPIKER || monsterMode == MEGA_SPIKER_SOFT) {
			
			ofxVec2f	norml, normlIn;
			ofxVec2f	diff;
			ofxPoint2f	mid;
			ofxPoint2f	normalScaled;
			
			// draw the shape of the body
			ofFill();
			ofBeginShape();
			for(int i=0; i<contourSmooth.size(); i++) {
				ofCurveVertex(contourSmooth[i].x, contourSmooth[i].y);
			}
			ofEndShape(true);
			
			
			
			// the spike HELL YA
			for(int k=1; k<contourSimple.size(); k+=5) {
				
				diff = contourSimple[k] - contourSimple[k-1];
				diff.normalize();
				norml.set(diff.y, -diff.x);
				
				normlIn.set(-diff.y, diff.x);
				
				normalScaled  = contourSimple[k] + (norml * (50+(area*60)));				
				
				
				if(monsterMode == MEGA_SPIKER) {
					glLineWidth(6.0);
					ofLine(contourSimple[k].x, contourSimple[k].y,
						   normalScaled.x, normalScaled.y);
					glLineWidth(1.0);
				} else {
					
					
					float offset = 10;
					if(normalScaled.x > pos.x) offset = -10;
					
					glPushMatrix();
					glTranslatef(contourSimple[k].x+offset, contourSimple[k].y-20, 0);
					ofEllipse(0, 0, 10, 40);
					glPopMatrix();
				}
				
			}
		}
		*/
		
		// ------------------------------------  a spiker monster (convex)
		if(monsterMode == SPIKER_MONSTER) {
			
			// convex shape of the monster
			ofSetColor(monsterColor[colorID]);
			ofFill();
			ofBeginShape();
			for(int i=0; i<contourConvex.size(); i++) {
				ofVertex(contourConvex[i].x, contourConvex[i].y);
			}
			ofEndShape(true);
			/*
			 for(int i=0; i<NUM_CONTOUR_PNTS; i++) {
			 hair[i].draw();
			 }
			 */
			
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
		
		
		//	ofNoFill();
		//	ofRect(rect.x, rect.y, rect.width, rect.height);
		
		
		
		if(bDebug) {	
			// draw the vel of the monster
			float len = vel.length() * 20.0;
			float arrowsize = 30;
			ofxVec2f v;
			float ang = v.angle(vel);
			ofSetColor(255, 0, 0);
			glPushMatrix();
			glTranslatef(pos.x, pos.y, 0);
			glRotatef(ang, 0, 0, 1);
			ofLine(0,0,len,0);
			ofLine(len,0,len-arrowsize,+arrowsize/2);
			ofLine(len,0,len-arrowsize,-arrowsize/2);
			glPopMatrix();
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