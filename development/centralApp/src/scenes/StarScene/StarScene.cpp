/*
 *  simpleScene.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "StarScene.h"


void StarScene::setup(){
	tempC = (int)ofRandom(0, 255);
	PM.setupWorld();


	temp.loadImage("images/logoHalf.png");
	temp.setImageType(OF_IMAGE_GRAYSCALE);
	PM.findLockTarget(temp.getPixels(), temp.width, temp.height);
	

	bAllocatedYet = false;
	
	
	
}

void StarScene::update(){

	
	track();
	
	if (!bAllocatedYet){
		int w, h;
		w = stitchedImage->width;
		h = stitchedImage->height;
		
		videoGray.allocate(w,h);
		videoGraySmoothed.allocate(w,h);
		videoGrayTemporallySmoothed.allocate(w,h);
		videoGrayTemporallySmoothed2.allocate(w,h);
		bAllocatedYet  = true;
	}

	/*// always move 200 particles OFFSCREEN
	for (int i = 0; i < 200; i++){
		PM.particles[i].pos.set(OFFSCREEN_WIDTH*2 + i * 10, 500,0);
	}
*/
	

/*

	for (int i = 0; i < packet.nCentroids; i++){
		PM.particles[i].pos.set(packet.allCentroid[i].x * scalex, packet.allCentroid[i].y * scaley, 0);
		PM.particles[i].vel.set(0,0,0);
		PM.particles[i].energy = 1;						// BIG
	}

*/
	
	
	
	float scalex =  (float)OFFSCREEN_WIDTH / (float)videoGray.width;
	float scaley = (float)OFFSCREEN_HEIGHT / (float)videoGray.height;

	
	videoGray = *stitchedImage;
	videoGraySmoothed = videoGray;
	videoGray.erode_3x3();
	videoGray.dilate_3x3();
	videoGray.dilate_3x3();
	videoGray.dilate_3x3();
	videoGray.blur();
	videoGray.threshold(127);
	
	videoGraySmoothed -= 100;
	videoGraySmoothed += videoGray;
	videoGraySmoothed.blur();
	videoGraySmoothed.threshold(127);
	
	contours.findContours(videoGraySmoothed, 5, 10000, 40, false, false);
						  
		
	videoGrayTemporallySmoothed.set(0);
	
	for (int i = 0; i < contours.nBlobs; i++){
		
		
		for (int j = 0; j < SOBJ.size(); j++){
			if (SOBJ[j].bFoundThisFrame && SOBJ[j].whoThisFrame == i){
				float avgVel = SOBJ[j].avgVel;
				
				float pct = avgVel / 20.0f;
				if (pct > 1) pct = 1;
				
				if (pct > 0.1f){
					
					
					
					PM.particles.erase(PM.particles.begin());
					
					groupableParticle tempParticle;
					
					PM.particles.push_back(tempParticle);
					
					float velx = SOBJ[j].center.x - SOBJ[j].prevCenter.x;
					float vely = SOBJ[j].center.y - SOBJ[j].prevCenter.y;
					
					PM.particles[PM.particles.size()-1].setInitialCondition(SOBJ[j].center.x*scalex, SOBJ[j].center.y*scaley,0,0);
					//PM.particles[PM.particles.size()-1].addForce(velx*1000, vely*1000);
					PM.particles[PM.particles.size()-1].drag = ofRandom(0.0717, 0.08);
					PM.particles[PM.particles.size()-1].radius = 30;
					PM.particles[PM.particles.size()-1].energy = 0.20f;
					
					PM.VF.addIntoField(SOBJ[j].center.x / videoGray.width, SOBJ[j].center.y / videoGray.height, ofxVec2f(SOBJ[j].vel.x*0.001, SOBJ[j].vel.y*0.001), 0.08);
					//PM.particles[PM.particles.size()-1].findLockTarget(temp.getPixels(), temp.width, temp.height);
					/*
					 
					 //------------------------------------- make some particles
					 particles.assign(1400, groupableParticle());
					 
					 //------------------------------------- properties, like drag and pos, etc, etc.
					 for(int j = 0; j < particles.size(); j ++){
					 particles[j].pos.set(ofxVec3f(ofRandom(0,1793), ofRandom(0,1024), 0));
					 particles[j].vel.set(0,0,0);
					 float drag = ofRandom(0.0717, 0.08);
					 particles[j].drag = drag;
					 
					 particles[j].radius = 5;// + powf(ofRandom(0,1), 3) * 20;
					 //if (j == 10)  particles[j].radius = 30;
					 }
					 }
					 
					 */
					
					
				}
				
				if (pct > 0.01f)
				videoGrayTemporallySmoothed.drawBlobIntoMe(contours.blobs[i], pct*255);
			}
		}
		
		
	}
	
	
	videoGrayTemporallySmoothed.blur();
	
	videoGrayTemporallySmoothed2 -= 6;
	
	videoGrayTemporallySmoothed2 += videoGrayTemporallySmoothed;
	
	videoGrayTemporallySmoothed2.blurHeavily();
	
	
	
	
	
	PM.update();
						  /*
	 
						   
	 int minArea,
	 int maxArea,
	 int nConsidered,
	 bool bFindHoles,
	 bool bUseApproximation
	 */
						  
	
	//videoGrayTemporallySmoothed.blur();
	//videoGrayTemporallySmoothed += videoGraySmoothed;
	//videoGrayTemporallySmoothed -= 7;


}

void StarScene::announceChange(int change){
	ofxOscMessage msg;
	msg.setAddress("/bang");								//	bang
	msg.addStringArg("internalSceneChange");					//	handTrigger
	msg.addIntArg(4);									//	SCENE 4 (?)
	msg.addIntArg(change);									//	SCENE 4 (?)
	ofxDaito::sendCustom(msg);
}




void StarScene::keyPressed(int key){

	switch (key){
		case '0': announceChange(0); PM.mode = 0; break;
		case '1': announceChange(1); PM.mode = 1; break;
		case '2': announceChange(2); PM.mode = 2; break;
		case '3': announceChange(3); PM.mode = 3; break;
		case '4': announceChange(4); PM.mode = 4; break;
		case '5': announceChange(5); PM.mode = 5; break;
	}

}

void StarScene::draw(){

	
	ofSetColor(176,190,220);
	videoGrayTemporallySmoothed2.draw(0,0,OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	
	ofSetColor(255, 255, 255);
	PM.draw();
	
	
}

void StarScene::drawTop(){
	if (bAllocatedYet){
		videoGrayTemporallySmoothed2.draw(0,0);
		contours.draw(0,0);
	}
}



static int starTrackerId = 0;

void StarScene::track(){
	
	// TRACKING
	int nBlobs = contours.nBlobs;
	bool bFoundThisFrame [nBlobs];
	for (int i = 0;  i < nBlobs; i++){
		bFoundThisFrame[i] = false;
	}
	
	for (int i = 0; i < SOBJ.size(); i++){
		SOBJ[i].bFoundThisFrame = false;
		SOBJ[i].whoThisFrame = -1;
	}
	
	for (int i = 0; i < SOBJ.size(); i++){
		
		
		float minDistance = 1000000;
		int minIndex = -1;
		
		for (int j = 0; j < nBlobs; j++){
			
			if (bFoundThisFrame[j]) continue;
			// try to find the closest "hand" to this object, withing a range
			float diffx = SOBJ[i].center.x - contours.blobs[j].centroid.x;
			float diffy =  SOBJ[i].center.y - contours.blobs[j].centroid.y;
			float distance = sqrt(diffx * diffx + diffy*diffy);
			
			if (minDistance > distance){
				minDistance = distance;
				minIndex = j;
			}
		}
		
		
		if (minIndex != -1 && minDistance < 50){
			bFoundThisFrame[minIndex] = true;
			// kk we are close, so let's mark and keep rolling:
			SOBJ[i].center.set(contours.blobs[minIndex].centroid.x, contours.blobs[minIndex].centroid.y, 0);
			
			float dx = SOBJ[i].center.x - SOBJ[i].prevCenter.x;
			float dy = SOBJ[i].center.y - SOBJ[i].prevCenter.y;
			float speed = sqrt(dx*dx + dy*dy);
			
			SOBJ[i].vel = ofxVec2f(dx, dy);
			
			SOBJ[i].prevCenter.set(SOBJ[i].center.x, SOBJ[i].center.y);
			SOBJ[i].avgVel = 0.91 * SOBJ[i].avgVel + 0.09f * speed;
			
			
			
			SOBJ[i].bFoundThisFrame = true;
			SOBJ[i].whoThisFrame = minIndex;
			SOBJ[i].energy += 0.1f;
			SOBJ[i].energy = MIN(SOBJ[i].energy, 1);
		}
	}
	
	
	// ok for ALL non found blobs, add them to the vector
	for (int i = 0;  i < nBlobs; i++){
		if (bFoundThisFrame[i] == false){
			// make some $ in SOBJ
			starObject temp;
			SOBJ.push_back( temp);
			SOBJ[SOBJ.size()-1].bFoundThisFrame = true;
			SOBJ[SOBJ.size()-1].whoThisFrame = i;
			SOBJ[SOBJ.size()-1].energy = 0.3;
			SOBJ[SOBJ.size()-1].id = starTrackerId;
			SOBJ[SOBJ.size()-1].center.x = contours.blobs[i].centroid.x;
			SOBJ[SOBJ.size()-1].center.y = contours.blobs[i].centroid.y;
			SOBJ[SOBJ.size()-1].prevCenter.set(SOBJ[SOBJ.size()-1].center.x, SOBJ[SOBJ.size()-1].center.y);
			SOBJ[SOBJ.size()-1].avgVel = 0;
			
			SOBJ[i].vel.set(0,0);
			
			starTrackerId ++;
			bFoundThisFrame[i] = true;
		}
	}
	
	
	// do some other stuff, delete off old ones:
	for (int i = 0;  i < SOBJ.size(); i++){
		if (SOBJ[i].bFoundThisFrame == false){
			SOBJ[i].energy *= 0.9f;
		}
	}
	
	std::vector<starObject >::iterator iter = SOBJ.begin();
	while (iter != SOBJ.end())
	{
		if ((*iter).energy <= 0.05)
		{
			
			
			iter = SOBJ.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	
	
}
