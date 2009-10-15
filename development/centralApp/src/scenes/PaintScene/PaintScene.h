/*
 *  PaintScene.h
 *  openFrameworks
 *
 *  Created by Joel Gethin Lewis on 08/10/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once;

#include "ofMain.h"
#include "baseScene.h"
#include "PaintBall.h"
#include "ofxContourAnalysis.h"
#include "SimplePaint.h"


#define NUM_GROUND_PNTS 20



class PaintScene : public baseScene , public b2ContactListener {
	
public: 
	
	// ---------------------- calls 
	void setup();
	void update();
	void draw();
	
	// ---------------------- mouse & keyboard
	void mousePressed(int x, int y, int button);
	void keyPressed(int key);

	// ---------------------- ground
	ofxVec2f groundPnts[NUM_GROUND_PNTS];
	
	
	
	// ---------------------- Ferry Building
	
	FerryBuilding				ferryBuilding;
	vector <ofxBox2dLine>		box2dBuilding;
	void						createBuildingContour();

	
	// ---------------------- Packets
	
	bool						bGotMyFirstPacket;
	int							lastFrameRendered;

	// ----------------------	Contour Shape
	vector <ofPoint>			simpleContour;
	vector <ofPoint>			smoothContour;
	vector <ofxBox2dLine>		bodyShapes;
	ofxContourAnalysis			contourAnalysis;
	
	
	
	// ----------------------	Box2D
	
	ofxBox2d					box2d;
	vector <ofxBox2dLine>		buildingBoundsShape;
	vector <PaintBall>			paintBalls;
	vector <SimplePaint>		paint;
	
	
	// a bit of a hack
	void Add(const b2ContactPoint* point);
	void Remove(const b2ContactPoint* point);
	
	
};






