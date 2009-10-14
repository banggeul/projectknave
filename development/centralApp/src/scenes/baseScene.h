/*
 *  baseScene.h
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */




#pragma once;

#include "ofMain.h"
#include "cvPacket.h"
#include "FerryBuilding.h"
#include "renderConstants.h"
#include "ofxVectorMath.h"
#include "ofxBox2d.h"				// <--- beacuse most scenes are using this
#include "ofCvBlobTracker.h"		// <--- ref to the blob tracker

class baseScene {
	
public: 
	
	baseScene() {
		tracker = NULL;
	}
	
	virtual void setup(){}
	virtual void update(){}
	virtual void draw(){}
	
	virtual void drawTop(){}		// this is a drawing "on top" of the building mask, for diagnostic stuff. 
	
	// key board & mouse
	virtual void keyPressed(int key){}
	virtual void keyReleased(int key){}
	virtual void mouseMoved(int x, int y){}
	virtual void mouseDragged(int x, int y, int button){}
	virtual void mousePressed(int x, int y, int button){}
	virtual void mouseReleased(int x, int y, int button){}

	// blob tracking
	virtual void blobOn( int x, int y, int bid, int order ) {}
    virtual void blobMoved( int x, int y, int bid, int order ) {};    
    virtual void blobOff( int x, int y, int bid, int order ) {};
    
	// get point in FBO
	ofPoint getPointInPreview(float x, float y, bool bCap=true){
		
		float ratio  = (float)OFFSCREEN_HEIGHT / (float)OFFSCREEN_WIDTH;
		float width  = ofGetWidth();
		float height = ratio * width;
		float diff = ofGetHeight() - height;
		if (diff < 0) diff = 0;
		//FBO.draw(0,diff,width, height);
		
		ofRectangle rect;
		rect.x = 0;
		rect.y = diff;
		rect.width = width;
		rect.height = height;
		
		float pctx = (x - rect.x) / rect.width;
		float pcty = (y - rect.y) / rect.height;
		
		if (bCap == true){
			pctx = ofClamp(pctx, 0,1);
			pcty = ofClamp(pcty, 0, 1);
		}
		
		ofPoint pos;
		pos.x = pctx * OFFSCREEN_WIDTH;
		pos.y = pcty * OFFSCREEN_HEIGHT;
		return pos;
	}

	
	// this is kind of ghetto
	// but for now, every scene will have the packet data for doing 
	// stuff with.
	// later we might have an object sitting between the raw data and
	// the scenes, smoothing or post processing
	
	ofCvBlobTracker *			tracker;
	
	
	computerVisionPacket		packet;	
	FerryBuilding				building;

};