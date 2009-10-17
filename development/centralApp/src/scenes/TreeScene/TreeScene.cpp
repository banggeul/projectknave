#include "TreeScene.h"

// ------------------------------------------
void TreeScene::setup() {
	curStem = 0;
	
	building.setupBuilding("buildingRefrences/buidlingFiles/treeFerryContour.xml");
	
	
	//for(int i=0; i<3; i++) {
	//		leaves.push_back(ofxSvg());
	//		leaves.back().loadFile("sceneAssets/trees/Fern_Leaf_"+ofToString(i)+".svg");
	//	}
	
	W = 1.0;	
	H = 1.0;
	
	bGotMyFirstPacket	= false;
	minSpawnToGrowW		= 120;
	minSpawnToGrowH		= 120;
	load();
	
	
	for (int i = 0; i < 60; i++){
		ButterFly bf;
		bf.setInitialCondition(ofRandom(200, 500), ofRandom(0, OFFSCREEN_HEIGHT), 0,0);
		bf.setupButterfly();
		butterflys.push_back(bf);
	}
	
	theDot.loadImage("sceneAssets/monsters/dot.png");
	
}

// ------------------------------------------

void TreeScene::mousePressed(int x, int y, int button) {
	stems[curStem].initStem(x, y);
	curStem ++;
	curStem %= MAX_STEMS;
	
	// events for the ferry building
	building.mousePressed(x, y, button);
}


//--------------------------------------------------------------
void TreeScene::keyPressed(int key) {
	
	// keys for the ferry
	building.keyPressed(key);
	
}






//--------------------------------------------------------------
void TreeScene::updateFlocking() {
	
	// on every frame 
	// we reset the forces
	// add in any forces on the particle
	// perfom damping and
	// then update
	
	for (int i = 0; i < butterflys.size(); i++){
		butterflys[i].resetForce();
	}
	
	for (int i = 0; i < butterflys.size(); i++){
		for (int j = 0; j < butterflys.size(); j++){
			if (i != j){
				butterflys[i].addForFlocking(butterflys[j]);	
			}
		}
		
		
		for(int q=0; q<tracker->blobs.size(); q++) {
			
			float barea  = (float)(tracker->blobs[q].boundingRect.height*tracker->blobs[q].boundingRect.width) / (float)(packet.width*packet.height);
			
			//			butterflys[i].addAttractionForce(tracker->blobs[q].centroid.x, tracker->blobs[q].centroid.y, 20+barea*250, 1.5+(barea*2.0));
			//			butterflys[i].addAttractionForce(mouseX, mouseY,200, 2.0);
			
		}
		// this was something we tried in class (partitcle move away from mouse...)
		//particles[i].addRepulsionForce(mouseX, mouseY, 150, 0.4);
	}
	
	for (int i = 0; i < butterflys.size(); i++){
		butterflys[i].addFlockingForce();
		butterflys[i].addDampingForce();
		//butterflys[i].update();
		butterflys[i].pos.x -= 10.0;
	}
	
	
	// wrap torroidally.
	for (int i = 0; i < butterflys.size(); i++){
		ofxVec2f pos = butterflys[i].pos;
		
		//	glTranslatef(((OFFSCREEN_WIDTH - packet.width)/2), (OFFSCREEN_HEIGHT-packet.height), 0);
		
		float offx = ((OFFSCREEN_WIDTH - packet.width)/2);
		float offy = (OFFSCREEN_HEIGHT-packet.height);
		
		if (pos.x < -(OFFSCREEN_WIDTH-offx)) pos.x = OFFSCREEN_WIDTH-offx;
		if (pos.x > OFFSCREEN_WIDTH-offx) pos.x = 0;
		if (pos.y < -OFFSCREEN_HEIGHT) pos.y = OFFSCREEN_HEIGHT;
		if (pos.y > OFFSCREEN_WIDTH) pos.y = 0;
		
		butterflys[i].pos = pos;
	}
	
	
	
}




// ------------------------------------------
void TreeScene::update() {
	
	
	
	// --------------------- Tree People
	for(int i=0; i<trees.size(); i++) {
		trees[i].update();	
	}
	// clean up the trees
	for(int i=trees.size()-1; i>=0; i--) {
		
		if(trees[i].bDead) {
			trees[i].cleanUp();
			trees.erase(trees.begin() + i);
			printf("tree deleted \n");
		}
	}
	
	
	
	
	
	// --------------------- update the trees
	for(int i=0; i<tracker->blobs.size(); i++) {
		
		int lookID = tracker->blobs[i].id;
		float barea  = (float)(tracker->blobs[i].boundingRect.height*tracker->blobs[i].boundingRect.width) / (float)(packet.width*packet.height);
		
		for(int j=0; j<trees.size(); j++) {
			if(lookID == trees[j].id) {
				trees[j].treeBase = tracker->blobs[i].centroid * TREE_SCALE;
				trees[j].frameAge = tracker->blobs[i].frameAge;
				
			}
		}
		
		
	}
	
	updateFlocking();
	
	
	/*
	 float scalex = (float)OFFSCREEN_WIDTH / (float)packet.width;
	 float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;
	 
	 for (int i = 0; i < MAX_STEMS; i++) {
	 
	 if (stems[i].bALive) {
	 
	 
	 for (int j = 0; j < building.shapes.size(); j++) {
	 FerryShape& cur = building.shapes[j];
	 for (int k = 0; k < cur.pnts.size() - 1; k++) {
	 ofxPoint2f start = cur.pnts[k];
	 ofxPoint2f stop = cur.pnts[k + 1];
	 for(float q = 0; q < 1; q += .1) {
	 ofxPoint2f interp = start.interpolate(stop, q);
	 stems[i].head.addRepulsionForce(interp.x, interp.y, 30.0, 30.0);
	 }
	 }
	 }
	 
	 for(int j = 0; j < packet.nBlobs; j++) {
	 for(int k = 0; k < packet.nPts[j]; k++) {
	 ofPoint& cur = packet.pts[j][k];
	 stems[i].head.addAttractionForce(cur.x * scalex, cur.y * scaley, 200, 1);
	 }
	 }
	 
	 stems[i].update();
	 }
	 }
	 */
	
	
	
	
	
	
	// --------------------- packet info
	if(packet.frameNumber >= 0 && !bGotMyFirstPacket) {
		printf("got my first packet - %i\n", packet.frameNumber);
		bGotMyFirstPacket = true;
		
		W = packet.width * TREE_SCALE;
		H = 100 + (packet.height * TREE_SCALE);
	}
	
	
	

}

// ---------------------------------------------------------
void TreeScene::draw() {
	
	ofEnableAlphaBlending();
	glPushMatrix();
	//glTranslatef(mouseX, mouseY, 0);
	glTranslatef(((OFFSCREEN_WIDTH - W)/2), (OFFSCREEN_HEIGHT-H), 0);
	
	for(int i=0; i<packet.nBlobs; i++) {
		ofSetColor(255, i*20, 255);
		ofFill();
		
		ofBeginShape();
		for (int j = 0; j < packet.nPts[i]; j++) {
			
			float x = packet.pts[i][j].x * TREE_SCALE;
			float y = packet.pts[i][j].y * TREE_SCALE;
			
			ofVertex(x, y);
		}
		ofEndShape(true);
	}
	
	
	for(int i = 0; i < butterflys.size(); i++){
		butterflys[i].draw();
	}
	
	// the big trees - funky style
	for (int i = 0; i < trees.size(); i++){
		trees[i].draw();
	}
	
	
	glPopMatrix();
	
	
	ofNoFill();
	ofSetColor(245, 2, 2);
	ofRect(((OFFSCREEN_WIDTH - W)/2), (OFFSCREEN_HEIGHT-H), W, H);
	
}







// --------------------------------------------------------- blob events
void TreeScene::blobOn( int x, int y, int bid, int order ) {
	
	printf("new tree  - %i\n", bid);
	ofxCvBlob blober = tracker->getById(bid);
	
	// if i am hole make a shape instead
	if(blober.hole) {
		
	}
	
	// else make a Tree
	else {
		//if(blober.boundingRect.width >= minSpawnToGrowW || blober.boundingRect.height >= minSpawnToGrowH) {
	//	treePeople.push_back(TreePerson());
	//	treePeople.back().init( tracker->getById(bid) );
	//	treePeople.back().tree.img = &theDot;
		//}
		trees.push_back(MagicTree());
		
		float tx = x * TREE_SCALE;
		float ty = y * TREE_SCALE;
		trees.back().initTree(tx, ty, 10);
		trees.back().img = &theDot;
		trees.back().id = bid;
		trees.back().frameAge = tracker->getById(bid).frameAge;
	}
	
	
	
}

// ---------------------------------------------------------
void TreeScene::blobMoved( int x, int y, int bid, int order ) {
	
	for(int i=treePeople.size()-1; i>=0; i--) {
		if(treePeople[i].id == bid) {
			
			// tell daito that the tree just moved
			ofxCvBlob blober = tracker->getById(bid);
			
		}
	}
	
}

// ---------------------------------------------------------
void TreeScene::blobOff( int x, int y, int bid, int order ) {	
	
	printf("tree off - %i\n", bid);
	for(int i=trees.size()-1; i>=0; i--) {
		if(trees[i].id == bid) {
			
			trees[i].id = -1;
			trees[i].bNoBlobAnymore = true;
		
		}
	}
}













//--------------------------------------------------------------
void TreeScene::load() {
	
	string theFile = "settings/treeSettings.xml";
	
	
	printf("--- loading: %s ---\n", theFile.c_str());
	
	xmlSaver.clear();
	
	if(xmlSaver.loadFile(theFile)) {
		
		minSpawnToGrowW = xmlSaver.getValue("minSpawnToGrowW", 100.0);
		minSpawnToGrowH = xmlSaver.getValue("minSpawnToGrowH", 100.0);
		
	}
	
}

//--------------------------------------------------------------

void TreeScene::save() {
	/*
	 
	 string fileToSave = "settings/treeSettings.xml";
	 
	 xmlSaver.clear();
	 
	 xmlSaver.addTag("windowPnts");
	 xmlSaver.pushTag("windowPnts");
	 for(int i=0; i<windowPnts.size(); i++) {
	 xmlSaver.addTag("wnd");
	 xmlSaver.pushTag("wnd", i);
	 
	 xmlSaver.setValue("pnt:x", windowPnts[i].pos.x, 0);
	 xmlSaver.setValue("pnt:y", windowPnts[i].pos.y, 1);
	 
	 xmlSaver.popTag();
	 }
	 
	 xmlSaver.popTag();
	 xmlSaver.saveFile(fileToSave);
	 
	 printf("--- monster settings saved [%s]---\n", fileToSave.c_str());
	 */
}




