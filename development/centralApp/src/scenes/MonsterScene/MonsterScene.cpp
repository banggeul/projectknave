

#include "MonsterScene.h"
#include "ofxVectorMath.h"

//-------------------------------------------------------------- setup
void MonsterScene::setup(){
	
	bDebug			  = false;
	bGotMyFirstPacket = false;
	
	// box2d
	box2d.init();
	box2d.setGravity(0, 20);
	box2d.checkBounds(true);
	box2d.setFPS(15.0);
	printf("monster box2d allocated\n");
	
	// load the ferry contour
	ferryBuilding.setupBuilding("buildingRefrences/buidlingFiles/windowsFerryContour.xml");
	loadMonsterSettings();
	
	
	box2d.getWorld()->SetContactListener(this);
	
	
	
	// no particles yet
	particleCount = 0;
	lastTypeofMonster = -1;
	timeSinceLastBurst = 0;
	
	// load all the svg parts
	parts.loadAllParts();
	
	//FBO.allocate(OFFSCREEN_WIDTH/5,OFFSCREEN_HEIGHT/5, false);	// fixed?
	dotImage.loadImage("sceneAssets/monsters/dot.png");
}

//-------------------------------------------------------------- update
void MonsterScene::update(){
	
	
	// add some balls
	if(ballCounter >= 20) {
		
		for(int i=0; i<(int)ofRandom(1, 10); i++) {
			float bx = 898 + ofRandom(-20, 20);
			float by = 227 + ofRandom(-20, 20);
			
			bx -= ((OFFSCREEN_WIDTH - packet.width)/2);
			by -= (OFFSCREEN_HEIGHT - packet.height);
			MonsterBall bub;
			bub.setPhysics(30.0, 0.53, 0.1); // mass - bounce - friction
			bub.setup(box2d.getWorld(), bx, by, ofRandom(10, 20));
			bub.img = &dotImage;
			balls.push_back(bub);
		}
		
		ballCounter = 0;
		
		// ok add a bunch
		if((int)ofRandom(0, 50) == 10 && timeSinceLastBurst > 30) {
			
			for(int i=0; i<(int)ofRandom(20, 100); i++) {
				float bx = 898 + ofRandom(-520, 520);
				float by = 227 + ofRandom(-100, -120);
				
				bx -= ((OFFSCREEN_WIDTH - packet.width)/2);
				by -= (OFFSCREEN_HEIGHT - packet.height);
				MonsterBall bub;
				bub.setPhysics(30.0, 0.53, 0.1); // mass - bounce - friction
				bub.setup(box2d.getWorld(), bx, by, ofRandom(10, 20));
				bub.img = &dotImage;
				balls.push_back(bub);
			}
			
			timeSinceLastBurst = 0;
		}
		
	}
	
	timeSinceLastBurst ++;
	ballCounter ++;
	
	
	
	// ---------------------------- monster dust
	
	
	for(int i=0; i<dust.size(); i++) {
		dust[i].update();
		
		if(packet.nBlobs > 1 && !dust[i].bColor) {
			
			int hex = particleColor[(int)ofRandom(0, NUM_PARTICLE_COLOR)];
			dust[i].colorD.x = (hex >> 16) & 0xff;
			dust[i].colorD.y = (hex >> 8)  & 0xff;
			dust[i].colorD.z = (hex >> 0)  & 0xff;
			
			dust[i].bColor = true;
			
		}
		
		else {
			dust[i].colorD= 255;
			dust[i].bColor = false;	
		}
		
		
	}
	for(int i=dust.size()-1; i>=0; i--) {
		if(dust[i].dead) {
			dust.erase(dust.begin() + i);
		}
	}
	
	
	
	// CONTOUR LOVE
	box2d.update();
	contourAnalysis.setSize(packet.width, packet.height);
	
	// --------------------- particles
	for(int i = 0; i < monsterParticles.size(); i++) {
		monsterParticles[i].update();	
	}
	for(int i=monsterParticles.size()-1; i>=0; i--) {
		// sorry time to go away
		if(monsterParticles[i].bDead) {
			monsterParticles[i].cleanUp();
			monsterParticles[i].destroyShape();
			monsterParticles.erase(monsterParticles.begin() + i);
		}	
		
		ofPoint mpos = monsterParticles[i].getPosition();
		if(mpos.y > OFFSCREEN_HEIGHT) {
			monsterParticles[i].bDead = true;
		}
	}
	
	
	for(int i=balls.size()-1; i>=0; i--) {
		
		ofPoint ps = balls[i].getPosition();
		if(ps.y > OFFSCREEN_HEIGHT) {
			
			balls[i].destroyShape();
			
			balls.erase(balls.begin() + i);
		}
	}
	
	
	// --------------------- update the monster contour pnts
	for(int i=0; i<tracker->blobs.size(); i++) {
		
		int lookID = tracker->blobs[i].id;
		
		for(int j=monsters.size()-1; j>=0; j--) {
			
			// yes we match
			if(monsters[j].monsterID == lookID) {
				
				// the contour (fixed)
				monsters[j].pos = tracker->blobs[i].centroid;
				monsters[j].rect = tracker->blobs[i].boundingRect;
				monsters[j].updateContourPnts(tracker->blobs[i].pts);
				
				
				// a simple contour
				monsters[j].contourSimple.assign(tracker->blobs[i].pts.size(), ofPoint());
				contourAnalysis.simplify(tracker->blobs[i].pts, monsters[j].contourSimple, 0.50);
				
				
				// a smooth contour
				monsters[j].contourSmooth.assign(monsters[j].contourSimple.size(), ofPoint());
				contourAnalysis.smooth(monsters[j].contourSimple, monsters[j].contourSmooth, 0.2);
				
				
				// a convex contour
				monsters[j].contourConvex.assign(monsters[j].contourSimple.size(), ofPoint());
				contourAnalysis.convexHull(monsters[j].contourSimple, monsters[j].contourConvex);
				
				
			}
		}
	}
	
	
	
	
	// --------------------- Monsters
	for(int i = 0; i < monsters.size(); i++) {
		monsters[i].update();
	}
	
	
	/// packet info
	if(packet.frameNumber >= 0 && !bGotMyFirstPacket) {
		printf("got my first packet - %i\n", packet.frameNumber);
		bGotMyFirstPacket = true;
		createBuildingContour();
	}
	
	
	// --------------------- render for all
	//render();
	
	
}

//-------------------------------------------------------------- get monster by id
BubbleMonster& MonsterScene::getMonsterById( int monsterId ) {
	
	for( int i=0; i<monsters.size(); i++ ) {
		if( monsters[i].monsterID == monsterId ) {
			return monsters[i];
		}
	}
	
}

//-------------------------------------------------------------- blob events
void MonsterScene::blobOn( int x, int y, int bid, int order ) {
	printf("monster on - %i\n", bid);
	
	
	ofxCvBlob blober = tracker->getById(bid);
	
	// if i am hole make a shape instead
	if(blober.hole) {
		
	}
	
	// else make a monster
	else {
		
		BubbleMonster monster;
		monster.monsterID = bid;
		
		// set a new monster type each time
		monster.monsterMode = (int)ofRandom(0, 2); 
		if(monster.monsterMode == lastTypeofMonster) {
			int newMonsMode = monster.monsterMode ++;
			newMonsMode %= 2;
			monster.monsterMode = newMonsMode;
		}
		lastTypeofMonster = monster.monsterMode;
		
		// set the parts
		monster.parts = &parts;
		monster.init( tracker->getById(bid) );
		monster.area = (float)(blober.boundingRect.height*blober.boundingRect.width) / (float)(packet.width*packet.height);
		
		
		monsters.push_back(monster);
		
		//monsters.push_back(BubbleMonster());
		//monsters.back().init( tracker->getById(id) );
		
	}
	
	
	ofxOscMessage msg;		
	msg.setAddress("/bang");							//	bang
	msg.addStringArg("newMonster");						//	new monster
	msg.addIntArg(3);									//	SCENE 3
	msg.addIntArg(monsters.back().monsterMode);					//  what kind of monster are we
	msg.addIntArg(bid);									//	monster ID
	msg.addFloatArg((float)x/(float)packet.width);		// centroid x (normalize)
	msg.addFloatArg((float)y/(float)packet.height);		// centroid y (normalize)
	msg.addFloatArg((float)blober.boundingRect.width/(float)packet.width);		// width (normalize)
	msg.addFloatArg((float)blober.boundingRect.height/(float)packet.height);		// hieght (normalize)
	
	

	
	
	ofxDaito::sendCustom(msg);
	
	
	
}

void MonsterScene::blobMoved( int x, int y, int bid, int order ) {
	
	for(int i=monsters.size()-1; i>=0; i--) {
		if(monsters[i].monsterID == bid) {
			
			// tell daito that the monster just moved
			ofxCvBlob blober = tracker->getById(bid);
			
			ofxOscMessage msg;		
			msg.setAddress("/bang");							//	bang
			msg.addStringArg("monsterMove");						//	new monster
			msg.addIntArg(3);									//	SCENE 3
			msg.addIntArg(monsters[i].monsterMode);					//  what kind of monster are we
			msg.addIntArg(bid);									//	monster ID
			msg.addFloatArg((float)x/(float)packet.width);		// centroid x (normalize)
			msg.addFloatArg((float)y/(float)packet.height);		// centroid y (normalize)
			msg.addFloatArg((float)blober.boundingRect.width/(float)packet.width);		// width (normalize)
			msg.addFloatArg((float)blober.boundingRect.height/(float)packet.height);		// hieght (normalize)	
			ofxDaito::sendCustom(msg);
			
			monsters[i].genNewRadius();
			monsters[i].area = (float)(blober.boundingRect.height*blober.boundingRect.width) / (float)(packet.width*packet.height);
			
			//---------- add some particle love -- ewww			
			if(monsterParticles.size() < MAX_MONSTER_PARTICLES) {
				float bx = x + ofRandom(-30, 30);
				float by = y + ofRandom(-30, 30);
				
				monsterParticles.push_back(MonsterParticles());
				monsterParticles.back().setPhysics(3.0, 0.53, 0.1); // mass - bounce - friction
				monsterParticles.back().setup(box2d.getWorld(), bx, by, ofRandom(4, 50));
				monsterParticles.back().img = &dotImage;
				monsterParticles.back().init();
				
			}
		}
	}
	
}

void MonsterScene::blobOff( int x, int y, int bid, int order ) {	
	
	printf("monster off - %i\n", bid);
	for(int i=monsters.size()-1; i>=0; i--) {
		if(monsters[i].monsterID == bid) {
			monsters.erase(monsters.begin() + i);
			
			
			// tell daito that the monster is dead
			
			ofxOscMessage msg;		
			msg.setAddress("/bang");							//	bang
			msg.addStringArg("deadMonster");						//	new monster
			msg.addIntArg(3);									//	SCENE 3
			msg.addIntArg(monsters[i].monsterMode);					//  what kind of monster are we
			msg.addIntArg(bid);									//	monster ID
			msg.addFloatArg((float)x/(float)packet.width);		// centroid x (normalize)
			msg.addFloatArg((float)y/(float)packet.height);		// centroid y (normalize)
			msg.addFloatArg(0);		// width (normalize)
			msg.addFloatArg(0);		// hieght (normalize)		
			
			ofxDaito::sendCustom(msg);
			
			
		}
	}
}


//-------------------------------------------------------------- debug drawing
void MonsterScene::drawTop() {
	if(!bDebug) return;
	glPushMatrix();
	glTranslatef(0, 100, 0);
	
	
	// for debuging draw all the parts
	// parts.drawAllParts();
	
	ofSetColor(255, 0, 0);
	string info = "Monsters Rule!\n";
	info += "Num Eyes: "+ofToString((int)parts.eyes.size())+"\n";
	ofDrawBitmapString(info, 20, 20);
	glPopMatrix();
}



//-------------------------------------------------------------- draw
void MonsterScene::render() {
	//
	//	ofEnableAlphaBlending();
	//	ofSetColor(50,50,50);
	//	glBlendFunc(GL_ONE, GL_ONE);
	//	FBO.draw(0, 0);
	//	ofEnableAlphaBlending();	
}


//-------------------------------------------------------------- render
void MonsterScene::draw() {
	
	ofEnableAlphaBlending();
	
	// --------------------- People
	glPushMatrix();
	//glTranslatef(mouseX, mouseY, 0);
	glTranslatef(((OFFSCREEN_WIDTH - packet.width)/2), (OFFSCREEN_HEIGHT-packet.height), 0);
	
	bool bDrawPeople = false;
	
	if(bDrawPeople) {
		for(int i=0; i<packet.nBlobs; i++) {
			ofSetColor(255, i*20, 255-(i*40), 100);
			ofFill();
			
			ofBeginShape();
			for (int j = 0; j < packet.nPts[i]; j++) {
				
				float x = packet.pts[i][j].x;
				float y = packet.pts[i][j].y;
				
				ofVertex(x, y);
			}
			ofEndShape(true);
		}
	}
	
	
	for(int i=0; i<dust.size(); i++) {
		dust[i].draw();
	}
	
	for(int i=0; i<balls.size(); i++) {
		balls[i].draw();	
	}
	
	
	// --------------------- particles
	for(int i=0; i<monsterParticles.size(); i++) {
		monsterParticles[i].draw();	
	}
	
	
	// --------------------- Monsters
	for(int i = 0; i < monsters.size(); i++) {
		monsters[i].draw();
	}
	
	
	// --------------------- tracker
	//tracker->draw(0, 0);
	
	
	// --------------------- building shape
	if(bDebug) {
		for(int i=0; i<box2dBuilding.size(); i++) {
			box2dBuilding[i].draw();	
		}
	}
	
	
	
	
	
	
	
	
	
	
	// --------------------- building shape
	if(bDebug) {
		for(int i=0; i<windowPnts.size(); i++) {
			windowPnts[i].draw();
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//ofDisableAlphaBlending();
	glPopMatrix();
	
	
	
	// ferry building only for setup (hide when live)
	if(bDebug) ferryBuilding.drawContour();
	//ferryBuilding.drawInfo();
	
	
	
	//ofDisableAlphaBlending();
	
}


//--------------------------------------------------------------
void MonsterScene::createBuildingContour() {
	for(int i=0; i<box2dBuilding.size(); i++) {
		box2dBuilding[i].destroyShape();
	}	
	box2dBuilding.clear();
	
	
	// build the building for box2d
	for(int i=0; i<ferryBuilding.shapes.size(); i++) {
		box2dBuilding.push_back(MonsterLine());
		box2dBuilding.back().setWorld(box2d.getWorld());
		box2dBuilding.back().clear();
		
		for(int j=0; j<ferryBuilding.shapes[i].pnts.size(); j++) {
			float bx = ferryBuilding.shapes[i].pnts[j].x;
			float by = ferryBuilding.shapes[i].pnts[j].y;
			
			bx -= ((OFFSCREEN_WIDTH - packet.width)/2);
			by -= (OFFSCREEN_HEIGHT-packet.height);
			
			
			box2dBuilding.back().addPoint(bx, by);
		}
		box2dBuilding.back().createShape();
		
	}
	
	printf("-- building shape made --\n");
}



//--------------------------------------------------------------
void MonsterScene::keyPressed(int key) {
	
	if(bDebug) {
		ferryBuilding.keyPressed(key);
		
		if(key == ' ') {
			createBuildingContour();
		}
		
		if(key == 'M') {
			saveMonsterSettings();
		}	
	}
	
	// add some
	if(key == 't') {
		float bx = mouseX;
		float by = mouseY;
		
		bx -= ((OFFSCREEN_WIDTH - packet.width)/2);
		by -= (OFFSCREEN_HEIGHT - packet.height);
		
		
		MonsterBall bub;
		bub.setPhysics(30.0, 0.53, 0.1); // mass - bounce - friction
		bub.setup(box2d.getWorld(), bx, by, ofRandom(10, 20));
		bub.img = &dotImage;
		balls.push_back(bub);
		
	}
	
	
}

//--------------------------------------------------------------
void MonsterScene::mousePressed(int x, int y, int button) {
	
	if(bDebug) {
		ferryBuilding.mousePressed(x, y, button);
		
		float bx = mouseX;
		float by = mouseY;
		
		bx -= ((OFFSCREEN_WIDTH - packet.width)/2);
		by -= (OFFSCREEN_HEIGHT - packet.height);
		
		//printf("windddddddddd = %i, %i", x, y);
		windowPnts.push_back(MonsterWindow());
		windowPnts.back().pos.set(x, y);	
		windowPnts.back().img = &dotImage;
	}
}

//--------------------------------------------------------------
void MonsterScene::loadMonsterSettings() {
	
	string theFile = "settings/monsterSettings.xml";
	
	
	printf("--- loading: %s ---\n", theFile.c_str());
	
	xmlSaver.clear();
	
	if(xmlSaver.loadFile(theFile)) {
		
		int numTags = xmlSaver.getNumTags("windowPnts:wnd");
		if(numTags > 0) {
			
			xmlSaver.pushTag("windowPnts");
			
			int numShapes = xmlSaver.getNumTags("wnd:pnt");
			for(int i=0; i<numShapes; i++) {
				
				xmlSaver.pushTag("wnd", i);
				
				float x = xmlSaver.getValue("pnt:x", 0.0, 0);
				float y = xmlSaver.getValue("pnt:y", 0.0, 1);
				windowPnts.push_back(MonsterWindow());
				windowPnts.back().pos.set(x, y);
				windowPnts.back().img = &dotImage;
				xmlSaver.popTag();
				
			}			
			xmlSaver.popTag();
		}
		
	}
	
}

//--------------------------------------------------------------

void MonsterScene::saveMonsterSettings() {
	
	string fileToSave = "settings/monsterSettings.xml";
	
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
}







//--------------------------------------------------------------
void MonsterScene::Add(const b2ContactPoint* point){
	
	b2Vec2 p = point->position;
	p *= OFX_BOX2D_SCALE;
	
	
	b2Body * body_1 = point->shape1->GetBody();
	MonsterData * theData = (MonsterData*)body_1->GetUserData();
	if(theData) {
		//printf("-- this is a window %i\n", theData->bIsWindow);
		
		if(dust.size() < MAX_MONSTER_DUST) {
			dust.push_back(MonsterDust(p.x, p.y));
			dust.back().img = &dotImage;
			
			
			// particle dust hit
			ofxOscMessage msg;		
			msg.setAddress("/bang");							    //	bang
			msg.addStringArg("particleHit");					    //	hit
			msg.addIntArg(3);									    //	SCENE 3
			msg.addFloatArg((float)p.x/(float)OFFSCREEN_WIDTH);		//  x (normalize)
			msg.addFloatArg((float)p.y/(float)OFFSCREEN_HEIGHT);	// centroid y (normalize)
			
			ofxDaito::sendCustom(msg);
		}
		
	}
}

void MonsterScene::Remove(const b2ContactPoint* point){
	//printf("-- time to remove --\n");
	
}






void MonsterScene::cleanUpScene() {
	
	
	for(int i=monsters.size()-1; i>=0; i--) {
		monsters.erase(monsters.begin() + i);
	}
	monsters.clear();
	
	
	for(int i=monsterParticles.size()-1; i>=0; i--) {
		monsterParticles[i].destroyShape();	
		monsterParticles.erase(monsterParticles.begin() + i);
	}	
	monsterParticles.clear();
	
	
	for(int i=balls.size()-1; i>=0; i--) {
		balls[i].destroyShape();	
		balls.erase(balls.begin() + i);
	}	
	balls.clear();
	
}
















