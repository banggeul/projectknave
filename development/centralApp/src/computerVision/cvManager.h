#ifndef _CV_MANAGER_H
#define _CV_MANAGER_H



#include "ofMain.h"
#include "ofxCvMain.h"

#include "cvStrokeResampler.h"
#include "cvPacket.h"
#include "ofxNetwork.h"

#include "ofxXmlSettings.h"

#include "ofxControlPanel.h"

//#include "udpChunker.h"	// good bye!

// fbo stricher
#include "stricherRenderManager.h"



enum {
	CV_SENDER, CV_RECEIVER
};



// this is for partial reads, take care of stuff, PER CLIENT (stored in a vector);
typedef struct {
	computerVisionPacket packet;
	int nBytesRead;
} partialReadObject;			


class cvManager {
	
	public:
	
	
	cvManager();
	~cvManager();
		
	//changing cameras or switching from/to the camera mode
	//requires the app to be restarted - mabe we can change this?
	void setupCamera(int which, int deviceNumber, int width, int height);
	void setupVideo(string videoPath);
	void setupCV();
	
	void setFBOStich(int fboW, int fboH);
	
	void setupNonCV();
	void setupGUI();
	void updateGUI();
	
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	
	ofxControlPanel panel;

	
	bool isFrameNew();
	
	//do all our openCV allocation
	//---------------------------		
		
	//good for adjusting the color balance, brightness etc
	void openCameraSettings(int which=0);
	
	void update();
	
	void draw();

	//-------------
	//-------------	
	ofVideoPlayer  	VP[2];
	ofVideoGrabber 	VG[2];
	
	
	ofPoint		warpFrom[4];
	ofPoint		warpTo[4];
	
	//int					threshold;
	//int					maxBlobSize, minBlobSize;
	
	bool					bUsingVideoGrabber;
	bool					bSetup;	
<<<<<<< .mine
	int						width, height;
	int						inputW, inputH;
=======
	int						width, height;
>>>>>>> .r139
	
	ofxCvColorImage			VideoFrame;
	ofxCvGrayscaleImage		GreyFrame;
	ofxCvGrayscaleImage		GreyFramePostWarp;
	ofxCvGrayscaleImage		PresenceFrame;
	ofxCvGrayscaleImage		PresenceFrameDialate;
	ofxCvContourFinder		Contour;
	
	//-----------------------------------
	//		* fbo stiching * 
	stricherRenderManager	stichManger;
	ofRectangle				stichGui;
	ofImage					dualImage;
	
	//-----------------------------------
	//		* packet * 
	
	void					fillPacket();
	cvStrokeResampler		SR;
	computerVisionPacket	*	packet;
	
	
	//-----------------------------------
	//		* settings * 
	
	ofxXmlSettings			XML;
	int						id;		 // sender?  receiver?
	int						sceneId;
	bool					bLive;
	int						nVideos;
	int						videoSource[2];
	string					videoFile[2];
	
	
	
	//-----------------------------------
	//		* network * 
	
	
	void					setupReceiver();
	void					receiveFromNetwork();
	
	void					setupSender();
	void					sendToNetwork();
	
	bool					bNetworkSetup;
	ofxUDPManager			manager;
	string					IP;
	unsigned short			port;
	unsigned char *			packetData;
	
	ofxTCPServer					TCPServer;
	ofxTCPClient					tcpClient;
	vector < partialReadObject * >	partialReadObjects;
	
	
	
	
	
	
	int connectTime;
	int deltaTime;
	bool weConnected;
	
	
	
	
	
	

};



#endif