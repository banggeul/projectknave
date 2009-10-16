#include "TreeScene.h"

// ------------------------------------------
void TreeScene::setup() {
	curStem = 0;
	
	building.setupBuilding("buildingRefrences/buidlingFiles/treeFerryContour.xml");
	
	
	//for(int i=0; i<3; i++) {
	//		leaves.push_back(ofxSvg());
	//		leaves.back().loadFile("sceneAssets/trees/Fern_Leaf_"+ofToString(i)+".svg");
	//	}
	minSpawnToGrowW = 120;
	minSpawnToGrowH = 120;
	load();
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

// ------------------------------------------
void TreeScene::update() {
	
	
	// --------------------- Tree People
	
	for(int i=0; i<treePeople.size(); i++) {
		treePeople[i].update();	
	}
	
	
	
	for(int i=treePeople.size()-1; i>=0; i--) {
		
		if(treePeople[i].bDead) {
			treePeople[i].cleanUp();
			treePeople.erase(treePeople.begin() + i);
		}
	}
	
	
	
	// --------------------- update the trees
	for(int i=0; i<tracker->blobs.size(); i++) {
		
		int lookID = tracker->blobs[i].id;
		
		for(int j=treePeople.size()-1; j>=0; j--) {
			
			// yes we match
			if(treePeople[j].id == lookID) {
				
				// the contour (fixed)
				treePeople[j].pos = tracker->blobs[i].centroid;
				treePeople[j].rect = tracker->blobs[i].boundingRect;
			//	treePeople[j].area = tracker->blobs[i].boundingRect.height;
				
				treePeople[j].area  = (float)(tracker->blobs[i].boundingRect.height*tracker->blobs[i].boundingRect.width) / (float)(packet.width*packet.height);
				
				
				
				//monsters[j].updateContourPnts(tracker->blobs[i].pts);
				
				//
				//				// a simple contour
				//				monsters[j].contourSimple.assign(tracker->blobs[i].pts.size(), ofPoint());
				//				contourAnalysis.simplify(tracker->blobs[i].pts, monsters[j].contourSimple, 0.50);
				//				
				//				
				//				// a smooth contour
				//				monsters[j].contourSmooth.assign(monsters[j].contourSimple.size(), ofPoint());
				//				contourAnalysis.smooth(monsters[j].contourSimple, monsters[j].contourSmooth, 0.2);
				//				
				//				
				//				// a convex contour
				//				monsters[j].contourConvex.assign(monsters[j].contourSimple.size(), ofPoint());
				//				contourAnalysis.convexHull(monsters[j].contourSimple, monsters[j].contourConvex);
				//				
				
			}
		}
	}
	
	
	
	
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
}

// ---------------------------------------------------------
void TreeScene::draw() {
	
	ofEnableAlphaBlending();
	glPushMatrix();
	//glTranslatef(mouseX, mouseY, 0);
	glTranslatef(((OFFSCREEN_WIDTH - packet.width)/2), (OFFSCREEN_HEIGHT-packet.height), 0);
	
	for(int i=0; i<packet.nBlobs; i++) {
		ofSetColor(255, i*20, 255);
		ofFill();
		
		ofBeginShape();
		for (int j = 0; j < packet.nPts[i]; j++) {
			
			float x = packet.pts[i][j].x;
			float y = packet.pts[i][j].y;
			
			ofVertex(x, y);
		}
		ofEndShape(true);
	}
	
	
	
	// the tree people
	for(int i=0; i<treePeople.size(); i++) {
		treePeople[i].draw();	
	}
	
	
	glPopMatrix();
	
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
		if(blober.boundingRect.width > minSpawnToGrowW || blober.boundingRect.height > minSpawnToGrowH) {
			treePeople.push_back(TreePerson());
			treePeople.back().init( tracker->getById(bid) );
		}
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
	for(int i=treePeople.size()-1; i>=0; i--) {
		if(treePeople[i].id == bid) {
			
			treePeople[i].bFadeOut = true;
			//treePeople.erase(treePeople.begin() + i);
			
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




