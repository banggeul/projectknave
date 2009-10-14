#include "cvManager.h"

cvManager::cvManager(){
	bSetup				= false;
	bUseFBOSticher      = false;
}

void cvManager::setupNonCV(){
	
	inputW					= 320;
	inputH					= 240;
	
	//threshold				= 80;
	//maxBlobSize			= 320*240*3;
	//minBlobSize			= 100;
	
	
	packet = new computerVisionPacket();
	packetData = new unsigned char [sizeof(computerVisionPacket)];
	
	bNetworkSetup = false;
	port = 11999;
	IP = "127.0.0.1";	
	
	
	// load XML :
	printf("----------------------------------------- \n");
	printf("------ loading cvManager settings ------- \n");
	printf("----------------------------------------- \n");
	XML.loadFile("settings/cvManagerSettings.xml");
	
	
	
	bLive = (bool) XML.getValue("cvManager:bUseLiveVideo", 0);
	printf("bLive =  %i \n", (int)bLive == true ? 1 : 0);
	sceneId = XML.getValue("cvManager:sceneId",0);
	printf("scene =  %i \n", sceneId);
	nVideos = XML.getValue("cvManager:sceneInfo:scene" + ofToString(sceneId) + ":nVideoSources", 0);
	printf("nVideos =  %i \n", nVideos);
	
	for (int i = 0; i <  nVideos; i++){
		string fileName = XML.getValue("cvManager:sceneInfo:scene" + ofToString(sceneId) + ":video" + ofToString(i), "..");
		videoFile[i] = fileName;
		cout << "fileName: " << fileName << endl;
	}
	
	for (int i = 0; i <  nVideos; i++){
		int videoGrabberSource = XML.getValue("cvManager:sceneInfo:scene" + ofToString(sceneId) + ":videoGrabberSource" + ofToString(i), 100);
		//cout << "cvManager:sceneInfo:scene" + ofToString(sceneId) + ":videoGrabberSource" + ofToString(i) << endl;
		videoSource[i] = videoGrabberSource;
		cout << "videoGrabberSource: " << videoGrabberSource << endl;
	}
	printf("----------------------------------------- \n");
	
	if (!bLive){
		// load all the videos need	
		for(int i=0; i< nVideos; i++) {
			setupVideo(videoFile[i]);
		}
	}
	else {
		for(int i=0; i< nVideos; i++) {
			
			printf("setup camera input - %i\n", videoSource[i]);	
			setupCamera(i, videoSource[i], inputW, inputH);
		}
	}
	
	// if we have more than one
	// video then lets use the sticher
	if(nVideos > 1) {
		bUseFBOSticher = true;
	}
	
	
	if(bUseFBOSticher) setFBOStich(width, height);
	setupCV();
	setupGUI();

	
	if(bLive) printf("\ncamera - w:%i h:%i\n", inputW, inputH);
	else 	  printf("\nvideo - w:%i h:%i\n", inputW, inputH);
	
	printf("cv - w:%i h:%i\n", width, height);
}

//------------------------------------------------------------------
cvManager::~cvManager(){
	
	if (bNetworkSetup == true){
		manager.Close();
	}
}

//------------------------------------------------------------------
void cvManager::setupGUI() {
	
	panel.setup("cv panel", 700, 10, 300, 750);
	panel.addPanel("input", 1, false);
		
	panel.addPanel("binary / blob", 1, false);
	
	panel.addPanel("geometry", 1, false);
	panel.addPanel("adjust ments", 1, false);
	
	
	
	
	panel.setWhichPanel("input");
	panel.addToggle("Video 0 Settings", "CAMERA_0_SETTING", false);
	panel.addToggle("Video 1 Settings", "CAMERA_1_SETTING", false);
	panel.addToggle("save coords", "STICH_COORDS", false);
	
	
	
	panel.setWhichPanel("binary / blob");
	panel.addToggle("flip horiz", "CV_MANAGER_PANEL_VIDEO_FLIP_HORIZ", false);
	panel.addToggle("flip vertical", "CV_MANAGER_PANEL_VIDEO_FLIP_VERT", false);					
	panel.addToggle("invert", "CV_MANAGER_PANEL_VIDEO_INVERT", false);	
	panel.addSlider("thrshold",  "CV_MANAGER_PANEL_VIDEO_THRESHOLD", 80, 0, 255, true);
	
	
	panel.addSlider("min blob size (pct of video)",  "CV_MANAGER_PANEL_MIN_BLOB", 0.01, 0,0.25, false);
	panel.addSlider("max blob size (pct of video)",  "CV_MANAGER_PANEL_MAX_BLOB", 0.5, 0,1, false);
	
	
	panel.setWhichPanel("geometry");
	panel.addSlider2D("pta", "CV_MANAGER_PANEL_VIDEO_PTA", 0, 0, 0, VideoFrame.width, 0, VideoFrame.height, true);
	panel.addSlider2D("ptb", "CV_MANAGER_PANEL_VIDEO_PTB", VideoFrame.width, 0, 0, VideoFrame.width, 0, VideoFrame.height, true);
	panel.addSlider2D("ptc", "CV_MANAGER_PANEL_VIDEO_PTC", VideoFrame.width, VideoFrame.height, 0, VideoFrame.width, 0, VideoFrame.height, true);
	panel.addSlider2D("ptd", "CV_MANAGER_PANEL_VIDEO_PTD", 0, VideoFrame.height, 0, VideoFrame.width, 0, VideoFrame.height, true);
	
	
	panel.addSlider("scale",  "CV_MANAGER_SCALE", 1,0.8, 1.4, false);
	
	warpFrom[0].set(0, 0);
	warpFrom[1].set(VideoFrame.width, 0);
	warpFrom[2].set(VideoFrame.width, VideoFrame.height);
	warpFrom[3].set(0, VideoFrame.height);
	warpTo[0].set(0, 0);
	warpTo[1].set(VideoFrame.width, 0);
	warpTo[2].set(VideoFrame.width, VideoFrame.height);
	warpTo[3].set(0, VideoFrame.height);
	
	
	
	//Slider("thrshold",  "CV_MANAGER_PANEL_VIDEO_THRESHOLD", 80, 0, 255, true);
	
	panel.loadSettings("settings/panels_xml/cvManagerPanel.xml");
	
}

void cvManager::updateGUI(){
	
	
	
	warpFrom[0].set( panel.getValueI("CV_MANAGER_PANEL_VIDEO_PTA", 0), panel.getValueI("CV_MANAGER_PANEL_VIDEO_PTA", 1));
	warpFrom[1].set( panel.getValueI("CV_MANAGER_PANEL_VIDEO_PTB", 0), panel.getValueI("CV_MANAGER_PANEL_VIDEO_PTB", 1));
	warpFrom[2].set( panel.getValueI("CV_MANAGER_PANEL_VIDEO_PTC", 0), panel.getValueI("CV_MANAGER_PANEL_VIDEO_PTC", 1));
	warpFrom[3].set( panel.getValueI("CV_MANAGER_PANEL_VIDEO_PTD", 0), panel.getValueI("CV_MANAGER_PANEL_VIDEO_PTD", 1));
	
	
}



//--------------------------------------------------------------
void cvManager::mouseDragged(int x, int y, int button){
	stichManger.mouseDragInputPoint(stichGui, ofPoint(x, y));
	panel.mouseDragged(x, y, button);
	
}

//--------------------------------------------------------------
void cvManager::mousePressed(int x, int y, int button){
	stichManger.mouseSelectInputPoint(stichGui, ofPoint(x, y));
	panel.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void cvManager::mouseReleased(){
	
	panel.mouseReleased();
	
}


//--------------------------------------------------------------
void cvManager::fillPacket(){
	
	
	memset( (char *)(packet), 0, sizeof(computerVisionPacket));
	
	int nBlobs = MIN(Contour.nBlobs, MAX_N_BLOBS);
	
	packet->myId = sceneId;
	packet->frameNumber = ofGetFrameNum();
	packet->width = VideoFrame.width;						// TODO: is videoFrame the right measurement here
	packet->height = VideoFrame.height;
	packet->nBlobs = nBlobs;
	
	for (int i = 0; i < nBlobs; i++){
		
		int nPts = Contour.blobs[i].nPts;
		
		packet->bAmInner[i] = !Contour.blobs[i].hole;
		packet->centroid[i] = Contour.blobs[i].centroid; // added blob centroids
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
	
	packet->checkSum = packet->myId + packet->frameNumber + packet->nBlobs;		
	
}





//changing cameras or switching from/to the camera mode
//requires the app to be restarted - mabe we can change this?
//---------------------------		
void cvManager::setupCamera(int which, int deviceNumber, int _width, int _height){
	
	bSetup				= false;
	
	VG[which].setVerbose(true);
	VG[which].setDeviceID(deviceNumber);
	
	
	VG[which].setDeviceID(deviceNumber);
	VG[which].initGrabber(_width, _height);
	width  = VG[which].width * nVideos;
	height = VG[which].height;	
	bUsingVideoGrabber = true;
	
	//setupCV(); // setupNonCV
	
}

//same as above - from/to using test movies
//requires a restart
//---------------------------		
void cvManager::setupVideo(string videoPath){
	
	bSetup				= false;
	
	for(int i=0; i<nVideos; i++) {
		VP[i].loadMovie(videoPath);	// from XML 
		VP[i].play(); 
		VP[i].setPosition(0.0);
		VP[i].setSpeed(0.90);
		VP[i].setUseTexture(true);
	}
	
	
	bUsingVideoGrabber = false;
	
	// if we are loading video files
	// set the inputW/inputH to the
	// video we are loading (assmuing they are the same size)
	
	inputW = VP[0].width;
	inputH = VP[0].height;
	
	// how big is the fbo texture 
	width  = inputW * nVideos;
	height = inputH;
	
	//setupCV(); // move to setupNonCV
}


//--------------------------- fbo strich		
void cvManager::setFBOStich(int fboW, int fboH) {
	
	if(bSetup || !bUseFBOSticher) return;
	
    stichManger.allocateForNScreens(nVideos, fboW, fboH);
    stichManger.loadFromXml("settings/fboStichSettings.xml");
    stichGui   = ofRectangle(0, 0, fboW, fboH);
	dualImage.allocate(fboW, fboH, OF_IMAGE_COLOR);
	
	printf("fbo sticher allocated\n");
	
}

//good for adjusting the color balance, brightness etc
//---------------------------		
void cvManager::openCameraSettings(int which){
	if(bUsingVideoGrabber) VG[which].videoSettings();			
}




//do all our openCV allocation
//---------------------------		
void cvManager::setupCV(){
	
	if(bSetup) return;
	
	//our openCV inits
	VideoFrame.allocate(width, height);
	PresenceFrame.allocate(width, height);
	
	PresenceFrameDialate.setUseTexture(false);
	PresenceFrameDialate.allocate(width, height);	
	GreyFrame.setUseTexture(false);
	GreyFrame.allocate(width, height);
	GreyFramePostWarp.setUseTexture(false);
	GreyFramePostWarp.allocate(width, height);
	
	
	printf("cv allocated\n");
	
	bSetup = true;
}


bool cvManager::isFrameNew(){
	if(bUsingVideoGrabber){
		return VG[0].isFrameNew();
	} else {
		return VP[0].isFrameNew();
		
	}
	return false;
}


//standard video processing
//---------------------------		
void cvManager::update(){
	
	
	
	panel.update();
	updateGUI();
	
	
	// open video settings
	bool bCamSettings_0 = panel.getValueB("CAMERA_0_SETTING");
	bool bCamSettings_1 = panel.getValueB("CAMERA_1_SETTING");
	
	if(bCamSettings_0) {
		VG[0].videoSettings();
		panel.setValueB("CAMERA_0_SETTING", false);	
	}
	if(bCamSettings_1) {
		VG[1].videoSettings();
		panel.setValueB("CAMERA_1_SETTING", false);	
	}
	
	
	///////////////////////////////////////////////////////////
	// Part 1 - get the video data [based on number of inputs]
	///////////////////////////////////////////////////////////
	
	//pointer to our incoming video pixels			
	unsigned char * pixCam;
	
	// are we using a video grabber
	if(bUsingVideoGrabber) {
		
		if(bUseFBOSticher) {
			for(int i=0; i<nVideos; i++) {
				VG[i].grabFrame();
			}
			
			// return if we do not have a new frame
			bool bNewFrame = false;
			for(int i=0; i<nVideos; i++) {
				if(VG[i].isFrameNew()) bNewFrame = true;
			}
			if(!bNewFrame) return;
		}
		
		else {
			
			pixCam 	= VG[0].getPixels();	
			
		}
		
	} 
	
	
	// or are we using a video file
	else {
		
		// if we have more than 1 video 
		// use the fbo love
		if(bUseFBOSticher) {
			
		
			for(int i=0; i<nVideos; i++) {
				VP[i].idleMovie();	
				
				bool bNewFrame = false;
				for(int i=0; i<nVideos; i++) {
					if(VP[i].isFrameNew()) bNewFrame = true;
				}
				if(!bNewFrame) return;
				
			}
		}
		
		// just work with one video
		else {
			VP[0].idleMovie();	
			pixCam 	= VP[0].getPixels();			
			if(!VP[0].isFrameNew()) return;	
			
		}
		
	}
	
	
	
	
	///////////////////////////////////////////////////////////
	// Part 2a - draw the fbo and seam them together
	///////////////////////////////////////////////////////////
	if(bUseFBOSticher) {
		// -------------- draw the camera image
		ofSetColor(0xffffff);
		stichManger.startOffscreenDraw();
		ofSetColor(0xffffff);
		for(int i=0; i<nVideos; i++) {			
			ofSetColor(0xffffff);
			if(bUsingVideoGrabber) VG[i].draw(i*inputW, 0);
			else VP[i].draw(i*inputW, 0);

		}
		stichManger.endOffscreenDraw();
		
		// -------------- draw the sceamed image
		glPushMatrix();
		glTranslatef(0, 0, 0);
		ofSetColor(0xffffff);
		for(int i=0; i<nVideos; i++) {
			stichManger.drawScreen(i);
		}
		// opencv grabbing
		dualImage.grabScreen(0, 0, width, height);
		glPopMatrix();
		
		
		
		
		bool bSaveStichCoords = panel.getValueB("STICH_COORDS");	
		if(bSaveStichCoords) {
			panel.setValueB("STICH_COORDS", false);	
			stichManger.saveToXml();
			printf("saved stich coords\n");
		}
	}
	
	///////////////////////////////////////////////////////////
	// Part 2 - load into openCV and make greyscale
	///////////////////////////////////////////////////////////
	
	/*
	 // this is a hack for this video. 
	 // masking (live) would be helpful !! (draw out regions we dont' care about).
	 for (int i = 0; i < width; i++){
	 for (int j = 0; j < height/2; j++){
	 pixCam[(j*width + i) * 3    ] = 255;
	 pixCam[(j*width + i) * 3 + 1] = 255;
	 pixCam[(j*width + i) * 3 + 2] = 255;
	 }
	 }
	 */
	
	
	if(bUseFBOSticher) {
		VideoFrame.setFromPixels(dualImage.getPixels(), width, height);
	}
	else {
		VideoFrame.setFromPixels(pixCam, width, height); 
	}
	
	GreyFrame.setFromColorImage(VideoFrame);
	
	
	bool bAnyDifferent = false;
	for (int i = 0; i < 4; i++){
		if (warpFrom[i].x != warpTo[i].x || warpFrom[i].y != warpTo[i].y){
			bAnyDifferent = true;
		}
	}
	
	// i think that we only need a warper for one video
	if(!bUseFBOSticher) {
		if (bAnyDifferent)	GreyFramePostWarp.warpIntoMe(GreyFrame, warpFrom, warpTo);
	}
		
	else GreyFramePostWarp = GreyFrame;
	
	float scale = panel.getValueF("CV_MANAGER_SCALE");
	if (fabs(scale - 1) > 0.05){
		//GreyFramePostWarp.translate(-GreyFramePostWarp.width/2, -GreyFramePostWarp.height/2);
		GreyFramePostWarp.scale(scale, scale);
		if (scale > 1) ; //GreyFramePostWarp.translate(-GreyFramePostWarp.width*(1-scale)*0.5, -GreyFramePostWarp.height*(1-scale)*0.5);
		else GreyFramePostWarp.translate(GreyFramePostWarp.width*(1-scale)*0.5, GreyFramePostWarp.height*(1-scale)*0.5);
		//GreyFramePostWarp.translate(GreyFramePostWarp.width/2, GreyFramePostWarp.height/2);
	}
	//else GreyFramePostWarp = GreyFrame;
	
	
	bool bFlipHoriz = panel.getValueB("CV_MANAGER_PANEL_VIDEO_FLIP_HORIZ");
	bool bFlipVert = panel.getValueB("CV_MANAGER_PANEL_VIDEO_FLIP_VERT");
	bool bInvert	= panel.getValueB("CV_MANAGER_PANEL_VIDEO_INVERT");
	GreyFramePostWarp.mirror(bFlipVert, bFlipHoriz);
	if (bInvert){
		GreyFramePostWarp.invert();
	}
	
	
	///////////////////////////////////////////////////////////
	// Part 3 - do thresholding
	///////////////////////////////////////////////////////////
	PresenceFrame = GreyFramePostWarp;	
	
	int threshold = panel.getValueI("CV_MANAGER_PANEL_VIDEO_THRESHOLD");
	//panel.addSlider("thrshold",  "CV_MANAGER_PANEL_VIDEO_THRESHOLD", 80, 0, 255, true);
	
	PresenceFrame.threshold(threshold);
	PresenceFrame.invert();
	PresenceFrameDialate = PresenceFrame;
	//PresenceFrameDialate.dilate_3x3();
	//PresenceFrameDialate.erode_3x3();
	
	int nPixels = PresenceFrame.width * PresenceFrame.height;
	int minBlobSize = panel.getValueF("CV_MANAGER_PANEL_MIN_BLOB") * nPixels;
	int maxBlobSize = panel.getValueF("CV_MANAGER_PANEL_MAX_BLOB") * nPixels;
	
	Contour.findContours(PresenceFrameDialate, minBlobSize, maxBlobSize, 50, true, true);  
	
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
	
	
	// ok, figure out if we have more clients then readObjects, allocate more (since people might disconnect and reconnect they get new IDs, etc)
	
	if (partialReadObjects.size() != TCPServer.getNumClients()){
		
		if (partialReadObjects.size() < TCPServer.getNumClients()){
			int nToMake = TCPServer.getNumClients() - partialReadObjects.size() ;
			for (int i = 0; i < nToMake; i++){
				partialReadObjects.push_back(new partialReadObject());
				partialReadObjects[partialReadObjects.size()-1]->nBytesRead = 0;
				memset((char *) &(partialReadObjects[partialReadObjects.size()-1]->packet), 0, sizeof(computerVisionPacket));
			}
		}
	}
	
	if (partialReadObjects.size() != TCPServer.getNumClients()){
		printf("error in recv -- some size issues with readers vs. clients");
	}	
	
	//for each client lets send them a message letting them know what port they are connected on
	/*
	 // helpful ????
	 for(int i = 0; i < TCPServer.getNumClients(); i++){
	 TCPServer.send(i, "hello client - you are connected on port - "+ofToString(TCPServer.getClientPort(i)) );
	 }*/
	
	for(int i = 0; i < TCPServer.getNumClients(); i++){
		
		int nToRead =  sizeof(computerVisionPacket) - partialReadObjects[i]->nBytesRead;
		int nGot = TCPServer.receiveRawBytes(i, (char *)&partialReadObjects[i]->packet, nToRead);
		
		
		
		if (nGot >= 0){
			if (nGot == nToRead){
				
				
				// NOTE -- 
				// THIS IS WHERE WE WOULD BREAK INTO TWO PACKETS (ie, L / R) FOR THE TWO PACKET RECV SCENES
				// ie: 
				
				// int whichCVdata = partialReadObjects[i].myId
				// if (whichCVdata == 2){
				// memcpy to right
				//} else {
				// memcpy to left...
				//}
				partialReadObjects[i]->nBytesRead += nGot;
				//printf( "copying w/ %i \n", partialReadObjects[i]->nBytesRead);
				//printf( "copying frameNumber %i \n", partialReadObjects[i]->packet.frameNumber);
				
				// TODO: checksums and stuff here
				if (partialReadObjects[i]->packet.frameNumber != 0){
					memcpy( (char *)packet, (char *)&partialReadObjects[i]->packet, sizeof(computerVisionPacket));
				}
				// reset the reader! 
				memset((char *) &(partialReadObjects[partialReadObjects.size()-1]->packet), 0, sizeof(computerVisionPacket));
				partialReadObjects[i]->nBytesRead = 0;
				
			} else {
				printf("partial read \n");
				//cout << partialReadObjects[i]->packet.frameNumber << endl;
				//partialReadObjects[i]->nBytesRead += nGot;
			}
		} else {
			//printf("trouble with client %i \n", i);
		}
		//int nGot = TCPServer.receiveRawBytes(i, (char *)packet, sizeof(computerVisionPacket));
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
void cvManager::draw() {
	
	ofSetColor(255, 255, 255);
	
	
	if(panel.getSelectedPanel() == 0) {
		
		
		if(bUseFBOSticher) {
			
			for(int i=0; i<nVideos; i++) {
				
				ofSetColor(0xffffff);
				VG[i].draw(i*inputW, 0);
				ofSetColor(255, 0, 255);
				ofDrawBitmapString("input "+ofToString(i), (i*inputW)+10, inputH+20);
				
			}
			
			ofSetColor(0xffffff);
			dualImage.draw(0, height+20);
			stichManger.drawInputDiagnostically(stichGui.x, 
												stichGui.y, 
												stichGui.width, 
												stichGui.height);
			ofSetColor(0xffffff);
			VideoFrame.draw(0, (height*2) + 30);
			
		}
		
		else {
			
			
			// just working with one video
			ofSetColor(0xffffff);
			PresenceFrame.draw(0, 50);
			Contour.draw(0, 50);
			
		}
		
	}
	
	
	
	
	
	
	if (panel.getSelectedPanel() == 2){
		
		float scalef = 768.0f / (VideoFrame.height * 2 + 50);
		
		glPushMatrix();
		glScalef(scalef, scalef, 1.0);
		VideoFrame.draw(0,0);
		PresenceFrame.draw(0,VideoFrame.height+50);
		Contour.draw(0,VideoFrame.height+50);
		
		if (panel.getSelectedPanel() == 2){
			
			ofSetColor(255, 0, 0);
			ofBeginShape();
			ofNoFill();
			for (int i = 0; i < 4; i++){
				ofVertex(warpFrom[i].x, warpFrom[i].y);
			}
			ofEndShape(true);
		}
		
		glPopMatrix();
	}
	
	if (panel.getSelectedPanel() == 1){
		
		
		float scalef = 768.0f / (VideoFrame.height * 2 + 50);
		
		glPushMatrix();
		glScalef(scalef, scalef, 1.0);
		PresenceFrame.draw(0,0);
		Contour.draw(0,0);
		
		glPushMatrix();
		glTranslatef(0,VideoFrame.height,0) ;
		
		ofFill();
		ofSetColor(0,0,0);
		ofRect(0,0,VideoFrame.width, VideoFrame.height);
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
		
		
		glPopMatrix();
		
	}
	
	/*
	 // draw the packet, to see if it's OK
	 
	 */
	
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


