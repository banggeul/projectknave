

#pragma once

#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxBox2d.h"


class PaintBall {
	
public:
	
	int								res;
	ofxBox2d *						world;
	ofxBox2dJoint					*joints;
	ofxBox2dCircle					*balls;
	ofxBox2dJoint					*jointsSoft;

	ofxBox2dCircle					center;
	float							width, height;
	float							damping;
	
	// ------------------------------------------------
	PaintBall() {
		world = NULL;
	}
	
	
	// ------------------------------------------------ build the blob
	void buildTheBlob(float x, float y) {
		
		res		 = 10;
		width = ofRandom(40, 90);
		height = width/ 1.30;// ofRandom(40, 90);

		damping  = 0.72;
		
		
		center.setPhysics(1.0, 0.53, 0.1);
		center.setup(world->getWorld(), x, y, 10);		
		
		jointsSoft = new ofxBox2dJoint[res];
		joints = new ofxBox2dJoint[res];
		balls = new ofxBox2dCircle[res];
		
		// the balls
		for(int i=0; i<res; i++) {
			float ni = (float)i / (float)(res-1.0);
			float px = x + (cos(ni * TWO_PI) * width); 
			float py = y + (sin(ni * TWO_PI) * height);
			balls[i].setPhysics(1.0, 0.53, 0.1);
			balls[i].setup(world->getWorld(), px, py, ofRandom(4, 10), false);		
		}
		
		// connect all the ball joints
		for(int i=1; i<res; i++) {
			joints[i].setWorld(world->getWorld());
			joints[i].addJoint(balls[i].body, balls[i-1].body, 3.0, damping);
			if(i == res-1) {
				joints[0].setWorld(world->getWorld());
				joints[0].addJoint(balls[res-1].body, balls[0].body, 3.0, damping);
			}
		}
		
		// connect all the ball joints
		for(int i=0; i<res; i++) {
			jointsSoft[i].setWorld(world->getWorld());
			jointsSoft[i].addJoint(balls[i].body, center.body, width * 0.08, damping);
		}
		
	}
	
	
	// ------------------------------------------------ init
	void init(ofxBox2d * bWrld, float x, float y) {
		world =	bWrld;
		if(world) {
			
			buildTheBlob(x, y);
			
		}
		else {
			printf("- i need a box2d world -\n");	
		}
	}
	
	// ------------------------------------------------
	
	void update() {
	}
	// ------------------------------------------------
	
	void draw() {
	/*
		for(int i=0; i<res; i++) {
			balls[i].draw();
			joints[i].draw();
			jointsSoft[i].draw();
		}
		*/
		center.draw();
		
		ofFill();
		ofBeginShape();
		for(int i=0; i<res; i++) {

			float ni = (float)i / (float)(res-1.0); 
			ofPoint p1 = balls[i].getPosition();
			ofSetColor(ni*255, 0, 255);
			ofCurveVertex(p1.x, p1.y);
			
			if(i == 0 || i == res-1) {
				ofCurveVertex( balls[res-1].getPosition().x, balls[res-1].getPosition().y);
				ofCurveVertex(p1.x, p1.y);
			}
		}		
		ofEndShape();
		
	
	}
	
	
};







