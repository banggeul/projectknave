/*
 *  PaintScene.h
 *  openFrameworks
 *
 *  Created by Joel Gethin Lewis on 08/10/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "baseScene.h"
#include "PaintBall.h"
#include "ofxContourAnalysis.h"
#include "SimplePaint.h"

#include "ofxMSAFluid.h"
#include "ParticleSystem.h"
#include "msaColor.h"


#define NUM_GROUND_PNTS 20


#include "paintObject.h"
#include "ofxCvGrayscaleAdvanced.h"




class PaintScene : public baseScene {// , public b2ContactListener {
	
public: 
	
	// ---------------------- calls 
	void setup();
	void update();
	void draw();
	
	// ---------------------- mouse & keyboard
	void mousePressed(int wx, int wy, int x, int y, int button);
	void keyPressed(int key);
	
	
	void trackBlobs();
	vector < paintObject > POBJ;
	
	void drawPaintObjectIntoFluid(paintObject & PO, int whichBlob);
	
	
	int	handImgW, handImgH;
	ofxCvGrayscaleAdvanced				handImage;
	ofxCvGrayscaleAdvanced				handImageTemporallyBlurred;
	ofxCvGrayscaleAdvanced				handImageTemporallyBlurredInvert;
	int lastFrameRendered;
	
	
	int					fluidCellsX;
	bool				resizeFluid;
	bool				drawFluid;
	bool				drawParticles;
	bool				renderUsingVA;
	
	void addToFluid(float x, float y, float dx, float dy, bool addColor = true, bool addForce = true);

	// cache these for slightly better performance
	struct {
		int				width;
		int				height;
		float			invWidth;
		float			invHeight;
		float			aspectRatio;
		float			aspectRatio2;
	} window;
	
	
	
	ofxMSAFluidSolver	fluidSolver;
	ofxMSAFluidDrawer	fluidDrawer;	
	
	ParticleSystem		particleSystem;
	
};






