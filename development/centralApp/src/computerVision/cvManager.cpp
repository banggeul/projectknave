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
	
	
	setupGUI();
	
	//cout << "creating a packet, size = " << endl;
	//cout << sizeof(computerVisionPacket) << endl;
	
	// load XML :
	printf("--------------------------------------------------------------------------- \n");
	printf("----------------------------- loading cvManager settings ------------------ \n");
	printf("--------------------------------------------------------------------------- \n");
	XML.loadFile("settings/cvManagerSettings.xml");
	int sceneId = XML.getValue("cvManager:sceneId",0);
	printf("scene =  %i \n", sceneId);
	int nVideos = XML.getValue("cvManager:sceneInfo:scene" + ofToString(sceneId) + ":nVideoSources", 0);
	printf("nVideos =  %i \n", nVideos);
	for (int i = 0; i <  nVideos; i++){
		string fileName = XML.getValue("cvManager:sceneInfo:scene" + ofToString(sceneId) + ":video" + ofToString(i), "..");
		cout << "fileName: " << fileName << endl;
	}
	for (int i = 0; i <  nVideos; i++){
		int videoGrabberSource = XML.getValue("cvManager:sceneInfo:scene" + ofToString(sceneId) + ":videoGrabberSource" + ofToString(i), 100);
		//cout << "cvManager:sceneInfo:scene" + ofToString(sceneId) + ":videoGrabberSource" + ofToString(i) << endl;
		cout << "videoGrabberSource: " << videoGrabberSource << endl;
	}
	printf("--------------------------------------------------------------------------- \n");
	
}


cvManager::~cvManager(){
	
	if (bNetworkSetup == true){
		manager.Close();
	}
}


void cvManager::setupGUI(){
	panel.setup("cv panel", 700, 20, 300, 450);
	panel.addPanel("overview", 1, false);
	panel.addPanel("geometry", 1, false);
	panel.addPanel("adjustments", 1, false);
	
	panel.setWhichPanel("overview");
	panel.addToggle("live video", "CV_MANAGER_LIVE_VIDEO", false);
}

void cvManager::updateGUI(){
	
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
	
	panel.update();
	
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
void cvManager::setupReceiver(){
	if (!bNetworkSetup){
		
		bool bOk = false;
		/*
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
		
		*/
		TCPServer.setup(11999);
		
		bNetworkSetup = true;
	}
}

//---------------------------				
void cvManager::setupSender(){
	if (!bNetworkSetup){
		
		/*
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
		*/
		bNetworkSetup = true;
		weConnected = tcpClient.setup("127.0.0.1", 11999);
		connectTime = 0;
		deltaTime = 0;
	}
}


//---------------------------				
void cvManager::receiveFromNetwork(){
	
	if (!bNetworkSetup) setupReceiver();
	
	
	//for each client lets send them a message letting them know what port they are connected on
	for(int i = 0; i < TCPServer.getNumClients(); i++){
		TCPServer.send(i, "hello client - you are connected on port - "+ofToString(TCPServer.getClientPort(i)) );
	}
	
	for(int i = 0; i < TCPServer.getNumClients(); i++){
		
		int nGot = TCPServer.receiveRawBytes(i, (char *)packet, sizeof(computerVisionPacket));
		//printf("got %i bytes from %i \n", nGot, i);
		
	}
	
	///char temp[1000];
	
	/*int recv;
	while (recv = manager.Receive((char *) UDPpacket, sizeof(udpPacket)) > 0){
		int chunk = UDPpacket->whichChunk;
		int packetId = UDPpacket->packetId;
		printf("received %i bytes of  chunk %i, of packet %i  \n", chunk, packetId);
		UDPC.receiveDataChunk(UDPpacket->data, chunk, packetId);
		UDPC.update();
	}*/
	
	
}

//---------------------------				
void cvManager::sendToNetwork(){
	
	
	if (!bNetworkSetup) setupSender();
	
	/*UDPC.convertToChunkedPacketForSending((unsigned char * )packet, sizeof(computerVisionPacket));
	
	for (int i = 0; i < UDPC.nChunks; i++){
		dataChunk * DC = UDPC.getChunk(i);
		UDPpacket->packetId = ofGetFrameNum();
		UDPpacket->whichChunk = i;
		memcpy(UDPpacket->data, DC->data, DC->length);			// this doesn't have to be a memcpy....
		int sent = manager.Send((char *) UDPpacket, sizeof(udpPacket));
		printf("sent %i bytes of chunk %i, of packet %i  \n", sent, i,  (int)UDPpacket->packetId);
	}
	*/
	
	//we are connected - lets send our text and check what we get back
	if(weConnected){
		
		
		tcpClient.sendRawBytes((char *)packet, sizeof(computerVisionPacket));
		
		
		//if data has been sent lets update our text
		string str = tcpClient.receive();
		
		/*if( str.length() > 0 ){
			msgRx = str;
		}*/
	}else{
		//if we are not connected lets try and reconnect every 5 seconds
		deltaTime = ofGetElapsedTimeMillis() - connectTime;
		
		if( deltaTime > 5000 ){
			weConnected = tcpClient.setup("127.0.0.1", 11999);
			connectTime = ofGetElapsedTimeMillis();
		}
		
	}
	
	
}


//---------------------------				
void cvManager::draw(float x, float y, int w, int h){
	ofSetColor(255, 255, 255);
	//VideoFrame.draw(x, y, w, h);
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
	
	
	panel.draw();
}





//void trackingManager::setupGui(){
//	
//	panel.setup("cv panel", 700, 20, 300, 450);
//	panel.addPanel("image adjustment", 1, false);
//	panel.addPanel("edge fixer", 1, false);
//	panel.addPanel("blob detection", 1, false);
//	panel.addPanel("glint detection", 1, false);
//	
//	if (IM.mode == INPUT_VIDEO){
//		panel.addPanel("video file settings", 1, false);
//	} else {
//		panel.addPanel("live video settings", 1, false);
//	}
//	
//	//---- gaze
//	panel.setWhichPanel("image adjustment");
//	panel.setWhichColumn(0);
//	
//	panel.addToggle("flip horizontal ", "B_RIGHT_FLIP_X", false);
//	panel.addToggle("flip vertical ", "B_RIGHT_FLIP_Y", false);
//	
//	panel.addToggle("use contrast / bri", "B_USE_CONTRAST", true);
//	panel.addSlider("contrast ", "CONTRAST", 0.28f, 0.0, 1.0f, false);
//	panel.addSlider("brightness ", "BRIGHTNESS", -0.02f, -1.0, 3.0f, false);
//	
//	panel.addToggle("use gamma ", "B_USE_GAMMA", true);
//	panel.addSlider("gamma ", "GAMMA", 0.57f, 0.01, 3.0f, false);
//	
//	panel.addSlider("threshold ", "THRESHOLD_GAZE", threshold, 0, 255, true);
//	
//	
//	
//	
//	panel.setWhichPanel("blob detection");
//	panel.addToggle("use dilate", "B_USE_DILATE", true);
//	panel.addSlider("dilate num ", "N_DILATIONS", 0, 0, 10, true);
//    panel.addSlider("min blob","MIN_BLOB",10*10,0,5000,true);
//    panel.addSlider("max blob","MAX_BLOB",100*100,0,50500,true);
//	
//	//---- tracker edges
//	panel.setWhichPanel("edge fixer");
//	panel.setWhichColumn(0);
//	panel.addSlider("x position ", "EDGE_MASK_X", 320, 0, 640, true);
//	panel.addSlider("y position ", "EDGE_MASK_Y", 240, 0, 640, true);
//	panel.addSlider("inner radius ", "EDGE_MASK_INNER_RADIUS", 250, 0, 500, true);
//	panel.addSlider("outer radius ", "EDGE_MASK_OUTER_RADIUS", 350, 0, 600, true);
//	
//	
//	panel.setWhichPanel("glint detection");
//	panel.setWhichColumn(0);
//	panel.addSlider("glint threshold ", "GLINT_THRESHOLD", 80,0,255, false);
//	panel.addSlider("glint min size ", "GLINT_MIN_SIZE", 10,0,1000, true);
//	panel.addSlider("glint max size ", "GLINT_MAX_SIZE", 1000,0,10000, true);
//	
//	
//	
//	
//	if (IM.mode == INPUT_VIDEO){
//		panel.setWhichPanel("video file settings");
//		// TODO: add theo's video playing things.... [zach]
//	} else {
//		panel.setWhichPanel("live video settings");
//		panel.addToggle("load video settings", "VIDEO_SETTINGS", false);
//	}
//	
//	
//	
//	panel.loadSettings("settings/trackingSettings.xml");
//	
//	
//}
//
//void trackingManager::updateGui(){
//	
//	tracker.flip(  panel.getValueB("B_RIGHT_FLIP_X"),  panel.getValueB("B_RIGHT_FLIP_Y") );
//	
//	minBlob = panel.getValueI("MIN_BLOB");
//	maxBlob = panel.getValueI("MAX_BLOB");
//	
//	threshold				= panel.getValueI("THRESHOLD_GAZE");
//	
//	tracker.gamma			= panel.getValueF("GAMMA");
//	tracker.bUseGamma		= panel.getValueB("B_USE_GAMMA");
//	
//	tracker.contrast		= panel.getValueF("CONTRAST");
//	tracker.brightness		= panel.getValueF("BRIGHTNESS");
//	tracker.bUseContrast	= panel.getValueB("B_USE_CONTRAST");
//	
//	tracker.nDilations		= panel.getValueI("N_DILATIONS");
//	tracker.bUseDilate		= panel.getValueB("B_USE_DILATE");
//	
//	int oldx				= tracker.edgeMaskStartPos.x;
//	int oldy				= tracker.edgeMaskStartPos.y;
//	int oldir				= tracker.edgeMaskInnerRadius;
//	int oldor				= tracker.edgeMaskOuterRadius;
//	
//	tracker.edgeMaskStartPos.x		= panel.getValueI("EDGE_MASK_X");
//	tracker.edgeMaskStartPos.y		= panel.getValueI("EDGE_MASK_Y");
//	tracker.edgeMaskInnerRadius	= panel.getValueI("EDGE_MASK_INNER_RADIUS");
//	tracker.edgeMaskOuterRadius	= panel.getValueI("EDGE_MASK_OUTER_RADIUS");
//	
//	tracker.glintThreshold = panel.getValueI("GLINT_THRESHOLD");
//	tracker.minGlintSize = panel.getValueI("GLINT_MIN_SIZE");
//	tracker.maxGlintSize = panel.getValueI("GLINT_MAX_SIZE");
//	
//	if (	oldx	!= tracker.edgeMaskStartPos.x  ||
//		oldy	!= tracker.edgeMaskStartPos.y  ||
//		oldir	!= tracker.edgeMaskInnerRadius ||
//		oldor	!= tracker.edgeMaskOuterRadius	){		
//		
//		tracker.calculateEdgePixels();
//		
//	}
//	
//	if (IM.mode != INPUT_VIDEO){
//		panel.setWhichPanel("live video settings");
//		if (panel.getValueB("VIDEO_SETTINGS") == true){
//			
//#ifdef TARGET_OSX
//			// since macs fuck up bad fullscreen with video settings
//			ofSetFullscreen(false);
//#endif
//			IM.vidGrabber.videoSettings();
//			panel.setValueB("VIDEO_SETTINGS", false);
//		}
//	}
//	
//}


