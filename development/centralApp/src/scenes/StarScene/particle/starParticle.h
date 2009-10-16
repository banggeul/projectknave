#pragma once


#define OF_ADDON_USING_OFXVECTORMATH
#include "ofMain.h"
#include "ofxVectorMath.h"

class starParticle {

	public:


        ofxVec3f pos;
        ofxVec3f vel;
        ofxVec3f frc;
	
		ofxVec3f lockTarget;
		void findLockTarget(unsigned char * pixels, int width, int height);
	

        starParticle();
	
		float radius;
	

        void        resetForce();
        void        setInitialCondition(float px, float py, float vx, float vy);
        void        addRepulsionForce(starParticle &p, float radius, float scale);
        void        addAttractionForce(starParticle &p, float radius, float scale);
        void        addRepulsionForce(float x, float y, float radius, float scale);
        void        addAttractionForce(float x, float y, float radius, float scale);
        void        addForce(float x, float y);
        void        addForce(ofxVec3f force);
        void        addDampingForce();
        void        draw();
        void        update();
        void        reset();
		float       drag;
		
		float		energy;
		void		peakMe(float amount){
			if ((ofGetFrameNum()	- nFrameOfLastPeaking) > 50){
				if (amount > energy) energy = amount;
				nFrameOfLastPeaking = ofGetFrameNum();
				bBeenPeaked = true;
			}
		}
		int			nFrameOfLastPeaking;
		bool		bBeenPeaked;
	
	
		bool		bBeenDirectionallyPeaked;
		float		direction;
		void		directionallyPeakMe(float amount, float angle){
			if ((ofGetFrameNum()	- nFrameOfLastPeaking) > 50){
				if (amount > energy) energy = amount;
				direction = angle;
				nFrameOfLastPeaking = ofGetFrameNum();
				bBeenDirectionallyPeaked = true;
			}
		}
	
	float fiveEnergy;

		bool        bInSameBin(starParticle &p);
        unsigned        bitFlagW;
        unsigned        bitFlagH;
};





