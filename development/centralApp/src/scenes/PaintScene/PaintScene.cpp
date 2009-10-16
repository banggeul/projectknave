
#include "PaintScene.h"


void PaintScene::setup() {
	
	// box2d
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.checkBounds(true);
	box2d.setFPS(30.0);
	
	//box2d.getWorld()->SetContactListener(this);
	
	
	// build the building for box2d
	for(int i=0; i<building.shapes.size(); i++) {
		buildingBoundsShape.push_back(ofxBox2dLine());
		buildingBoundsShape.back().clear();
		buildingBoundsShape.back().setWorld(box2d.getWorld());
		
		for(int j=0; j<building.shapes[i].pnts.size(); j++) {
			float bx = building.shapes[i].pnts[j].x;
			float by = building.shapes[i].pnts[j].y;
			ofPoint pos = getPointInPreview(bx, by);
			buildingBoundsShape.back().addPoint(pos.x, pos.y);
		}
		buildingBoundsShape.back().createShape();
	}
	
	// load the ferry contour
	bGotMyFirstPacket = false;
	ferryBuilding.setupBuilding("buildingRefrences/buidlingFiles/paintFerryContour.xml");
	printf("- paint ferry building loaded -\n");
	
}

//--------------------------------------------------------------
void PaintScene::createBuildingContour() {
	for(int i=0; i<box2dBuilding.size(); i++) {
		box2dBuilding[i].destroyShape();
	}	
	box2dBuilding.clear();
	
	
	// build the building for box2d
	for(int i=0; i<ferryBuilding.shapes.size(); i++) {
		box2dBuilding.push_back(ofxBox2dLine());
		box2dBuilding.back().setWorld(box2d.getWorld());
		box2dBuilding.back().clear();
		
		for(int j=0; j<ferryBuilding.shapes[i].pnts.size(); j++) {
			float bx = ferryBuilding.shapes[i].pnts[j].x;
			float by = ferryBuilding.shapes[i].pnts[j].y;
			
			//bx -= ((OFFSCREEN_WIDTH - packet.width)/2);
			//by -= (OFFSCREEN_HEIGHT-packet.height);
			
			
			
			box2dBuilding.back().addPoint(bx, by);
		}
		box2dBuilding.back().createShape();
	}
	
	printf("-- building shape made --\n");
}

// ------------------------------------------------
void PaintScene::update(){
	
	
	box2d.update();
	
	// ground points
	for(int i=0; i<NUM_GROUND_PNTS; i++) {
		float n = (float)i / (float)(NUM_GROUND_PNTS-1);
		groundPnts[i].x = n * OFFSCREEN_WIDTH;
		groundPnts[i].y = OFFSCREEN_HEIGHT -  (60 + (cos((n*ofGetElapsedTimef())) * 80.0) );		
	}
	
	
	// do we have a new packet
	if (lastFrameRendered !=  packet.frameNumber){
		
		
		
		
		
		
		/*
		// a smooth contour
		monsters[j].contourSmooth.assign(monsters[j].contourSimple.size(), ofPoint());
		contourAnalysis.smooth(monsters[j].contourSimple, monsters[j].contourSmooth, 0.2);
		
		
		// a convex contour
		monsters[j].contourConvex.assign(monsters[j].contourSimple.size(), ofPoint());
		contourAnalysis.convexHull(monsters[j].contourSimple, monsters[j].contourConvex);
		*/
		
		
		
		// clear out all the body shapes
		for(int i=0; i<bodyShapes.size(); i++) {
			bodyShapes[i].destroyShape();
		}	
		bodyShapes.clear();
		
		
		if(packet.nBlobs > 0) {
			// now build the body shapes
			for(int i=0; i<packet.nBlobs; i++) {
				
				
				// a simple contour
				simpleContour.assign(packet.nPts[i], ofPoint());
				contourAnalysis.simplify(tracker->blobs[i].pts, simpleContour, 0.50);
				
				bodyShapes.push_back(ofxBox2dLine());
				bodyShapes.back().setWorld(box2d.getWorld());
				bodyShapes.back().clear();
				
				for(int j=simpleContour.size()-1; j>=0; j--) {
					float x = simpleContour[j].x + ((OFFSCREEN_WIDTH - packet.width)/2);
					float y = (OFFSCREEN_HEIGHT-packet.height) + simpleContour[j].y;
					bodyShapes.back().addPoint(x, y);
				}
				bodyShapes.back().createShape();
			}
			
		}
		
		
	}
	
	
	
	
	// paint
	for(int i=0; i<paintBalls.size(); i++) {
		paintBalls[i].update();	
	}
	
	
	if(packet.frameNumber >= 0 && !bGotMyFirstPacket) {
		printf("got my first packet - %i\n", packet.frameNumber);
		bGotMyFirstPacket = true;
		contourAnalysis.setSize(packet.width, packet.height);
		createBuildingContour();
	}
	
	// what was the last frame
	lastFrameRendered = packet.frameNumber;
	
}



// ------------------------------------------------
void PaintScene::draw() {
	
	// The Ground
	/*ofFill();
	 ofSetColor(0, 255, 52);
	 ofBeginShape();
	 ofVertex(0, OFFSCREEN_HEIGHT);
	 ofVertex(0, OFFSCREEN_HEIGHT-130);
	 
	 for(int i=0; i<NUM_GROUND_PNTS; i++) {
	 ofVertex(groundPnts[i].x, groundPnts[i].y);
	 }
	 
	 ofVertex(OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT-130);
	 ofVertex(OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	 ofEndShape();
	 */
	
	ferryBuilding.drawContour();
	
	
	// draw the shape of the building for box2d
	for(int i=0; i<buildingBoundsShape.size(); i++) {
		buildingBoundsShape[i].draw();
	}
	
	
	/*
	// People
	glPushMatrix();
	glTranslatef(0, 0, 0);
	
	for(int i=0; i<packet.nBlobs; i++) {
		ofSetColor(255, i*20, 255-(i*40), 100);
		ofFill();
		ofEnableSmoothing();
		ofBeginShape();
		for (int j = 0; j < packet.nPts[i]; j++) {
			
			float x = packet.pts[i][j].x + ((OFFSCREEN_WIDTH - packet.width)/2);
			float y = (OFFSCREEN_HEIGHT-packet.height) + packet.pts[i][j].y;
			
			ofVertex(x, y);
		}
		ofEndShape(true);
	}
	glPopMatrix();
	*/
	
	
	// --------------------- Contour
	for(int i=0; i<bodyShapes.size(); i++) {
		bodyShapes[i].draw();
	}
	
	
	/*
	ofFill();
	ofSetColor(255, 9, 9);
	ofBeginShape();
	for (int j=0; j<simpleContour.size(); j++) {
		
		float cx = simpleContour[j].x;// + ((OFFSCREEN_WIDTH - packet.width)/2);
		float cy = simpleContour[j].y;//(OFFSCREEN_HEIGHT-packet.height) + simpleContour[j].y;
		ofVertex(cx, cy);
	}
	ofEndShape(true);
	*/
	
	// --------------------- building shape
	for(int i=0; i<box2dBuilding.size(); i++) {
		box2dBuilding[i].draw();	
	}
	
	// --------------------- paint
	for(int i=0; i<paintBalls.size(); i++) {
		paintBalls[i].draw();	
	}
	for(int i=0; i<paint.size(); i++) {
		paint[i].draw();	
	}	
	
	
}



// ------------------------------------------------
void PaintScene::mousePressed(int x, int y, int button) {
	
	printf("--- adding a paint ball ---\n");
	PaintBall ball;
	ball.init(&box2d, x, y);
	paintBalls.push_back(ball);
	ferryBuilding.mousePressed(x, y, button);
	
}

//--------------------------------------------------------------
void PaintScene::keyPressed(int key) {
	
	ferryBuilding.keyPressed(key);
	
	if(key == ' ') {
		createBuildingContour();
	}
}

/*
//--------------------------------------------------------------
void PaintScene::Add(const b2ContactPoint* point){
	b2Vec2 p = point->position;
	p *= OFX_BOX2D_SCALE;
	//	myApp->contact_points.push_back(p);
	//	p = point->velocity;
	//	myApp->contact_velocities.push_back(p);
	
	
	SimplePaint bub;
	bub.pos.x = p.x;
	bub.pos.y = p.y;
	
	//bub.setPhysics(3.0, 0.53, 0.1); // mass - bounce - friction
	//bub.setup(box2d.getWorld(), 9, 9, 20);
	paint.push_back(bub);
	
	printf("-- add some paint herer --\n");	
}
void PaintScene::Remove(const b2ContactPoint* point){
	printf("-- time to remove --\n");
	
}*/




