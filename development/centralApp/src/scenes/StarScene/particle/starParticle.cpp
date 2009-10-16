

#include "starParticle.h"


starParticle::starParticle(){
    drag            = 0.3;
    pos = ofxVec3f(0,0,0);
    vel = ofxVec3f(0,0,0);
    frc = ofxVec3f(0,0,0);
	
	energy = 0;
	radius = 4;// + ofRandom(0,6);
	bBeenPeaked = false;
	nFrameOfLastPeaking = 0;
	fiveEnergy = 0;
}





//--------------------------------------------
void starParticle::addForce(ofxVec3f force){
    vel.x += force.x;
    vel.y += force.y;
}


//--------------------------------------------
void starParticle::reset(){
    //bBeenChosen = false;
}



//---------------------------------------------------------------------- p5 style...

//--------------------------
// contructor:
/*
 void starParticle::starParticle(){
 pos = new ofxVec2f(0,0);
 vel = new ofxVec2f(0,0);
 frc = new ofxVec2f(0,0);
 }*/

//--------------------------
void starParticle::resetForce(){
    // we reset the forces every frame
    frc.set(0,0,0);
}

//--------------------------
void starParticle::setInitialCondition(float px, float py, float vx, float vy){
    pos.x = px;
    pos.y = py;
    vel.x = vx;
    vel.y = vy;
}

void starParticle::addRepulsionForce(starParticle & p, float radius, float scale){
	float x = p.pos.x;
	float y = p.pos.y;
	bool bAmCloseEnough = true;
	if (radius > 0){
		if (sqrt((x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y)) > radius){
			bAmCloseEnough = false;
		}
	}
	
	if (bAmCloseEnough == true){
		
		ofxVec2f forceVec = ofxVec2f(0,0);
		forceVec.x = x-pos.x;
		forceVec.y = y-pos.y;
		forceVec.normalize();
		forceVec.x *= scale;
		forceVec.y *= scale;
		frc.x = frc.x - (forceVec.x );
		frc.y = frc.y - (forceVec.y );
		p.frc.x = p.frc.x + (forceVec.x );
		p.frc.y = p.frc.y + (forceVec.y );
	}
}


void starParticle::addAttractionForce(starParticle & p, float radius, float scale){
	float x = p.pos.x;
	float y = p.pos.y;
	
	bool bAmCloseEnough = true;
	if (radius > 0){
		if (sqrt((x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y)) > radius){
			bAmCloseEnough = false;
		}
	}
	
	if (bAmCloseEnough == true){
		
		ofxVec2f forceVec = ofxVec2f(0,0);
		forceVec.x = x-pos.x;
		forceVec.y = y-pos.y;
		forceVec.normalize();
		forceVec.x *= scale;
		forceVec.y *= scale;
		frc.x = frc.x + forceVec.x;
		frc.y = frc.y + forceVec.y;
		p.frc.x = p.frc.x - forceVec.x;
		p.frc.y = p.frc.y - forceVec.y;
	}
}

void starParticle::addRepulsionForce(float x, float y, float radius, float scale){
	bool bAmCloseEnough = true;
	if (radius > 0){
		if (sqrt((x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y)) > radius){
			bAmCloseEnough = false;
		}
	}
	
	if (bAmCloseEnough == true){
		ofxVec2f forceVec = ofxVec2f(0,0);
		forceVec.x = x-pos.x;
		forceVec.y = y-pos.y;
		forceVec.normalize();
		frc.x = frc.x - forceVec.x * scale;
		frc.y = frc.y - forceVec.y * scale;
	}
}

void starParticle::addAttractionForce(float x, float y, float radius, float scale){
	
	bool bAmCloseEnough = true;
	if (radius > 0){
		if (sqrt((x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y)) > radius){
			bAmCloseEnough = false;
		}
	}
	
	if (bAmCloseEnough == true){
		ofxVec2f forceVec = ofxVec2f(0,0);
		forceVec.x = x-pos.x;
		forceVec.y = y-pos.y;
		forceVec.normalize();
		forceVec.x *= scale;
		forceVec.y *= scale;
		frc.x = frc.x + forceVec.x;
		frc.y = frc.y + forceVec.y;
	}
}

//--------------------------
void starParticle::addForce(float x, float y){
	// add in a force in X and Y for this frame.
	frc.x = frc.x + x;
	frc.y = frc.y + y;
}

//--------------------------
void starParticle::addDampingForce(){
	float damping = 0.05;
	frc.x = frc.x - (vel.x * damping);
	frc.y = frc.y - (vel.y * damping);
}

//--------------------------
void starParticle::update(){
    vel.x = vel.x + frc.x;
    vel.y = vel.y + frc.y;
    pos.x = pos.x + vel.x;
    pos.y = pos.y + vel.y;
	
	energy *= 0.989f;
}


//--------------------------
void starParticle::findLockTarget(unsigned char * pixels, int width, int height){
	
	//cout << "heeeer \n";
	bool bfoundWhite = false;
	while (bfoundWhite == false){
		
		int randW = (int)ofRandom(0,width-1);
		int randH = (int)ofRandom(0,height-1);
		if (pixels[randH * width + randW] > 10){
			lockTarget.set(randW*2, randH*2, 0);
			//pos.set(randW*2, randH*2, 0);
			bfoundWhite = true;
			//cout << "2 \n";
		} else {
			//cout << "1 \n";
			//cout << "looking again \n";
		}
		
	}
	
	
}


//--------------------------
void starParticle::draw(){
    //noStroke();
    //fill(198,246,55);
    //ellipse(pos.x, pos.y, 15,15);
}


bool starParticle::bInSameBin(starParticle &p){
    if ((bitFlagW & p.bitFlagW) && (bitFlagH & p.bitFlagH)) return true;
    return false;
}






