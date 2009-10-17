#include "TreeScene.h"


// ------------------------------------------
void TreeScene::setup() {
	
	butterFlyColor.push_back(0xd1007e);
	butterFlyColor.push_back(0x00a3c4);
	butterFlyColor.push_back(0x6ac539);
	butterFlyColor.push_back(0xff5500);
	butterFlyColor.push_back(0xff0009);
	
	theDot.loadImage("sceneAssets/monsters/dot.png");
	theDotS.loadImage("sceneAssets/trees/littleDot.png");
	
	
	
	// ------------ Control Panel For Trees
	panel.setup("Tree Scene", 700, 10, 300, 750);
	panel.addPanel("Triggers", 1, false);
	
	panel.setWhichPanel("Triggers");
	panel.addSlider("Tree Delay", "TREE_DELAY", 1.3, 0.0, 10.0, false);
	panel.addSlider("Bottom Offset", "BOTTOM_OFFSET", 100, 0.0, 500.0, false);
	panel.addSlider("People Scale", "PEOPLE_SCALE", 2.4, 1.0, 5.0, false);
	panel.addSlider("people glow", "PEOPLE_GLOW", 20.4, 0.0, 255.0, false);
	
	panel.addToggle("do people glow", "BPEOPLE_GLOW", 1);
	
	
	panel.loadSettings("settings/panels_xml/treeScenePanel.xml");
	
	
	// ------------ The Ferr Contour
	building.setupBuilding("buildingRefrences/buidlingFiles/treeFerryContour.xml");
	
	
	
	// ------------ Load some SVG Files for the Ferns 
	RandomFern tempFern;
	tempFern.addLeaf("sceneAssets/trees/Fern_Leaf_001.svg");
	tempFern.addLeaf("sceneAssets/trees/Fern_Leaf_002.svg");
	tempFern.addLeaf("sceneAssets/trees/Fern_Leaf_003.svg");
	
	
	
	// ------------ Settins 
	W = 1.0;	
	H = 1.0;
	
	bGotMyFirstPacket	= false;
	minSpawnToGrowW		= 120;
	minSpawnToGrowH		= 120;
	load();
	
	// ------------ Get some Flocling butterflys in
	ButterflyAnimation tempButterFly;
	tempButterFly.addFile("sceneAssets/trees/Butterfly_001.svg");
	tempButterFly.addFile("sceneAssets/trees/Butterfly_002.svg");
	tempButterFly.addFile("sceneAssets/trees/Butterfly_003.svg");
	tempButterFly.addFile("sceneAssets/trees/Butterfly_004.svg");
	
	if(butterflys.size() < MAX_BUTTERFLYS) {
		for (int i = 0; i < MAX_BUTTERFLYS; i++){
			ButterFlyParticle bf;
			bf.setInitialCondition(ofRandom(200, OFFSCREEN_WIDTH), ofRandom(0, OFFSCREEN_HEIGHT), 0,0);
			bf.setupButterfly();
			
			butterflys.push_back(bf);
			butterflys.back().img = &theDot;
			butterflys.back().color = butterFlyColor[(int)ofRandom(0, butterFlyColor.size()-1)];
		}
	}
	
}


//--------------------------------------------------------------
void TreeScene::cleanUpScene() {
	

	for(int i=treeBlobs.size()-1; i>=0; i--) {
		treeBlobs.erase(treeBlobs.begin() + i);
	}
	treeBlobs.clear();
	
	
	for(int i=trees.size()-1; i>=0; i--) {
		trees.erase(trees.begin() + i);
	}
	trees.clear();
	
	
}


//--------------------------------------------------------------
void TreeScene::keyPressed(int key) {
	// keys for the ferry
	building.keyPressed(key);	
}

//--------------------------------------------------------------
void TreeScene::mouseDragged(int wx, int wy, int x, int y, int button){
	panel.mouseDragged(wx, wy, button);
}

//--------------------------------------------------------------
void TreeScene::mousePressed(int wx, int wy, int x, int y, int button){
	panel.mousePressed(wx, wy, button);
	
	// events for the ferry building
	building.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void TreeScene::mouseReleased(int wx, int wy, int x, int y, int button){
	panel.mouseReleased();
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
			
			ofRectangle newRec = tracker->blobs[q].boundingRect;
			
			float forceFactor = (newRec.width * newRec.height) / (packet.width*packet.height);
			
			newRec.x		*= TREE_SCALE;
			newRec.y		*= TREE_SCALE;
			newRec.width	*= TREE_SCALE;
			newRec.height	*= TREE_SCALE;
			ofPoint center   = tracker->blobs[q].centroid * TREE_SCALE;
			
			
			butterflys[i].addAttractionForce(center.x, center.y - 20, 200, 1.0);
			
			
		}
	}
	
	for (int i = 0; i < butterflys.size(); i++){
		butterflys[i].addFlockingForce();
		butterflys[i].addDampingForce();
		butterflys[i].update();
	}
	
	// wrap torroidally.
	for (int i = 0; i < butterflys.size(); i++){
		ofxVec2f pos = butterflys[i].pos;
		
		float offx = ((OFFSCREEN_WIDTH - W)/2);
		float offy = (OFFSCREEN_HEIGHT - H);
		float gap  = 20;
		if (pos.x < -offx)						pos.x = OFFSCREEN_WIDTH-offx;
		if (pos.x > OFFSCREEN_WIDTH-offx)		pos.x = -offx;
		if (pos.y < -(offy+gap))				pos.y = offy;
		if (pos.y > offy)						pos.y = - (offy + gap);
		
		butterflys[i].pos = pos;
	}
	
	
	
}




// ------------------------------------------
void TreeScene::update() {
	
	panel.update();
	TREE_SCALE = panel.getValueF("PEOPLE_SCALE");
	
	
	
	// --------------------- add some fern
	if((int)ofRandom(0, 30) == 10) {
		if(ferns.size() <= 10) {	// just 10 for now
			//	ferns.push_back(RandomFern());
			printf("-- new fern --\n");
			
		}
	}
	
	// --------------------- Tree Blobs
	for(int i=0; i<treeBlobs.size(); i++) {
		treeBlobs[i].age = (float)(ofGetElapsedTimef()-treeBlobs[i].initTime);	
		if(treeBlobs[i].age > panel.getValueF("TREE_DELAY") && !treeBlobs[i].bAlive) {
			treeBlobs[i].bAlive = true;	
			
			// time to grow a tree
			printf("--- time to grow a tree:%i---\n", treeBlobs[i].id);
			trees.push_back(MagicTree());
			
			trees.back().initTree(0, 0, (int)ofRandom(20, 50));	// <--- i need a init pos
			trees.back().img = &theDot;
			trees.back().id = treeBlobs[i].id;
			
			for(int j=0; j<tracker->blobs.size(); j++) {
				
				ofRectangle newRec = tracker->blobs[j].boundingRect;
				newRec.x		*= TREE_SCALE;
				newRec.y		*= TREE_SCALE;
				newRec.width	*= TREE_SCALE;
				newRec.height	*= TREE_SCALE;
				
				trees.back().rect	  = newRec;
				trees.back().center   = tracker->blobs[j].centroid * TREE_SCALE;
				
				// init the tree pos
				trees.back().treeBaseD   = trees.back().center;
				trees.back().treeBaseD.y = trees.back().rect.y + trees.back().rect.height + H;
				trees.back().treeBase = trees.back().treeBaseD;
			}
		}
		
		
	}
	
	
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
		
		ofRectangle newRec = tracker->blobs[i].boundingRect;
		
		newRec.x		*= TREE_SCALE;
		newRec.y		*= TREE_SCALE;
		newRec.width	*= TREE_SCALE;
		newRec.height	*= TREE_SCALE;
		newRec.height   += 30;
		
		for(int j=0; j<trees.size(); j++) {
			if(lookID == trees[j].id) {
				trees[j].frameAge = 0;// tracker->blobs[i].frameAge;
				trees[j].rect	  = newRec;
				trees[j].center   = tracker->blobs[i].centroid * TREE_SCALE;
				
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
		
	}
	
	W = packet.width * TREE_SCALE;
	H = panel.getValueF("BOTTOM_OFFSET") + (packet.height * TREE_SCALE);
	
}

// ---------------------------------------------------------
void TreeScene::drawTop() {
	panel.draw();
}

// ---------------------------------------------------------
void TreeScene::draw() {
	
	ofSetColor(0, 25, 255);
	ofFill();
	ofRect(-300, -500, 1000000, 100000);
	
	
	ofEnableAlphaBlending();
	glPushMatrix();
	//glTranslatef(mouseX, mouseY, 0);
	glTranslatef(((OFFSCREEN_WIDTH - W)/2), (OFFSCREEN_HEIGHT-H), 0);
	
	
	// draw the ferns
	for(int i=0; i<ferns.size(); i++) {
		ofPushStyle();
		glPushMatrix();
		glTranslatef(ferns[i].pos.x, ferns[i].pos.y, 0);
		glRotatef(0, 0, 0, 1);
		//ferns[i].draw(1.0);
		glPopMatrix();
		ofPopStyle();
	}
	
	// The Tree Blobs
	for(int i=0; i<treeBlobs.size(); i++) {
		ofFill();
		ofSetColor(200, 40, 255);
		ofCircle(i*85, 0, 40);
		ofSetColor(255, 255, 255);
		ofDrawBitmapString("id: "+ofToString(treeBlobs[i].id) +"\n"+ ofToString(treeBlobs[i].age), i*78, 10);
	}
	
	
	
	// the big trees - funky style
	for (int i = 0; i < trees.size(); i++){
		trees[i].draw();
	}
	
	// draw the people
	for(int i=0; i<packet.nBlobs; i++) {
		
		
		if(panel.getValueB("BPEOPLE_GLOW")) {
			// drop shadow
			ofEnableAlphaBlending();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			for (int j = 0; j < packet.nPts[i]; j+=2) {
				float x = packet.pts[i][j].x * TREE_SCALE;
				float y = packet.pts[i][j].y * TREE_SCALE;
				
				
				ofSetRectMode(OF_RECTMODE_CENTER);
				ofSetColor(255, 255, 255, panel.getValueF("PEOPLE_GLOW"));
				theDotS.draw(x, y, 55, 55);
				ofSetRectMode(OF_RECTMODE_CORNER);
			}
			glDisable(GL_BLEND);
			ofDisableAlphaBlending();
		}
		
		// people
		ofSetColor(50, 50, 50);
		ofFill();
		ofBeginShape();
		for (int j = 0; j < packet.nPts[i]; j++) {
			
			float x = packet.pts[i][j].x * TREE_SCALE;
			float y = packet.pts[i][j].y * TREE_SCALE;
			
			ofVertex(x, y);
		}
		ofEndShape(true);
		
		
		
		
	}
	
	
	// draw the butter flys
	for(int i = 0; i < butterflys.size(); i++){
		butterflys[i].draw();
	}
	
	
	
	
	glPopMatrix();
	
	
	ofNoFill();
	ofSetColor(245, 2, 2);
	ofRect(((OFFSCREEN_WIDTH - W)/2), (OFFSCREEN_HEIGHT-H), W, H);
	
}







// --------------------------------------------------------- blob events
void TreeScene::blobOn( int x, int y, int bid, int order ) {
	
	ofCvTrackedBlob * blober = &tracker->getById(bid);
	
	// if i am hole make a shape instead
	if(blober->hole) {
		
	}
	
	// else make a Tree
	else {
		
		
		treeBlobs.push_back(TreeBlob());
		treeBlobs.back().id = bid;
		treeBlobs.back().age = 0;
		treeBlobs.back().initTime = ofGetElapsedTimef();
		
		
		//if(blober.boundingRect.width >= minSpawnToGrowW || blober.boundingRect.height >= minSpawnToGrowH) {
		//	treePeople.push_back(TreePerson());
		//	treePeople.back().init( tracker->getById(bid) );
		//	treePeople.back().tree.img = &theDot;
		//}
		/*
		 printf("-- id:%i, frameAge:%i\n", bid, tracker->getById(bid).frameAge);
		 if(blober->frameAge > 50) {
		 trees.push_back(MagicTree());
		 
		 float tx = x * TREE_SCALE;
		 float ty = y * TREE_SCALE;
		 trees.back().initTree(tx, ty, 10);
		 trees.back().img = &theDot;
		 trees.back().id = bid;
		 trees.back().frameAge = blober->frameAge;
		 
		 printf("new tree  - %i\n", bid);
		 
		 }
		 */
	}
	
	
	
}

// ---------------------------------------------------------
void TreeScene::blobMoved( int x, int y, int bid, int order ) {
	
}

// ---------------------------------------------------------
void TreeScene::blobOff( int x, int y, int bid, int order ) {	
	
	for(int i=trees.size()-1; i>=0; i--) {
		if(trees[i].id == bid) {
			
			trees[i].id = -1;
			trees[i].bNoBlobAnymore = true;
			printf("tree off - %i\n", bid);
			
		}
	}
	
	// clean up the tree blobs
	for(int i=treeBlobs.size()-1; i>=0; i--) {
		if(treeBlobs[i].id == bid) {
			treeBlobs.erase(treeBlobs.begin() + i);
			printf("--- tree blob removed [%i] -- \n", bid);
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




