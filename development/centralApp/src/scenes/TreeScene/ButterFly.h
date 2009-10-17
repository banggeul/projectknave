

#pragma once
#include "ofMain.h"
#include "particle.h"

class ButterFly : public particle {
	
	
public:
	
	
	float radius;
	
	ButterFly() {
		
		
		
	}
	
	void setupButterfly() {
		
		radius = 20;
		seperation.distance		= radius *2;
		alignment.distance		= 80;
		cohesion.distance		= 90;
		damping = 0.020;//0.07;
		
		
		seperation.strength		= .93;
		alignment.strength		= .15;
		cohesion.strength		= .15;
		
	}
	
	
	void draw() {
		
		ofSetColor(255, 0, 255);
		ofFill();
		ofCircle(pos.x, pos.y, radius);
		
		ofxVec2f velNormal = vel;
		velNormal.normalize();
		
		ofxVec2f velPerp;
		velPerp.x = -velNormal.y;
		velPerp.y = velNormal.x;
		
		ofLine(pos.x, pos.y, pos.x + velNormal.x*10, pos.y + velNormal.y*10);
		
	}
	
	
};