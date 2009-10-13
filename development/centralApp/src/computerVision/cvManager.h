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



enum {
	CV_SENDER, CV_RECEIVER
};

#define CHUNK_SIZE	8000		// 8kb

typedef struct {
	long long	packetId;
	int			whichChunk;
	unsigned char data[CHUNK_SIZE];
} udpPacket;


class cvManager {
	
	public:
	
	
	cvManager();
	~cvManager();
		
	//changing cameras or switching from/to the camera mode
	//requires the app to be restarted - mabe we can change this?
	void setupCamera(int deviceNumber, int width, int height);
	void setupVideo(string videoPath);
	void setupCV();
	
	
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
	void openCameraSettings();
	
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
	int					width, height;
	
	ofxCvColorImage			VideoFrame;
	ofxCvGrayscaleImage		GreyFrame;
	ofxCvGrayscaleImage		GreyFramePostWarp;
	ofxCvGrayscaleImage		PresenceFrame;
	ofxCvGrayscaleImage		PresenceFrameDialate;
	ofxCvContourFinder		Contour;
	
	
	//-----------------------------------
	//		* packet * 
	
	void					fillPacket();
	cvStrokeResampler		SR;
	computerVisionPacket	*	packet;
	
	
	//-----------------------------------
	//		* settings * 
	
	ofxXmlSettings			XML;
	int					id;		 // sender?  receiver?
	bool					bLive;
	int					nVideos;
	int					videoSource[2];
	string				videoFile[2];
	
	
	
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
	
	ofxTCPServer TCPServer;
	ofxTCPClient tcpClient;
	
	
	
	
	
	
	int connectTime;
	int deltaTime;
	bool weConnected;
	
	
	
	
	//udpChunker				UDPC;
	//udpPacket				* UDPpacket;
	
	
	

};



#endif