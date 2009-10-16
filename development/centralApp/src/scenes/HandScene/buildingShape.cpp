

#include "buildingShape.h"



buildingShape::buildingShape(){
	energy = 0;
	animator = ofRandom(0,1);
	direction = (ofRandom(0,1) > 0.5) ? -1 : 1;
	bSetupList = false;
	bHasPeakedWhileTouched = false;
	bAnyInsideMeFromLastFrame = false;
	peakEnergy = 0;
	peakMakeStuffSick = 0;
	bPeakedThisFrame = false;
	nFramesSinceTrigger = 0;
	type = 0;

	myColor.set(255,0,255);
	myColorTarget.set(255,0,255);


}

void buildingShape::setupList(){
	bSetupList = true;
	DL.begin();
	ofFill();
	ofBeginShape();
	for (int i= 0; i < nPts; i++){
		ofVertex(pts[i].x, pts[i].y);
	}
	ofEndShape(true);
	DL.end();

}



void buildingShape::draw(){

	if (energy < 0.01f) return;

	ofSetColor(myColor.x, myColor.y, myColor.z, powf(energy,3)*255*(0.25 + peakEnergy*0.65));
	/*ofFill();
	//ofRect(boundingRect.x, boundingRect.y, boundingRect.width, boundingRect.height);
	ofBeginShape();
	for (int i= 0; i < nPts; i++){
		ofVertex(pts[i].x, pts[i].y);
	}
	ofEndShape(true);
	*/

	if (!DL.ready()){
		setupList();
	} else {
		DL.draw();
	}

	ofSetColor(255, 255, 255, energy*255);

	int posInArray = MAX(MIN(animator * nPts, nPts-1),0);
	ofSetColor(myColor.x, myColor.y, myColor.z, energy*255);

	ofFill();
	ofPoint temp = pts[posInArray];
	//ofCircle(temp.x, temp.y, 8);

	ofNoFill();
	ofPushStyle();
	glLineWidth(BUILDING_HIGHLIGHT_SIZE);
	ofBeginShape();
	for (int i = 0; i < trail.size(); i++){
		ofVertex(trail[i].x, trail[i].y);
	}
	ofEndShape();
	ofPopStyle();

}


void buildingShape::update(){


	myColor.x = 0.93f * myColor.x + 0.07f * myColorTarget.x;
	myColor.y = 0.93f * myColor.y + 0.07f * myColorTarget.y;
	myColor.z = 0.93f * myColor.z + 0.07f * myColorTarget.z;


	energy*=0.95f;


	float lengthPct = (length - 50) / 700.0f;
	if (lengthPct < 0) lengthPct = 0;
	if (lengthPct > 1) lengthPct = 1;

	animator += (energy + peakMakeStuffSick*0.3) * (0.02f - 0.015f*lengthPct) * 2 * direction;

	while (animator < 0) animator += 1;
	while (animator > 1) animator -= 1;


	float posInArrayF = animator * nPts;
	int posInArray = MAX(MIN((int)posInArrayF, nPts-1),0);
	float diff = posInArrayF - posInArray;


	int pos_p_1 = (posInArray + 1) % nPts;
	float x = (1-diff) * pts[posInArray].x + (diff) * pts[pos_p_1].x;
	float y = (1-diff) * pts[posInArray].y + (diff) * pts[pos_p_1].y;


	ofPoint temp;
	temp.set(x,y);
	trail.push_back(temp);

	if (trail.size() > 30){
		trail.erase(trail.begin());
	}


	if (bAnyInsideMeFromLastFrame == false){
		bHasPeakedWhileTouched = false;
	}
	bAnyInsideMeFromLastFrame = false;

	if (nFramesSinceTrigger > 15 && energy > 0.94f){

		//cout << "peaking ! \n" << endl;
		peakEnergy = 1;
		peakMakeStuffSick = 1;
		bHasPeakedWhileTouched = true;
		bPeakedThisFrame = true;
		nFramesSinceTrigger = 0;
	}

	peakEnergy *= 0.96f;
	peakMakeStuffSick *= 0.99;

	nFramesSinceTrigger ++;


}

void buildingShape::checkInside(float x, float y, ofPoint colorMe){
	if (x > boundingRect.x && x < (boundingRect.x + boundingRect.width)){
		if (y > boundingRect.y && y < (boundingRect.y + boundingRect.height)){
			energy = MIN(energy+0.04, 1);
			bAnyInsideMeFromLastFrame = true;
			myColorTarget.set(colorMe.x, colorMe.y, colorMe.z);
		}
	}
}
