

#include "MonsterScene.h"
#include "ofxVectorMath.h"

//-------------------------------------------------------------- setup
void MonsterScene::setup(){
	
	bGotMyFirstPacket = false;
	
	// box2d
	box2d.init();
	box2d.setGravity(0, -10);
	box2d.checkBounds(true);
	box2d.setFPS(70.0);
	printf("monster box2d allocated\n");
	
	// load the ferry contour
	ferryBuilding.setupBuilding("buildingRefrences/buidlingFiles/monsterFerryContour.xml");
	
	
	// no particles yet
	particleCount = 0;
	
	
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
	
	// --------------------- Monsters
	for(int i = 0; i < monsters.size(); i++) {
		monsters[i].update();
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
	
	
	if(packet.frameNumber >= 0 && !bGotMyFirstPacket) {
		printf("got my first packet - %i\n", packet.frameNumber);
		bGotMyFirstPacket = true;
		createBuildingContour();
	}
	
	
	
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
	monster.init( tracker->getById(bid) );
	monsters.push_back(monster);
	
	//monsters.push_back(BubbleMonster());
	//monsters.back().init( tracker->getById(id) );
	
}

void MonsterScene::blobMoved( int x, int y, int bid, int order ) {
	
	for(int i=monsters.size()-1; i>=0; i--) {
		if(monsters[i].monsterID == bid) {
			
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
		}
	}
}



//-------------------------------------------------------------- draw
void MonsterScene::draw(){
	
	
	
	float scalex = (float)OFFSCREEN_WIDTH / (float)packet.width;
	float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;
	
	
	
	ofEnableAlphaBlending();
	
	// --------------------- People
	glPushMatrix();
	//glTranslatef(0, 0, 0);
	glTranslatef(((OFFSCREEN_WIDTH - packet.width)/2), (OFFSCREEN_HEIGHT-packet.height), 0);
	
	bool bDrawPeople = false;
	
	if(bDrawPeople) {
		for(int i=0; i<packet.nBlobs; i++) {
			ofSetColor(255, i*20, 255-(i*40), 100);
			ofFill();
			ofEnableSmoothing();
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
	ofSetColor(255, 0, 0);
	//tracker->draw(0, 0);
	
	
	// --------------------- building shape
	for(int i=0; i<box2dBuilding.size(); i++) {
		box2dBuilding[i].draw();	
	}
	
	
	glPopMatrix();
	
	
	
	
	
	
	
	
	// ferry building only for setup (hide when live)
	//ferryBuilding.drawContour();
	//ferryBuilding.drawInfo();
	
	
	
	ofDisableAlphaBlending();
	
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
}

//--------------------------------------------------------------
void MonsterScene::mousePressed(int x, int y, int button) {
	
	
	ferryBuilding.mousePressed(x, y, button);
	
}



























