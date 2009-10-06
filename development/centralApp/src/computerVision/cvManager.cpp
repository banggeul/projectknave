#include "cvManager.h"

cvManager::cvManager(){
	bSetup				= false;
	
	threshold			= 80;
	maxBlobSize			= 320*240*3;
	minBlobSize			= 100;
	
	int						threshold;
	int						maxBlobSize, minBlobSize;
}

//changing cameras or switching from/to the camera mode
//requires the app to be restarted - mabe we can change this?
//---------------------------		
void cvManager::setupCamera(int deviceNumber, int _width, int _height){
	VG.setVerbose(true);
	//VG.setDeviceID(deviceNumber);
	

	VG.setDeviceID(deviceNumber);
	VG.initGrabber(_width, _height);
	width = VG.width;
	height = VG.height;	
	
	bSetup = true;
	bUsingVideoGrabber = true;

	
	setupCV();
}
		
//same as above - from/to using test movies
//requires a restart
//---------------------------		
void cvManager::setupVideo(string videoPath){
	
	VP.loadMovie(videoPath);
	VP.play(); 
	VP.setPosition(0.0);
	VP.setSpeed(0.90);
	VP.setUseTexture(true);
	width = VP.width;
	height = VP.height;
	bSetup = true;
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
void cvManager::processFrame(){


	
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
	
	
	
	VideoFrame.setFromPixels(pixCam,640,480);
	
	//VideoFrame.setFromPixels(pixCam, W, H);
	GreyFrame.setFromColorImage(VideoFrame);
	
	///////////////////////////////////////////////////////////
	// Part 3 - do thresholding
	///////////////////////////////////////////////////////////
	PresenceFrame = GreyFrame;	
	PresenceFrame.threshold(threshold);
	PresenceFrameDialate = PresenceFrame;
	PresenceFrameDialate.dilate_3x3();
	PresenceFrameDialate.erode_3x3();
	
	
	Contour.findContours(PresenceFrameDialate, minBlobSize, maxBlobSize, 50, false, false);  
		
}
		
//---------------------------				
void cvManager::draw(float x, float y, int w, int h){
	ofSetColor(255, 255, 255);
	VideoFrame.draw(x, y, w, h);
}



