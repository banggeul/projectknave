

#include "buildingShape.h"



buildingShape::buildingShape(){
	energy = 0;
	
	animator = ofRandom(0,1);
	direction = (ofRandom(0,1) > 0.5) ? -1 : 1;
}



void buildingShape::draw(){
	
	if (energy < 0.01f) return;
	ofSetColor(255, 0, 255, energy*255);
	
	ofNoFill();
	//ofRect(boundingRect.x, boundingRect.y, boundingRect.width, boundingRect.height);
	ofBeginShape();
	for (int i= 0; i < nPts; i++){
		//ofVertex(pts[i].x, pts[i].y);
	}
	ofEndShape(true);
	
	int posInArray = MAX(MIN(animator * nPts, nPts-1),0);
	ofSetColor(255, 0, 255, energy*255);
	
	ofFill();
	ofPoint temp = pts[posInArray];
	//ofCircle(temp.x, temp.y, 8);
	
	ofNoFill();
	ofBeginShape();
	for (int i = 0; i < trail.size(); i++){
		ofVertex(trail[i].x, trail[i].y);	
	}
	
	
	
	
	ofEndShape();
}


void buildingShape::update(){
	energy*=0.99f;
	
	animator += energy * 0.02f * direction;
	
	while (animator < 0) animator += 1;
	while (animator > 1) animator -= 1;
	
	int posInArray = MAX(MIN(animator * nPts, nPts-1),0);
	ofPoint temp = pts[posInArray];
	trail.push_back(temp);
	
	if (trail.size() > 30){
		trail.erase(trail.begin());
	}
	
}

void buildingShape::checkInside(float x, float y){
	if (x > boundingRect.x && x < (boundingRect.x + boundingRect.width)){
		if (y > boundingRect.y && y < (boundingRect.y + boundingRect.height)){
			
			energy = MIN(energy+0.5, 1);
		}
	}
}