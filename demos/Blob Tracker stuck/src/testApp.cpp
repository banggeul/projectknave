#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
	
	ofBackground(120, 120, 120);
	ofSetVerticalSync(true);
	
	video.loadMovie("wide-silhouettes-sorenson.mov");
	video.play();
	
	cw = 640;
	ch = 240;
	
	//allocate the openCV images
	cvColor.allocate(video.width, video.height);
	cvGray.allocate(video.width, video.height);
	cvBackground.allocate(video.width, video.height);
	cvThresh.allocate(video.width, video.height);
	
	
	//-----
	backSubMode = 2;
	threshold   = 30;
	
	//how far a blob has to move between frames before it is treated as a new blob
	//you usually have to adjust this based on the size of the video and the people within it.
	//basically this is 'how far do you think someone would normally move between frames measured in pixels'
	distanceThresh = 80;
	
	//we say that we are interested in previous blobs that are younger than 500ms
	//this means that if we don't see a stored blob for more than half a second we forget about it.
	//this is essentially the memory in milliseconds of your tracker
	tracker.setup(500);
	
	
}

//--------------------------------------------------------------
void testApp::update() {
	
	video.idleMovie();
	
	if(video.isFrameNew()){
		
		unsigned char * pixels = video.getPixels();
		cvColor.setFromPixels(pixels, video.width, video.height);
		
		cvGray = cvColor;
		cvGray.threshold(threshold, true);
		
		int numFound = contourFinder.findContours(cvGray, 10 * 10, (cw * ch)/3, 100, false, false);
		
		
		//TRACKER  
		//we do this whether or not we have found any blobs because the tracker also kills its stored blobs
		//if they haven't been matched for a while.
		tracker.begin();
		
		
		
		
		if( numFound > 0){
			
			//allocate our array (vector) with enough space for the number of blobs we have found
			trackedBlobs.assign(numFound, trackedBlob());
			
			//loop through each blob found
			for(int i = 0; i < numFound; i++){
				
				//copy each blob over to our array
				trackedBlobs[i] = contourFinder.blobs[i];
				
				
				//see if the blob is one known from previous frames
				bool blobMatched = tracker.matchTrackedBlob( trackedBlobs[i], distanceThresh );
				
				//if the blob can't be matched against the stored blobs
				//add it to the stored blobs
				if(blobMatched == false ) {
					
					tracker.addBlob(trackedBlobs[i]);
					
					// i guess here i could see how many blobs i have 
					// and find the closet person to match against. then 
					// just swap the monster to a new blob.
					
				}
			}
			
			
			//this is where stored blobs that need to die, get killed :)
			tracker.end();
			
			
			
			
			
			
			
			
			
			// --------------------------------------
			// Ok add a monster if we have a new blob
			// --------------------------------------
			for(int i=0; i<trackedBlobs.size(); i++) {
				
				if(trackedBlobs[i].bNew) {
					
					WiggleMonster monster;
					monster.id = trackedBlobs[i].id;
					monster.init(trackedBlobs[i].centroid);
					wiggleMonsters.push_back(monster);
					
					
					printf("added monster - %i\n", trackedBlobs[i].id);
					
				}
				
			}
			
			// --------------------------------------
			// Match the blobs and update
			// --------------------------------------
			for(int i=0; i<wiggleMonsters.size(); i++) {
				for(int j=0; j<trackedBlobs.size(); j++) {
					
					if(trackedBlobs[j].id == wiggleMonsters[i].id) {
						
						float x = trackedBlobs[j].centroid.x;
						float y = trackedBlobs[j].centroid.y;
						
						wiggleMonsters[i].updatePnts(trackedBlobs[j].pts);	
						wiggleMonsters[i].pos.set(x, y);
						
					}
					
				}
			}
			
			// --------------------------------------
			// See if a blob ids that not around
			// --------------------------------------
			vector <int> monsterToBury;
			monsterToBury.clear();
			
			for(int i = wiggleMonsters.size()-1; i >= 0; i--){
				
				bool bStillAround = false;
				
				for(int j = trackedBlobs.size()-1; j >= 0; j--){
					
					if(wiggleMonsters[i].id == trackedBlobs[j].id) {
						bStillAround = true;
					}
				}
				
				if(!bStillAround) {
					wiggleMonsters[i].dead = true;
					monsterToBury.push_back(i);
				}
				
			}
			
			
			// --------------------------------------
			// ok send them off to monster heaven
			// --------------------------------------
			for(int i=0; i<monsterToBury.size(); i++) {
				printf("Bury monster - %i \n", wiggleMonsters[monsterToBury[i]].id);
				wiggleMonsters.erase(wiggleMonsters.begin() + monsterToBury[i]);
			}
			
			
		}
	}
	
	
	
	
	// Monsters
	for(int i=0; i<wiggleMonsters.size(); i++) {
		wiggleMonsters[i].update();	
	}
	
	
	
	
}

//--------------------------------------------------------------
void testApp::draw() {
	
	
	glPushMatrix();
	glTranslatef((ofGetWidth()-cw)/2, ofGetHeight()-ch, 0);
	ofSetColor(0xffffff);
	cvGray.draw(0, 0);
	contourFinder.draw(0, 0);
	
	
	for(int i = 0; i < trackedBlobs.size(); i++) {
		
		if( trackedBlobs[i].bNew ){
			ofSetColor(0xFFFF00);
		}else{
			ofSetColor(0x0000FF);
		}
		
		float x = trackedBlobs[i].centroid.x;
		float y = trackedBlobs[i].centroid.y;
		
		ofFill();
		ofCircle(x, y, 4);
		
		ofSetColor(0xFF0000);
		string bInfo ="";
		bInfo += "\nid: "+ofToString(trackedBlobs[i].id);
		bInfo += "\nage: "+ofToString((float)trackedBlobs[i].frameAge);
		
		ofDrawBitmapString(bInfo, x, y);		
	}
	
	
	
	
	// Monsters
	for(int i=0; i<wiggleMonsters.size(); i++) {
		wiggleMonsters[i].draw();	
	}
	glPopMatrix();

	
	// Monster Data
	string monsterInfo = "Monster Data\n";
	for(int i=0; i<wiggleMonsters.size(); i++) {
		monsterInfo += "MID: "+ofToString(wiggleMonsters[i].id)+"\n";
	}
	ofDrawBitmapString(monsterInfo, 10, 10);		
	
	
	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	
	//reset the tracker and id count
	if(key == 'r'){
		tracker.resetCounter();
	}
	
	if( key == OF_KEY_UP ){
		threshold++;
		if(threshold > 255)threshold = 255;
	}else if(key == OF_KEY_DOWN){
		threshold--;
		if(threshold < 0)threshold = 0;
	}
	
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){ 
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
	
}
