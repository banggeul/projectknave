#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"
#include "TreeLeaf.h"


#define MAX_TREE_PNTS 10


class MagicTree {
	
public:
	float				trunkGirth;
	int					totalTreePts;
	int					treePtsCount;
	
	bool				bDoneDrowing;
	bool				bMadeLeaves;
	ofImage	*			img;
	int					numLeaves;
	vector <TreeLeaf>	theLeaves;
	
	
	// blob information
	int					frameAge;
	int					id;
	bool				bDead;
	bool				bNoBlobAnymore;
	ofxVec2f			center;
	ofRectangle			rect;
	
	// tree position
	ofxVec2f			root, rootD;
	ofxVec2f			treeBase, treeBaseD;
	
	
	// the fade in and out
	float alpha;
	
	
	
	
	
	int pid;
	
	// ref of main tree branches
	//vector <MagicTree> children;
	
	bool	spawn;
	
	// 
	float	 growInc, growSpeed;
	//int		 cnt;
	
	float		angle;
	
	ofxVec2f	* pnts;
	//int			length;
	
	
	float		life;
	float		theta;
	ofxVec2f	dir;
	ofxVec2f	pos, vel, des;
	
	
	
	
	// ------------------------------------------------------ The Magic Tree
	MagicTree() {
		
		// the base of the trees
		treeBase	= 0;
		treeBaseD	= 0;
		
		
		trunkGirth		= 100;	
		totalTreePts	= 0;
		treePtsCount	= 0;
	
		
		// blob information
		frameAge		= 0;
		bNoBlobAnymore  = false;
		bDead			= false;
		id				= -1;
		
		
		// color & alpha
		alpha = 255;
		
	}
	
	
	// ------------------------------------------------------ take a shower
	void cleanUp() {
		
	}
	
	
	// ------------------------------------------------------ init
	void initTree(float x, float y, int numPnts) {
		
		totalTreePts = numPnts;
		treePtsCount = 1;
		
		bDoneDrowing = false;
		spawn			= false;
		life			= 10.0;
		
	
		theta			= ofRandomf();
		angle			= 0;
		
		pnts			= new ofxVec2f[totalTreePts];
		
		// tree point zero is the root
		growInc			= 0;
		
		
		//cnt				= 1;	// zero is the root start at 1
		root.x			= x;
		root.y			= y;
		rootD			= root;
		
		// the base of the tree
		treeBase.set(x, y);
		treeBaseD = treeBase;
		
		
		
		pos				= root;
		des				= root;
		
		setAngle(0, 1);
		
		// init the tree pos
		treeBaseD   = center;
		treeBaseD.y = rect.y + rect.height;
		treeBase += (treeBaseD-treeBase) / 10.0;
		
		
		
		
		printf("--- new branch[%i] ---\n", pid);
		
		
		
		
	}
	
	
	// ------------------------------------------------------ set angle to grow
	void setAngle(float theAngle, bool theDirection) { 
		
		if(theAngle > 5.0 && theAngle < 5.0) theAngle = ofRandom(-45, 45);
		
		ofxVec2f	norml;
		ofxVec2f	delta = root - ofxVec2f(root.x, root.y - 20);
		angle			  = theAngle;
		
		delta.normalize();
		delta.rotate(90+angle);
		
		// which way do we want to go
		if(theDirection)	norml.set(-delta.y, delta.x);
		else				norml.set(-delta.y, delta.x);
		
		dir = (norml * ofRandom(26.0, 50.0));
		
	}
	
	// ------------------------------------------------------
	void update() {
		
		
		// ease the tree around
		treeBaseD   = center;
		treeBaseD.y = rect.y + rect.height;
		treeBase += (treeBaseD-treeBase) / 10.0;
		
		
		
		if(bNoBlobAnymore) {
			
			alpha -= 20;
			if(alpha <= 0) {
				bDead = true;	
			}
			
			return;	
		}
		
		
		if(bDoneDrowing && !bMadeLeaves) {
			printf("--- done growing ---\n");
			numLeaves = (int)ofRandom(30, 100);
			for(int i=0; i<numLeaves; i++) {
				theLeaves.push_back(TreeLeaf());
				theLeaves.back().pos = pnts[totalTreePts-1];
				theLeaves.back().img = img;
			}
			bMadeLeaves = true;
			
		}
		
		for(int i=0; i<theLeaves.size(); i++) {
			theLeaves[i].update();
		}
		
		
		
		
		
		
		
		
		if(treePtsCount < totalTreePts-1) {
			
			
			pnts[0] = root;			// keep attached to root
			growInc += 0.092;		// simulate people moving
			
			
			theta += 0.031;
			
			if(growInc >= 1.0) {
				
				des += dir;
				des.x += cos(theta) * 4.3;
				des.y += sin(theta) * 10.03;
				growInc = 0;
				
				
			}
			
			// this needs to be here
			// we hit zero at this point
			// and need to add a new point
			if(growInc == 0) {
				treePtsCount ++;
				/*
				cnt ++;
				if(cnt > length) cnt = length;
				
				life *= 0.66;
				
				if(pid < 2 && cnt > 3) { 						
					spawn = true;
				//	children.push_back(TreeBranch());
				//	children.back().pid = pid + 1;
				//	children.back().initBranch(pos.x, pos.y, life);
				//	children.back().setAngle(ofRandom(-75.0, 75.0), (int)ofRandom(0, 1));
					
					//printf("-- add branch[%f] --\n", life);
					
				}
				 */
			}
				 
			
			
			
			// springy growing
			vel = (vel * 0.87) + (des - pos) / 8.0;
			pos += vel;
			pnts[treePtsCount]	  = pos;
			
			
			
		}
		else {
			dir = 0;
			bDoneDrowing = true;
			
		}
		
		
		
		// update all the children
	//	for(int i=0; i<children.size(); i++) {
	//		children[i].update();	
	//	}
		
	}
	
	// ------------------------------------------------------ 
	void draw() {
		
		ofEnableAlphaBlending();
		
		glPushMatrix();
		glTranslatef(treeBase.x, treeBase.y, 0);
		
		
		// some blod ID
		ofFill();
		ofSetColor(0, 0, 255, alpha);
		ofCircle(0, 0, 40);
		
		ofSetColor(255, 255, 255);
		ofDrawBitmapString(ofToString(frameAge), -10, 10);
		
		
		
		int count = treePtsCount+1;
		if(count >= totalTreePts) count = totalTreePts-1;
		
		bool bDrawLines = false;
		
		if(bDrawLines) {
			ofSetColor(44, 44, 44);
			glBegin(GL_LINES);
			for(int i=1; i<count; i++) {
				glVertex3f(pnts[i].x, pnts[i].y, 0);
				glVertex3f(pnts[i-1].x, pnts[i-1].y, 0);
			}
			glEnd();
		}
		else {
			
			ofxVec3f pos1, pos2;
			ofxVec3f perp;
			ofxVec3f scr;
			scr.set(0, 0, 1);
			
			// draw the trunk
			glBegin(GL_QUAD_STRIP);
			for(int i=1; i<count; i++) {
				
				pos1 = pnts[i];
				pos2 = pnts[i-1];
				perp.set(pos1 - pos2);
				perp.perpendicular(scr);
				
				float n			= ofMap((float)i, 1.0, (float)count, 0.0, 1.0);
				float thickness = trunkGirth - (n * trunkGirth/1.6);
				float offx		= (perp.x * thickness);
				float offy		= perp.y * thickness;
				
				glVertex3f(pos1.x - offx, pos1.y - offy, 0);
				glVertex3f(pos1.x + offx, pos1.y + offy, 0);
				
			}
			glEnd();
			
		}
		
		glPopMatrix();
		
		
		
		
		
		
		
		
		
		
		
		for(int i=0; i<theLeaves.size(); i++) {
			theLeaves[i].draw();
		}
		
		
		
		// nodes
		ofFill();
		ofSetColor(4, 4, 4);
		for(int i=0; i<treePtsCount; i++) {
			ofCircle(pnts[i].x, pnts[i].y, 2);
		}
		
		
		// top node
		ofFill();
		ofSetColor(224, 24, 114);
		ofCircle(pos.x, pos.y, 5);
		
		
		ofLine(pos.x, pos.y, pos.x+dir.x, pos.y+dir.y);
		
		
		
	}		
	
};