

#include "MonsterScene.h"
#include "ofxVectorMath.h"

//-------------------------------------------------------------- setup
void MonsterScene::setup(){
	
	
	
}

//-------------------------------------------------------------- update
void MonsterScene::update(){
	
	
	contourAnalysis.setSize(packet.width, packet.height);

	// --------------------- Monsters
	for(int i = 0; i < monsters.size(); i++) {
		monsters[i].update();
	}
	
	
	
	// update the pnts to the blob and monser
	/*for(int i=0; i<tracker->blobs.size(); i++) {
	 for(int j=0; j<monsters.size(); j++) {
	 if(tracker->blobs[i].id == monsters[j].id) {
	 monsters[j].updateContourPnts(tracker->blobs[i].pts);
	 }
	 }
	 }
	 */
	for(int i=0; i<tracker->blobs.size(); i++) {
		
		int lookID = tracker->blobs[i].id;
		
		for(int j=monsters.size()-1; j>=0; j--) {
			
			// yes we match
			if(monsters[j].monsterID == lookID) {
				
				monsters[j].pos = tracker->blobs[i].centroid;
				monsters[j].updateContourPnts(tracker->blobs[i].pts);
				
				monsters[j].contourSimple.assign(tracker->blobs[i].pts.size(), ofPoint());
				contourAnalysis.simplify(tracker->blobs[i].pts, monsters[j].contourSimple, 0.50);

				monsters[j].contourSmooth.assign(monsters[j].contourSimple.size(), ofPoint());
				contourAnalysis.smooth(monsters[j].contourSimple, monsters[j].contourSmooth, 0.2);
				
								
			}
		}
	}
	
	
}

//-------------------------------------------------------------- get monster by id
BubbleMonster&  MonsterScene::getMonsterById( int monsterId ) {
	
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
			//ofCvTrackedBlob tempB = tracker->getById(id);
			//monsters[i].updateContourPnts(tracker->getById(id).pts);
			monsters[i].genNewRadius();
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
	
	ofSetColor(255, 0, 0);
	ofLine (0,0,OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	ofLine (0,OFFSCREEN_HEIGHT,OFFSCREEN_WIDTH, 0);
	
	
	
	float scalex = (float)OFFSCREEN_WIDTH / (float)packet.width;
	float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;
	
	
	
	ofEnableAlphaBlending();
	
	// --------------------- People
	glPushMatrix();
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
	
	
	// --------------------- Monsters
	for(int i = 0; i < monsters.size(); i++) {
		monsters[i].draw();
	}
	
	
	// --------------------- tracker
	ofSetColor(255, 0, 0);
	//tracker->draw(0, 0);
	
	
	glPopMatrix();
	
	
	
	ofDisableAlphaBlending();
	
}