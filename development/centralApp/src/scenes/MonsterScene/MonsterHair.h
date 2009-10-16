

#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"

class MonsterHair {
	
	
public:
	
	ofxVec2f pos1, pos2, p, v, a;
	
	MonsterHair() {
		p = 0;
		v = 0;
		a = 0;
		pos1 = 0;
		pos2 = 0;
	}
	
	
	void update() {
		// Spring
		float M = 0.8;   // Mass
		float K = 1.2;   // Spring constant
		float D = 0.92;  // Damping
		
		ofxVec2f force = -K * (p - pos2);    // f=-ky
		a = force / M;							
		v = D * (v + a);						
		p = p + v;	
		
	}
	
	void draw() {
		ofSetColor(255, 0, 0);
		glLineWidth(4.09);
		ofLine(pos1.x, pos1.y, p.x, p.y);
		glLineWidth(1.0);
	}
};