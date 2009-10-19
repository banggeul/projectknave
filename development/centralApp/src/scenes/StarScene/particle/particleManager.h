#pragma once


#include "ofMain.h"
#include "ofxVectorMath.h"
#include "starParticle.h"
#include "binner.h"
#include "vectorField.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "del_interface.hpp"
#include "groupableParticle.h"
#include "ImprovedNoise.h"

#include "ofxShader.h"
#include "ofxDaito.h"

#define NUM_PARTICLES_VBO 1400

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::ifstream;
using std::string;

using namespace tpp;


class vfAdder{
	
public: // move the public 
	ofxVec2f	radius;
	ofxVec2f	center;
	float		speedx;
	float		speedy;
	float		angle;
	float		timeOn;
	float		startTime;
	ofxVec2f	point;
	ofxVec2f	lastPoint;
	ofxVec2f	diffPoint;

	float		angleAdder;
	float		myAngle;
	float angleAdderSlow;
	
	void setup(){
		angleAdderSlow = 0;
		center.set(ofRandom(0+300,1793-300), ofRandom(0+300,1024-300)); 
		radius.set(ofRandom(200,350), ofRandom(200,300));
		speedx = ofRandom(-1,1);
		speedy = ofRandom(-1,1);
		angle = ofRandom(0,TWO_PI);
		timeOn = ofRandom(1,2.5);
		startTime = ofGetElapsedTimef();
		angleAdder = ofRandom(-0.05,0.05);
		
		point.x = center.x + radius.x * cos ( angle * speedx);
		point.y = center.y + radius.y * sin ( angle * speedy);

		lastPoint = point;
		diffPoint.set(0,0);
		
		myAngle = 0;
	}
	
	void update(){
		
		angleAdderSlow = 0.97f * angleAdderSlow + 0.03f * angleAdder;
		
		point.x = center.x + radius.x * cos ( angle * speedx);
		point.y = center.y + radius.y * sin ( angle * speedy);
		
		myAngle = atan2(point.y - lastPoint.y, point.x - lastPoint.x);
		
		angle += angleAdderSlow;
		angleAdder *= 0.99f;
		diffPoint = point - lastPoint;
		lastPoint = point;
	}
	
	void draw(){
		
		if (ofGetElapsedTimef() - startTime < timeOn){ 
			ofCircle(point.x, point.y, 5);
		}
	}
	
	
};


class particleManager{

    public:

        particleManager();

        void setupWorld();
        void update();
        void draw();
        void setTrackedPoint(float x, float y);

        void ghettoAdd();

        void assembleParticleGroups();
        void disassembleParticleGroup();

		vector <groupableParticle> particles;
	
		void findLockTarget(unsigned char * pixels, int width, int height){
			for (int i = 0; i < particles.size(); i++){
				particles[i].findLockTarget(pixels, width, height);
			}
		}
	
		float	getRads(float $val1, float $val2, float zoff, float $mult, float $div);
	
	
		//vector <particleGroup>  particleGroups;
        //particleShapeDescriber * PSD;
        ofxVec3f center;

        // ----------------------------------
        void handleMousePress(float x, float y) { prevPos.set(x,y); }
        void handleMouseDrag(float x, float y);
        ofxVec2f prevPos;
        vectorField VF;
		
		vector < vfAdder > VFs;
	
		
		void computeStatistics();
		ofxVec2f avgVelocity;
		ofxVec2f avgPosition;
		ofxVec2f	stdDeviation;
	
		
	
        void triangulateParticles();
        vector< Delaunay::Point > v;
	
		ImprovedNoise * impNoise;
		float minNoise ;//= 0.499;
		float maxNoise ;//= 0.501;
	
	int mode;
	
	// image of the dot
	float		maxAlpha, maxPtsSize;
	ofxShader   shader;
	ofImage		theDot;
	GLuint		vbo[2];
	float		vboPts[NUM_PARTICLES_VBO][3];
	float		vboColor[NUM_PARTICLES_VBO][4];
	float		vboPtsSize[NUM_PARTICLES_VBO];
	
	
	

};

