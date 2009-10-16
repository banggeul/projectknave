#pragma once
#include "ofMain.h"
#include "TreeBranch.h"


class MagicTree {
	
public:
	
	bool					bAddBranch;
	bool					bAlive;
	
	ofxVec2f				root;
	TreeBranch				trunk;
	vector <TreeBranch>		branches;
	
	float					life;
	
	// -------------------------------------------
	MagicTree() {
		bAddBranch  = false;
		bAlive		= false;
		
		
	}
	
	// -------------------------------------------
	void initTree(float x, float y) {
		
		root.set(x, y);
		trunk.initBranch(x, y, 15);
		trunk.life = 15.0;
		trunk.girth = ofRandom(20.0, 50.0);
		trunk.pid = 0;
		
		
		bAlive = true;
	}
	// -------------------------------------------
	void update() {
		
		if(bAlive) {
			
			trunk.root = root;
			trunk.update();
			
			if (trunk.cnt == 4 && bAddBranch == false) {
				
				//printf("--- adding a branch ---\n");
				bAddBranch = true;
				ofxVec2f   bp = trunk.pos;
				
				int	len = (int)ofRandom(5, 10);
				/*
				branches.push_back(TreeBranch());
				//branches.back().refBranches = &branches;
				branches.back().initBranch(bp.x, bp.y, len);
				branches.back().setAngle(ofRandom(-45.0, 45.0), (int)ofRandom(0, 1));
				branches.back().girth = trunk.girth/3.9;// ofRandom(0.0, 50.0);
				*/
				
				//printf("add a branch: x:%f y:%f\n", bp.x, bp.y);
			}
			
				
		}
	}
	
	// -------------------------------------------
	void draw() {
		if(bAlive) {
			
			trunk.draw();
			
			
			// draw the root
			ofSetColor(255, 0, 200);
			ofFill();
			ofCircle(root.x, root.y, 4);
		}
	}
	
};