#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
	
	ofBackground(20, 20, 20);
	ofSetVerticalSync(true);
	
	video.loadMovie("silhouettes-sorenson.mov");
	video.play();
	
	cw = 640;
	ch = 480;
	
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
	tracker.setup(250);
	
	
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
		
		
		
		{	
			//TRACKER  
			//we do this whether or not we have found any blobs because the tracker also kills its stored blobs
			//if they haven't been matched for a while.
			tracker.begin();
			
			
			
			
			if( numFound > 0){
				
				//allocate our array (vector) with enough space for the number of blobs we have found
				//trackedBlobs.assign(numFound, trackedBlob());
				trackedBlobs.clear();
				
				//loop through each blob found
				for(int i = 0; i < numFound; i++){
					
					//copy each blob over to our array
					trackedBlobs.push_back(trackedBlob());
					trackedBlobs[i] = contourFinder.blobs[i];
					
					
					//see if the blob is one known from previous frames
					bool blobMatched = tracker.matchTrackedBlob( trackedBlobs[i], distanceThresh );
					
					//if the blob can't be matched against the stored blobs
					//add it to the stored blobs
					if( blobMatched == false ){
						
						tracker.addBlob(trackedBlobs[i]);
//						printf("added %i \n", trackedBlobs[i].id);	

						//	wiggleMonsters.push_back(WiggleMonster());
						//	wiggleMonsters.back().root = trackedBlobs[i].centroid;
						//	wiggleMonsters.back().id = trackedBlobs[i].id;
					}
				}
			}
			
			
			// loop through and see who is dead and who is alive
			for(int i=0; i<trackedBlobs.size(); i++) {
				
				if(!trackedBlobs[i].assigned) {
					printf("-- new blob %i --\n", trackedBlobs[i].id);
				}
				
			}
			
			
			//printf("-- total:%i ---\n", (int)trackedBlobs.size());
			
			
			//this is where stored blobs that need to die, get killed :)
			tracker.end();
			
			//printf("-- total:%i ---\n", (int)trackedBlobs.size());
		}
		
		
	}
	
	
	// Monsters
	for(int i=0; i<wiggleMonsters.size(); i++) {
		wiggleMonsters[i].update();	
		
		for(int j=0; j<trackedBlobs.size(); j++) {
			if(wiggleMonsters[i].id == trackedBlobs[i].id) {
				
				float x = trackedBlobs[i].centroid.x / (float)cw;
				float y = trackedBlobs[i].centroid.y / (float)ch;
				
				wiggleMonsters[i].root.set(x * 320.0, y * 240.0);
			}
		}
		
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	glPushMatrix();
	glTranslatef(0, ofGetHeight()-240, 0);
	ofSetColor(0xffffff);
	cvGray.draw(0, 0, 320, 240);
	cvThresh.draw(320, 0, 320, 240);
	contourFinder.draw(320, 0, 320, 240);
	
	
	for(int i = 0; i < trackedBlobs.size(); i++) {
		
		if( trackedBlobs[i].bNew ){
			ofSetColor(0xFFFF00);
		}else{
			ofSetColor(0x0000FF);
		}
		
		float x = trackedBlobs[i].centroid.x / (float)cw;
		float y = trackedBlobs[i].centroid.y / (float)ch;
		
		ofFill();
		ofCircle(x * 320.0, y * 240.0, 4);
		
		ofSetColor(0xFF0000);
		string bInfo ="";
		bInfo += "\nid: "+ofToString(trackedBlobs[i].id);
		bInfo += "\nage: "+ofToString((float)trackedBlobs[i].frameAge);
		
		ofDrawBitmapString(bInfo, x * 320.0, y * 240.0);		
	}
	
	
	
	// Monsters
	for(int i=0; i<wiggleMonsters.size(); i++) {
		wiggleMonsters[i].draw();	
	}
	
	glPopMatrix();
	
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
	wiggleMonsters.push_back(WiggleMonster());
	wiggleMonsters.back().root.set(x, y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
	
}
