#include "cvManager.h"

cvManager::cvManager(){
	bSetup				= false;
	
	threshold			= 80;
	maxBlobSize			= 320*240*3;
	minBlobSize			= 100;
	
	int						threshold;
	int						maxBlobSize, minBlobSize;
	
	
	packet = new computerVisionPacket();
	packetData = new unsigned char [sizeof(computerVisionPacket)];
	
	bNetworkSetup = false;
	port = 11999;
	IP = "127.0.0.1";	
	
	cout << "creating a packet, size = " << endl;
	cout << sizeof(computerVisionPacket) << endl;
	

}


cvManager::~cvManager(){
	
	if (bNetworkSetup == true){
		manager.Close();
	}
}


void cvManager::fillPacket(){
	
	
	memset( (char *)(packet), 0, sizeof(computerVisionPacket));
	
	int nBlobs = MIN(Contour.nBlobs, MAX_N_BLOBS);
	packet->nBlobs = nBlobs;
	
	for (int i = 0; i < nBlobs; i++){
		
		int nPts = Contour.blobs[i].nPts;
		
		if (nPts < MAX_BLOB_LENGTH){
			
			packet->nPts[i] = nPts;
			for (int j = 0; j < nPts; j++){
				packet->pts[i][j] = Contour.blobs[i].pts[j];
			}
			
		} else {
			
			// do some resampling ;)
			// cout << SR.pts.size() << endl; 
			SR.clear();
			ofPoint temp;
			for (int j = 0; j < nPts; j++){
				temp =  Contour.blobs[i].pts[j];
				SR.addPoint(temp.x, temp.y);
			}
			SR.resample(MAX_BLOB_LENGTH);
			packet->nPts[i] = MAX_BLOB_LENGTH;
			for (int j = 0; j < MAX_BLOB_LENGTH; j++){
				packet->pts[i][j] = SR.pts[j];
			}
		}
		
	}
}



//changing cameras or switching from/to the camera mode
//requires the app to be restarted - mabe we can change this?
//---------------------------		
void cvManager::setupCamera(int deviceNumber, int _width, int _height){
	
	bSetup				= false;
	
	VG.setVerbose(true);
	//VG.setDeviceID(deviceNumber);
	
	
	VG.setDeviceID(deviceNumber);
	VG.initGrabber(_width, _height);
	width = VG.width;
	height = VG.height;	
	bUsingVideoGrabber = true;
	
	
	setupCV();
}

//same as above - from/to using test movies
//requires a restart
//---------------------------		
void cvManager::setupVideo(string videoPath){
	
	bSetup				= false;
	
	VP.loadMovie(videoPath);
	VP.play(); 
	VP.setPosition(0.0);
	VP.setSpeed(0.90);
	VP.setUseTexture(true);
	width = VP.width;
	height = VP.height;
	bUsingVideoGrabber = false;
	
	setupCV();
}

//good for adjusting the color balance, brightness etc
//---------------------------		
void cvManager::openCameraSettings(){
	if(bUsingVideoGrabber)VG.videoSettings();			
}




//do all our openCV allocation
//---------------------------		
void cvManager::setupCV(){
	
	if(bSetup) return;
	
	//our openCV inits
	VideoFrame.allocate(width, height);
	PresenceFrame.allocate(width, height);
	PresenceFrameDialate.allocate(width, height);	
	GreyFrame.allocate(width, height);
	
	
	bSetup = true;
}


bool cvManager::isFrameNew(){
	if(bUsingVideoGrabber){
		return VG.isFrameNew();
	} else {
		return VP.isFrameNew();
		
	}
	return false;
}


//standard video processing
//---------------------------		
void cvManager::update(){
	
	
	
	///////////////////////////////////////////////////////////
	// Part 1 - get the video data
	///////////////////////////////////////////////////////////
	
	//pointer to our incoming video pixels			
	unsigned char * pixCam;
	
	
	if(bUsingVideoGrabber){
		VG.grabFrame();
		if (!VG.isFrameNew())  return;
		pixCam 	= VG.getPixels();		
	} else {
		VP.idleMovie();	
		if (!VP.isFrameNew())return;
		pixCam 	= VP.getPixels();
	}
	
	
	
	///////////////////////////////////////////////////////////
	// Part 2 - load into openCV and make greyscale
	///////////////////////////////////////////////////////////
	
	
	// this is a hack for this video. 
	// masking (live) would be helpful !! (draw out regions we dont' care about).
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height/2; j++){
			pixCam[(j*width + i) * 3    ] = 255;
			pixCam[(j*width + i) * 3 + 1] = 255;
			pixCam[(j*width + i) * 3 + 2] = 255;
		}
	}
	
	
	VideoFrame.setFromPixels(pixCam,width,height);
	
	//VideoFrame.setFromPixels(pixCam, W, H);
	GreyFrame.setFromColorImage(VideoFrame);
	
	///////////////////////////////////////////////////////////
	// Part 3 - do thresholding
	///////////////////////////////////////////////////////////
	PresenceFrame = GreyFrame;	
	PresenceFrame.threshold(threshold);
	PresenceFrame.invert();
	PresenceFrameDialate = PresenceFrame;
	PresenceFrameDialate.dilate_3x3();
	PresenceFrameDialate.erode_3x3();
	
	
	Contour.findContours(PresenceFrameDialate, minBlobSize, maxBlobSize, 50, false, false);  
	
	fillPacket();	// THIS IS FOR THE NETWORK COMMUNICATON
}

//---------------------------				
void cvManager::receiveFromNetwork(){
	
	if (!bNetworkSetup){
		
		bool bOk = false;
		
		// --------------------------------- create
		bOk = manager.Create();
		if (!bOk) {   
			printf("network, create failed \n");
			return;
		}
		
		// --------------------------------- set non blocking
		bOk = manager.Bind( port);
		if (!bOk) {   
			printf("network, setNonBlocking failed \n");
			return;
		}
		
		// --------------------------------- set non blocking
		bOk = manager.SetNonBlocking(true);
		if (!bOk) {   
			printf("network, setNonBlocking failed \n");
			return;
		}
		
		
		bNetworkSetup = true;
	}
	
	int recv = manager.Receive((char *)packet, sizeof(computerVisionPacket));
	cout << "received " << recv << endl;
}

//---------------------------				
void cvManager::sendToNetwork(){
	if (!bNetworkSetup){
		
		
		bool bOk = false;
		
		// --------------------------------- create
		bOk = manager.Create();
		if (!bOk) {   
			printf("network, create failed \n");
			return;
		}
		
		// --------------------------------- connect
		bOk = manager.Connect(IP.c_str(), port);
		if (!bOk) {   
			printf("network, Connect failed \n");
			return;
		}
		
		bNetworkSetup = true;
	}
	
	int sent = manager.Send( (char *) packet, sizeof(computerVisionPacket));
	cout << "sent " << sent << endl;
	
}


//---------------------------				
void cvManager::draw(float x, float y, int w, int h){
	ofSetColor(255, 255, 255);
	VideoFrame.draw(x, y, w, h);
	//Contour.draw(x, y, w, h);
	
	// draw the packet, to see if it's OK
	glPushMatrix();
	glTranslatef(x,y,0) ;
	glScalef(w / (float)width, h / (float)height, 1);
	for (int i = 0; i < packet->nBlobs; i++){
		ofNoFill();
		ofSetColor(255, 0, 0);
		ofBeginShape();
		for (int j = 0; j < packet->nPts[i]; j++){	
			ofVertex(packet->pts[i][j].x, packet->pts[i][j].y);
		}
		ofEndShape(true);
	}
	glPopMatrix();
	
}



