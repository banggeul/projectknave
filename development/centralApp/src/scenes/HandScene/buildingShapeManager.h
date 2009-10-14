/*
 *  simpleScene.h
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include <fstream.h>
#include "buildingShape.h"

class buildingShapeManager {
	
public: 
	
	
	buildingShapeManager(){
		spot.loadImage("images/spot.png");
	}
	void parseShapesFromFile(string fileName);
	
	void buildingShapeManager::draw(){
		for (int i = 0; i < shapes.size(); i++){
			shapes[i]->draw();
		}
	}
	
	void update(){
		for (int i = 0; i < shapes.size(); i++){
			shapes[i]->update();
		}
	}
	void checkShapesForPoint(float x, float y);
	
	vector < buildingShape * > shapes;
	
	ofImage spot;
	
};