

#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"
#include "cvStrokeResampler.h"

#define RES 30

class WiggleMonster {
	
public:
	
	bool	 dead;
	bool	 bDebug;
	
	
	float cw,	ch;
	float sclw, sclh;
	
	cvStrokeResampler rs;
	
	int		 id;
	
	
	ofxVec2f pos;
	
	ofxVec2f ptsA[RES];
	ofxVec2f ptsB[RES];
	ofxVec2f ptsC[RES];
	
	ofxVec2f vels[RES];
	
	// ---------------------------------
	WiggleMonster() {
	
		bDebug = true;
		dead   = false;
		id	   = -1;
		
		for(int i=0; i<RES; i++) {
			ptsA[i]  = 0;
			ptsB[i]  = 0;
			ptsC[i]  = 0;
			
			vels[i]  = 0;
		}
		
		cw  = 640;
		ch  = 240;
		
		sclw = 640.0;
		sclh = 240.0;
		
	}
	
	// ---------------------------------
	void init(ofPoint startPnt) {
		for(int i=0; i<RES; i++) {
			
			ptsA[i].x  = (startPnt.x / cw) * sclw;
			ptsA[i].y  = (startPnt.y / ch) * sclh;
			
			ptsB[i].x  = (startPnt.x / cw) * sclw;
			ptsB[i].y  = (startPnt.y / ch) * sclh;
			
			ptsC[i].x  = (startPnt.x / cw) * sclw;
			ptsC[i].y  = (startPnt.y / ch) * sclh;
			
			pos.x  = (startPnt.x / cw) * sclw;
			pos.y  = (startPnt.y / ch) * sclh;
		}
		
	}
	
	// ---------------------------------
	void update() {
		
		if(bDebug) return;
		
		for(int i=0; i<RES; i++) {
			vels[i] = (vels[i] * 0.9) + (ptsC[i]-ptsB[i]) / 14.0; 
			ptsB[i] += vels[i];
		}
	}
	
	
	// ---------------------------------	
	void updatePnts(vector <ofPoint> &cpts) {
		
		if(bDebug) return;
		
		rs.clear();
		ofPoint temp;
		for(int i=0; i<cpts.size(); i++) {
			temp = cpts[i];
			rs.addPoint(temp.x, temp.y);
		}	
		rs.resample(RES);
		
		
		
		for(int i=1; i<rs.pts.size(); i++) {
			
			ofxVec2f mid = rs.pts[i] - rs.pts[i-1];
			mid.normalize();
			
			ofxVec2f normL(-mid.y, mid.x);
			ofxVec2f normV = rs.pts[i] + (normL * 30.0);
			
			ptsA[i].x = (rs.pts[i].x / cw) * sclw;	// anchor
			ptsA[i].y = (rs.pts[i].y / ch) * sclh;
			
			ptsC[i].x = (normV.x / cw) * sclw;	// destination
			ptsC[i].y = (normV.y / ch) * sclh;
			
			
		}
	}
	
	
	// ---------------------------------
	void draw() {
		
		ofEnableAlphaBlending();
		ofFill();
		if(dead) ofSetColor(0, 0, 0, 100);
		else ofSetColor(255, 25, 0, 100);
		
		ofCircle(pos.x, pos.y, 30);
		
		ofSetColor(255, 255, 250);
		ofDrawBitmapString(ofToString(id), pos.x, pos.y);
		ofDisableAlphaBlending();
		
		if(bDebug) return;
		
		if(dead) return;
		
		int wiggleMode = 0;
		
		if(wiggleMode == 0) {
			ofSetColor(255, 25, 50);
			for(int i=0; i<RES; i++) {
				ofLine(ptsA[i].x, ptsA[i].y, ptsB[i].x, ptsB[i].y);
				ofCircle(ptsA[i].x, ptsA[i].y, 2);
				ofCircle(ptsB[i].x, ptsB[i].y, 3);
			}
			
		}
		
		else {
			glBegin(GL_POLYGON);
			for(int i=1; i<RES; i++) {
				float n = (float)i/(float)(RES-1);
				glColor3f(n, .3, 1.0-n);
				
				glVertex3f(ptsB[i].x, ptsB[i].y, 0);
				glVertex3f(pos.x, pos.y, 0);
				glVertex3f(ptsB[i-1].x, ptsB[i-1].y, 0);
				
			}	
			glEnd();
			
		}
		
	}
	
};