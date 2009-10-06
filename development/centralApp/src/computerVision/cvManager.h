#ifndef _CV_MANAGER_H
#define _CV_MANAGER_H



#include "ofMain.h"
#include "ofxCvMain.h"


class cvManager {
	
	public:
	
	
	cvManager();
		
	//changing cameras or switching from/to the camera mode
	//requires the app to be restarted - mabe we can change this?
	void setupCamera(int deviceNumber, int width, int height);
	void setupVideo(string videoPath);
	
	bool isFrameNew();
	
	//do all our openCV allocation
	//---------------------------		
	void setupCV();
	
	//good for adjusting the color balance, brightness etc
	void openCameraSettings();

	
	
	void processFrame();
			
	void draw(float x, float y, int w = 640, int h = 480);

	//-------------
	//-------------	
	ofVideoPlayer  	VP;
	ofVideoGrabber 	VG;
	
	int						threshold;
	int						maxBlobSize, minBlobSize;
	
	bool					bUsingVideoGrabber;
	bool					bSetup;	
	int						width, height;
	
	ofxCvColorImage			VideoFrame;
	ofxCvGrayscaleImage		GreyFrame;
	ofxCvGrayscaleImage		PresenceFrame;
	ofxCvGrayscaleImage		PresenceFrameDialate;
	ofxCvContourFinder		Contour;
	
	
	

};



#endif