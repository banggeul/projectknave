#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"


#define MAX_TREE_PNTS 10


class TreeBranch {
	
public:
	
	int pid;
	
	// ref of main tree branches
	vector <TreeBranch> children;
	
	bool	spawn;
	
	// 
	float	 growInc, growSpeed;
	int		 cnt;
	
	float		angle;
	ofxVec2f	root;
	ofxVec2f	* pnts;
	int			length;
	float		girth;
	
	float		life;
	float		theta;
	ofxVec2f	dir;
	ofxVec2f	pos, vel, des;
	
	// ------------------------------------------------------ init
	void initBranch(float x, float y, int numPnts) {
		
		spawn			= false;
		life			= 10.0;
		
		//	refBranches		= NULL;
		girth			= 10.0;
		length			= numPnts;
		theta			= ofRandomf();
		angle			= 0;
		
		pnts			= new ofxVec2f[length];
		
		// tree point zero is the root
		growInc			= 0;
		
		
		cnt				= 1;	// zero is the root start at 1
		root.x			= x;
		root.y			= y;
		pos				= root;
		des				= root;
		
		setAngle(0, 1);
		
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
		
		if(cnt < length) {
			
			
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
				
				cnt ++;
				if(cnt > length) cnt = length;
				
				life *= 0.66;
				
				if(pid < 2 && cnt > 3) { 						
					spawn = true;
					children.push_back(TreeBranch());
					children.back().pid = pid + 1;
					children.back().initBranch(pos.x, pos.y, life);
					children.back().setAngle(ofRandom(-75.0, 75.0), (int)ofRandom(0, 1));
					
					printf("-- add branch[%f] --\n", life);
					
				}
			}
			
			
			
			// springy growing
			vel = (vel * 0.8) + (des - pos) / 8.0;
			pos += vel;
			
			pnts[cnt]	  = pos;//pnts[cnt-1];
			
		}
		else {
			dir = 0;
		}
		
		
		
		// update all the children
		for(int i=0; i<children.size(); i++) {
			children[i].update();	
		}
		
	}
	
	// ------------------------------------------------------ 
	void draw() {
		
	//	return;

		
		// update all the children
		for(int i=0; i<children.size(); i++) {
			children[i].draw();	
		}
		
		
		
		
		int count = cnt+1;
		if(count > length) count = length;
		
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
			ofSetColor(255, 255, 0);
			glBegin(GL_QUAD_STRIP);
			for(int i=1; i<count; i++) {
				
				pos1 = pnts[i];
				pos2 = pnts[i-1];
				perp.set(pos1 - pos2);
				perp.perpendicular(scr);
				
				float n			= ofMap((float)i, 1.0, (float)count, 0.0, 1.0);
				float thickness = girth - (n * girth/1.6);
				float offx		= (perp.x * thickness);
				float offy		= perp.y * thickness;
				
				glVertex3f(pos1.x - offx, pos1.y - offy, 0);
				glVertex3f(pos1.x + offx, pos1.y + offy, 0);
				
			}
			glEnd();
			
		}
		
		
		// nodes
		ofFill();
		ofSetColor(4, 4, 4);
		for(int i=0; i<cnt; i++) {
			ofCircle(pnts[i].x, pnts[i].y, 2);
		}
		
		
		// top node
		ofFill();
		ofSetColor(224, 24, 114);
		ofCircle(pos.x, pos.y, 5);
		
		
		ofLine(pos.x, pos.y, pos.x+dir.x, pos.y+dir.y);
		
	}		
	
};