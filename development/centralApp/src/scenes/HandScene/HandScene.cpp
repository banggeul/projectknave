/*
 *  simpleScene.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "HandScene.h"


void HandScene::setup(){

	// box2d
	box2d.init();
	box2d.setGravity(0, 30);
	box2d.checkBounds(true);
	box2d.setFPS(16.0);
	printf("box2d allocated\n");

	// the ferry building contour
	ferryBuilding.setupBuilding("buildingRefrences/buidlingFiles/handsBuildingContour.xml");
	// hit space to make a ferry building contour -- needs work


	// make a big ball
	HandBall bub;
	bub.setPhysics(3.0, 0.53, 0.1); // mass - bounce - friction
	bub.setup(box2d.getWorld(), OFFSCREEN_WIDTH/2, 120, 50);
	balls.push_back(bub);
	floor.setup(box2d.getWorld(), 0, OFFSCREEN_HEIGHT-20, OFFSCREEN_WIDTH, 20, true);

	// ----------


	handImgW = 640;
	handImgH = 480;


	handImage.setUseTexture(false);
	handImage.allocate(handImgW, handImgH);

	handImageTemporallyBlurred.setUseTexture(false);
	handImageTemporallyBlurred.allocate(handImgW, handImgH);
	handImageTemporallyBlurredInvert.allocate(handImgW, handImgH);

	handImage.preAllocate(MAX_BLOB_LENGTH);
	lastFrameRendered = -1;

	//------------------------------------------- shape finding
	roof.loadImage("buildingRefrences/itoMasks/roof.png");
	roof.setImageType(OF_IMAGE_GRAYSCALE);
	width = roof.width;
	height = roof.height;
	printf("loaded roof, width %i height %i \n", width, height);
	grayImage.allocate(width,height);
	grayImage.setFromPixels(roof.getPixels(), width, height);
	contourFinder.findContours(grayImage, 20, width*height, 100, false);	// find holes


	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/2WindowsOnly.bin", 0);
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/Clock-Clock.bin", 1);
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/ClockOnly.bin", 2);
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/Columns.bin", 3);
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/Roof1.bin", 4);
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/Roof2.bin",5);
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/StripeArea1.bin",6);
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/StripeArea2.bin",7);
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/Wall1.bin",8);
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/WindowFrames.bin",9);
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/WindowTop_Only.bin",10);

	/*
	for (int i = nShapes; i < nShapes2; i++){

		ofxBox2dRect temp;
		temp.setup(box2d.getWorld(),  BSM.shapes[i]->boundingRect.x,  BSM.shapes[i]->boundingRect.y, BSM.shapes[i]->boundingRect.width/2, BSM.shapes[i]->boundingRect.height/2, true);

	}*/


	FBO.allocate(OFFSCREEN_WIDTH/5,OFFSCREEN_HEIGHT/5, false);	// fixed?

	//neon.loadImage("image/Scene_01_Neon.png");
	//neon.setImageType(OF_IMAGE_COLOR);	// RGB yah
	//neonPixels = neon.getPixels();
}

static int trackerId = 0;
//--------------------------------------------------------------
void HandScene::update(){



	// TRACKING


	int nBlobs = packet.nBlobs;
	bool bFoundThisFrame [nBlobs];
	for (int i = 0;  i < nBlobs; i++){
		bFoundThisFrame[i] = false;
	}

	for (int i = 0; i < HOBJ.size(); i++){
		HOBJ[i].bFoundThisFrame = false;
		HOBJ[i].whoThisFrame = -1;
	}

	for (int i = 0; i < HOBJ.size(); i++){


		float minDistance = 1000000;
		int minIndex = -1;

		for (int j = 0; j < nBlobs; j++){

			if (bFoundThisFrame[j]) continue;
			// try to find the closest "hand" to this object, withing a range
			float diffx = HOBJ[i].center.x - packet.centroid[j].x;
			float diffy =  HOBJ[i].center.y - packet.centroid[j].y;
			float distance = sqrt(diffx * diffx + diffy*diffy);

			if (minDistance > distance){
				minDistance = distance;
				minIndex = j;
			}
		}


		if (minIndex != -1 && minDistance < 200){
				bFoundThisFrame[minIndex] = true;
				// kk we are close, so let's mark and keep rolling:
				HOBJ[i].center.set(packet.centroid[minIndex].x, packet.centroid[minIndex].y, 0);
				HOBJ[i].bFoundThisFrame = true;
				HOBJ[i].whoThisFrame = minIndex;
				HOBJ[i].energy += 0.1f;
				HOBJ[i].energy = MIN(HOBJ[i].energy, 1);
		}
	}


	// ok for ALL non found blobs, add them to the vector
	for (int i = 0;  i < nBlobs; i++){
		if (bFoundThisFrame[i] == false){
				// make some $ in HOBJ
			handObject temp;
			HOBJ.push_back( temp);
			HOBJ[HOBJ.size()-1].bFoundThisFrame = true;
			HOBJ[HOBJ.size()-1].whoThisFrame = i;
			HOBJ[HOBJ.size()-1].energy = 0.3;
			HOBJ[HOBJ.size()-1].id = trackerId;
			HOBJ[HOBJ.size()-1].center.x = packet.centroid[i].x;
			HOBJ[HOBJ.size()-1].center.y = packet.centroid[i].y;

			// let's pick a random color word:


			// this wasn't so great.  maybe if they were SUPER bright, the problem is I kind of need equality in terms of brightness
			// across the color spectrum.  so I bumped to something else....
			/*
			float xp = (int)(ofRandom(0,1) * (neon.width-1));
			float yp = (int)(ofRandom(0,1) * (neon.height-1));
			int pixelPos = (yp*neon.width + xp) * 3;
			*/
			float random = ofRandom(0,1);
			if (random < 0.25){
				HOBJ[HOBJ.size()-1].myColor.set(255,255,0);
			} else if (random >= 0.25 && random < 0.5) {
				HOBJ[HOBJ.size()-1].myColor.set(255,0,255);
			} else if (random >= 0.5f && random < 0.75f){
				HOBJ[HOBJ.size()-1].myColor.set(0,255,255);
			} else {
				HOBJ[HOBJ.size()-1].myColor.set(255,255,255);
			}


			trackerId ++;
			bFoundThisFrame[i] = true;
		}
	}


	// do some other stuff, delete off old ones:
	for (int i = 0;  i < HOBJ.size(); i++){
		if (HOBJ[i].bFoundThisFrame == false){
			HOBJ[i].energy *= 0.9f;
		}
	}

	std::vector<handObject >::iterator iter = HOBJ.begin();
	while (iter != HOBJ.end())
	{
		if ((*iter).energy <= 0.05)
		{

			// seems to work!
			//printf("removing a packet with ID  %i \n", (*iter)->packetID);
			//delete (*iter);	// this should !!! free memory.  please check

			iter = HOBJ.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	/*
	printf("--------------------------------- \n");
	for (int i = 0; i < HOBJ.size(); i ++){

			printf("object %i  id %i \n", i, HOBJ[i].id);
			printf("object %i  energy  %f \n", i, HOBJ[i].energy);
			printf("object %i  bFoundThisFrame  %i \n", i, (int)HOBJ[i].bFoundThisFrame);
			printf("object %i  whoThisFrame  %i \n", i, HOBJ[i].whoThisFrame);

	}
	printf("--------------------------------- \n");
	*/


	ofFill();
	FBO.swapIn();
	FBO.setupScreenForMe();
	ofEnableAlphaBlending();

	ofSetColor(0, 0, 0, 15);
	ofRect(0,0,640,480);




	for (int i = 0; i < BSM.shapes.size(); i++){

		ofSetColor( BSM.shapes[i]->myColor.x,  BSM.shapes[i]->myColor.y,  BSM.shapes[i]->myColor.z, 10 * powf(BSM.shapes[i]->energy, 1.4));

		if (BSM.shapes[i]->trail.size() > 2){
			ofPoint pta = BSM.shapes[i]->trail[BSM.shapes[i]->trail.size()-2];
			ofPoint ptb = BSM.shapes[i]->trail[BSM.shapes[i]->trail.size()-2];

			ofPoint diff;
			diff.x = ptb.x - pta.x;
			diff.y = ptb.y - pta.y;

			for (int j = 0; j < 10; j++){
				pta.x += diff.x/9.0f;
				pta.x  += diff.x/9.0f;
				BSM.spot.draw(pta.x/5-4, pta.y/5-4, 8,8);
			}
		}

	}
	/*
	ofSetColor(255, 255, 255);
	ofRect(ofRandom(0,640), ofRandom(0,480), 33,33);
	*/
	FBO.swapOut();
	FBO.setupScreenForThem();


	//box2d.update();


	if (lastFrameRendered !=  packet.frameNumber){

		handImage.set(0);

		for (int i = 0; i < packet.nBlobs; i++){
			int nPts = packet.nPts[i];
			if (!packet.bAmInner[i]){
				handImage.drawBlobIntoMeWithPreallocatedPts(packet.pts[i], nPts, 100);
			}
		}

		for (int i = 0; i < packet.nBlobs; i++){
			int nPts = packet.nPts[i];
			if (packet.bAmInner[i]){
				handImage.drawBlobIntoMeWithPreallocatedPts(packet.pts[i], nPts, 0);
			}
		}

		handImage.blur();

		handImageTemporallyBlurred -= 50;
		handImageTemporallyBlurred.blur();
		handImageTemporallyBlurred.blur();
		handImageTemporallyBlurred += handImage;

		handImageTemporallyBlurredInvert = handImageTemporallyBlurred;
		handImageTemporallyBlurredInvert.invert();



		// clear out the shape
		/*for(int i=0; i<box2dBuilding.size(); i++) {
			box2dBuilding[i].destroyShape();
		}
		box2dBuilding.clear();
		*/
		float scalex = (float)OFFSCREEN_WIDTH / (float)packet.width;
		float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;

		/*
		for (int i = 0; i < packet.nBlobs; i++){

			box2dBuilding.push_back(ofxBox2dLine());
			box2dBuilding.back().clear();
			box2dBuilding.back().setWorld(box2d.getWorld());

			for (int j = packet.nPts[i]-1; j>0 ; j-=5){
				float bx = packet.pts[i][j].x * scalex;
				float by = packet.pts[i][j].y * scaley;
				box2dBuilding.back().addPoint(bx, by);

			}
			box2dBuilding.back().createShape();

		}
		 */

	}


	BSM.update();

	float scalex =  (float)OFFSCREEN_WIDTH / (float)packet.width;
	float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;

	for (int i = 0; i < packet.nBlobs; i++){

		// try to find my color :

		ofPoint myColor;
		myColor.set(255,0,255);

		for (int j = 0; j < HOBJ.size(); j++){
			if (HOBJ[j].bFoundThisFrame && HOBJ[j].whoThisFrame == i){
				myColor = HOBJ[j].myColor;
			}
		}

		for (int j = packet.nPts[i]-1; j>0 ; j-=10){
			float bx = packet.pts[i][j].x * scalex;
			float by = packet.pts[i][j].y * scaley;
			BSM.checkShapesForPoint(bx, by, myColor);

		}

	}


	lastFrameRendered = packet.frameNumber;


	// for DAITO :



	for (int i = 0; i < BSM.shapes.size(); i++){
		if (BSM.shapes[i]->bPeakedThisFrame){

			//printf("%i peaked at %f %f \n", BSM.shapes[i]->type, BSM.shapes[i]->centroid.x, BSM.shapes[i]->centroid.y);
			BSM.shapes[i]->bPeakedThisFrame = false;

			ofxOscMessage msg;
			msg.setAddress("/bang");								//	bang
			msg.addStringArg("handTrigger");					//	handTrigger
			msg.addIntArg(0);									//	SCENE 0
			msg.addIntArg(BSM.shapes[i]->type);					//	type of object (column, etc) see the list in setup "BSM.parseShapesFromFile"
			msg.addIntArg((int)(BSM.shapes[i]->centroid.x));	// centroid x
			msg.addIntArg((int)(BSM.shapes[i]->centroid.y));	// centroid y
			msg.addIntArg((int)(BSM.shapes[i]->boundingRect.width * BSM.shapes[i]->boundingRect.height / 100.0));

																// area (w*h / 100) of the object (how big)

			//   /bang /handTrigger /0 /type /centroidx /centroidy /size

		ofxDaito::sendCustom(msg);

		}
	}

}

//--------------------------------------------------------------
void HandScene::draw(){
	ofSetColor(255, 0, 0);
	//ofLine (0,0,OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	//ofLine (0,OFFSCREEN_HEIGHT,OFFSCREEN_WIDTH, 0);

	BSM.draw();


	ofEnableAlphaBlending();
	ofSetColor(50,50,50);
	glBlendFunc(GL_ONE, GL_ONE);
	FBO.draw(0,0,OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	ofEnableAlphaBlending();


	//ferryBuilding.drawContour();
	//ferryBuilding.drawInfo();



	ofEnableAlphaBlending();
	glBlendFunc(GL_DST_COLOR, GL_ZERO);
	ofSetColor(255, 255, 255);
	handImageTemporallyBlurredInvert.draw(0,0,OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);





	ofEnableAlphaBlending();

	float scalex =  (float)OFFSCREEN_WIDTH / (float)packet.width;
	float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;

	ofPushStyle();
	glLineWidth(NEON_SILHOUETTE_SIZE);
	for (int i = 0; i < packet.nBlobs; i++){

		/*glBegin(GL_LINE_LOOP);
		 for (int j = 0; j < packet.nPts[i]; j++){
		 glVertex2f(packet.pts[i][j].x * scalex, packet.pts[i][j].y * scaley);
		 }
		 glEnd();
		 */

		ofSetColor(255, 255, 255, 100);
		ofNoFill();
		ofEnableSmoothing();
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < packet.nPts[i]; j++){
			glVertex2f(packet.pts[i][j].x * scalex, packet.pts[i][j].y * scaley);
		}
		glEnd();

		ofSetColor(255, 255, 255, 85);
		/*ofBeginShape();
		for (int j = 1; j < packet.nPts[i]; j+=3){
			ofCurveVertex(packet.pts[i][j].x * scalex, packet.pts[i][j].y * scaley);
		}
		ofEndShape(true);*/
	}
	ofPopStyle();

	// draw some new stuff.
	for(int i=0; i<balls.size(); i++) {
		//balls[i].draw();
	}

	for(int i=0; i<box2dHandShape.size(); i++) {
		//box2dHandShape[i].draw();
	}


	//floor.draw();
	//box2d.draw();
}

//--------------------------------------------------------------
void HandScene::drawTop(){


	/*
	for (int i = 0; i < packet.nBlobs; i++){
		ofFill();
		ofSetColor(255, 0, 0);
		ofBeginShape();
		for (int j = 0; j < packet.nPts[i]; j++){
			ofVertex(packet.pts[i][j].x, packet.pts[i][j].y);
		}
		ofEndShape(true);
	}

	ofSetColor(255, 255, 255);
	//handImageTemporallyBlurred.draw(0,400);
	*/
}


//--------------------------------------------------------------
void HandScene::keyPressed(int key) {
	ferryBuilding.keyPressed(key);


	if(key == ' ') {

		for(int i=0; i<box2dBuilding.size(); i++) {
			box2dBuilding[i].destroyShape();
		}
		box2dBuilding.clear();

		// build the building for box2d
		for(int i=0; i<ferryBuilding.shapes.size(); i++) {
			box2dBuilding.push_back(ofxBox2dLine());
			box2dBuilding.back().clear();
			box2dBuilding.back().setWorld(box2d.getWorld());

			for(int j=0; j<ferryBuilding.shapes[i].pnts.size(); j++) {
				float bx = ferryBuilding.shapes[i].pnts[j].x;
				float by = ferryBuilding.shapes[i].pnts[j].y;
				box2dBuilding.back().addPoint(bx, by);
			}
			box2dBuilding.back().createShape();
		}
	}
}

//--------------------------------------------------------------
void HandScene::mousePressed(int x, int y, int button) {

	ferryBuilding.mousePressed(x, y, button);
	/*
	HandBall bub;
	bub.setPhysics(3.0, 0.53, 0.1); // mass - bounce - friction
	bub.setup(box2d.getWorld(), x, y, 20);
	balls.push_back(bub);
	*/

}












