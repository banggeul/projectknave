

#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxSvg.h"


class MonsterEye {
	
	
public:
	float	 size, sizeD, sizeV;
	
	ofxVec2f pos, p, v, a;
	ofxVec2f eb, ev;
	
	
	MonsterEye() {
		size = 0;
		sizeD = 0;
		sizeV = 0;
		eb = 0;
		p = 0; v = 0; a = 0;
		ev = 0;
		size = ofRandom(10, 30);
	}
	
	
	void init(float _x, float _y) {
		pos.set(_x, _y);
		p.set(_x, _y);
		eb.set(_x, _y);
	}
	
	void update() {
		/* to springy
		// Spring
		float M = 1.8;   // Mass
		float K = 1.2;   // Spring constant
		float D = 0.92;  // Damping
		
		ofxVec2f force = -K * (p - pos);    // f=-ky
		a = force / M;							
		v = D * (v + a);						
		p = p + v;	
		*/
		
		p += (pos-p) / 3.0;
		
		ev = (ev * 0.7) + (ofPoint(pos.x, pos.y+10)-eb) / 8.0;
		eb += ev;
		
	}
	
	
	void draw() {
		
		ofFill();
		ofSetColor(0xffffff);
		ofCircle(p.x, p.y, size);
		ofSetColor(0x000000);
		ofCircle(eb.x, eb.y, size/3.5);
		
	}
	
};