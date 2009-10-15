

#include "MonsterScene.h"
#include "ofxVectorMath.h"

//-------------------------------------------------------------- setup
void MonsterScene::setup(){
	
	bGotMyFirstPacket = false;
	
	// box2d
	box2d.init();
	box2d.setGravity(0, 30);
	box2d.checkBounds(true);
	box2d.setFPS(70.0);
	printf("monster box2d allocated\n");
	
	// load the ferry contour
	ferryBuilding.setupBuilding("buildingRefrences/buidlingFiles/windowsFerryContour.xml");
	
	
	// no particles yet
	particleCount = 0;
	
	
	// load all the svg parts
	parts.loadAllParts();
	
	FBO.allocate(OFFSCREEN_WIDTH/5,OFFSCREEN_HEIGHT/5, false);	// fixed?
	
}

//-------------------------------------------------------------- update
void MonsterScene::update(){
	
	box2d.update();
	contourAnalysis.setSize(packet.width, packet.height);
	
	// --------------------- particles
	for(int i = 0; i < monsterParticles.size(); i++) {
		monsterParticles[i].update();	
	}
	for(int i=monsterParticles.size()-1; i>=0; i--) {
		// sorry time to go away
		if(monsterParticles[i].bDead) {
			monsterParticles[i].destroyShape();
			monsterParticles.erase(monsterParticles.begin() + i);
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
	
	BubbleMonster monster;
	monster.monsterID = bid;
	monster.parts = &parts;
	monster.init( tracker->getById(bid) );
	
	monsters.push_back(monster);
	
	//monsters.push_back(BubbleMonster());
	//monsters.back().init( tracker->getById(id) );
	
	
	ofxOscMessage msg;		
	msg.setAddress("/bang");							//	bang
	msg.addStringArg("newMonster");						//	new monster
	msg.addIntArg(3);									//	SCENE 3
	msg.addIntArg(bid);									//	monster ID
	msg.addFloatArg((float)x/(float)packet.width);		// centroid x (normalize)
	msg.addFloatArg((float)y/(float)packet.height);		// centroid y (normalize)
	
	DAITO.sendCustom(msg);
	
	
}

void MonsterScene::blobMoved( int x, int y, int bid, int order ) {
	
	for(int i=monsters.size()-1; i>=0; i--) {
		if(monsters[i].monsterID == bid) {
			
			// tell daito that the monster just moved
			ofxOscMessage msg;		
			msg.setAddress("/bang");							//	bang
			msg.addStringArg("monsterMove");					//	new monster
			msg.addIntArg(3);									//	SCENE 3
			msg.addIntArg(bid);									//	monster ID
			msg.addFloatArg((float)x/(float)packet.width);		// centroid x (normalize)
			msg.addFloatArg((float)y/(float)packet.height);		// centroid y (normalize)
			DAITO.sendCustom(msg);
			
			monsters[i].genNewRadius();
			
			if(monsterParticles.size() < MAX_MONSTER_PARTICLES) {
				// add some particle love -- ewww			
				float bx = x + ofRandom(-30, 30);
				float by = y + ofRandom(-30, 30);
				
				monsterParticles.push_back(MonsterParticles());
				monsterParticles.back().init();
				monsterParticles.back().setPhysics(3.0, 0.53, 0.1); // mass - bounce - friction
				monsterParticles.back().setup(box2d.getWorld(), bx, by, ofRandom(4, 50));
				
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
			msg.addStringArg("deadMonster");					//	new monster
			msg.addIntArg(3);									//	SCENE 3
			msg.addIntArg(bid);									//	monster ID
			msg.addFloatArg((float)x/(float)packet.width);		// centroid x (normalize)
			msg.addFloatArg((float)y/(float)packet.height);		// centroid y (normalize)
			
			DAITO.sendCustom(msg);
			
		}
	}
}


//-------------------------------------------------------------- debug drawing
void MonsterScene::drawTop() {
	
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
	
	ofEnableAlphaBlending();
	ofSetColor(50,50,50);
	glBlendFunc(GL_ONE, GL_ONE);
	FBO.draw(0, 0);
	ofEnableAlphaBlending();	
}


//-------------------------------------------------------------- render
void MonsterScene::draw() {
	
	ofEnableAlphaBlending();
	
	// --------------------- People
	glPushMatrix();
	//glTranslatef(mouseX, mouseY, 0);
	glTranslatef(((OFFSCREEN_WIDTH - packet.width)/2), (OFFSCREEN_HEIGHT-packet.height), 0);
	
	bool bDrawPeople = true;
	
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
	for(int i=0; i<box2dBuilding.size(); i++) {
		box2dBuilding[i].draw();	
	}
	
	for(int i=0; i<balls.size(); i++) {
		balls[i].draw();	
	}
	
	//ofDisableAlphaBlending();
	glPopMatrix();
	
	
	
	// ferry building only for setup (hide when live)
	ferryBuilding.drawContour();
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
		box2dBuilding.push_back(ofxBox2dLine());
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
	
	ferryBuilding.keyPressed(key);
	
	if(key == ' ') {
		createBuildingContour();
	}
	
	// add some
	if(key == 't') {
		float bx = mouseX;
		float by = mouseY;
		
		bx -= ((OFFSCREEN_WIDTH - packet.width)/2);
		by -= (OFFSCREEN_HEIGHT - packet.height);
		
		
		MonsterBall bub;
		bub.setPhysics(30.0, 0.53, 0.1); // mass - bounce - friction
		bub.setup(box2d.getWorld(), bx, by, ofRandom(3, 50));
		balls.push_back(bub);
		
	}
}

//--------------------------------------------------------------
void MonsterScene::mousePressed(int x, int y, int button) {
	
	
	ferryBuilding.mousePressed(x, y, button);
	
}



























