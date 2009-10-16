
#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"
#include "TreeConstants.h"
#include "ofCvTrackedBlob.h"
#include "MagicTree.h"
#include "Fern.h"
#include "RandomFern.h"

class TreePerson {
	
public:
	
	
	int id;
	ofRectangle		rect;
	ofxVec2f		pos;
	float			area;
	
	
	int				age;
	bool			bGrowATree, bFadeOut, bDead;
	
	// the tree
	int				numTreePts;
	int				ptsCount;
	ofxVec2f	   *pts;
	
	MagicTree		tree;
	
	ofPoint			colorD, color;
	float			alpha;
	
	float rate;
	
	float treeAge;
	//RandomFern fern;
	
	
	void cleanUp() {
		
	}
	
	
	
	// --------------------------------------------
	TreePerson() {
		age = 0;
		area = 1.0;
		bGrowATree = false;
		bFadeOut = false;
		bDead = false;
		treeAge = 0;
		
		rate = ofRandom(5, 10);
	}
	

	// --------------------------------------------
	void init(ofCvTrackedBlob &blob) {
		
		//randomize();
		pos = blob.centroid;
		rect = blob.boundingRect;
		id = blob.id;
		/*
		 ptsCount   = 2;
		 numTreePts = 30;
		 
		 pts = new ofxVec2f[numTreePts];
		 
		 for(int i=2; i<numTreePts; i++) {
		 pts[i] = pos;
		 }
		 */
		
		//string numI = ofToString((int)ofRandom(0, 2));
		//fern.addLeaf("sceneAssets/trees/Fern_Leaf_001.svg");
		
		color = 0;
		colorD.x = ofRandom(0, 255);
		colorD.y = ofRandom(0, 255);
		colorD.z = ofRandom(0, 255);
	}	
	
	
	// -------------------------------------------- update 
	void update() {
		
		age ++;
		
		if(age > 20 && !bGrowATree) {
			printf("--- start growing the tree [%i]---\n", id);	
			bGrowATree = true;
			tree.initTree(pos.x, rect.y + (rect.height/1.3));
		}
		
		// ok we are alive and growing
		if(bGrowATree) {
			
			treeAge += 0.3;
			
			//tree.root.x = pos.x;
			//tree.root.y = rect.y + (rect.height/1.3); 
			
		}
		
		
		color += (colorD-color) / rate;
		
		
		if(bFadeOut) {
			alpha -= 10.0;
			
		}
		
		
		if(alpha <= 0) {
			
			bDead = true;	
		}
		
		/*
		 pts[0].x = pos.x;
		 pts[0].y = rect.y + rect.height;
		 
		 pts[1] = pts[0];
		 pts[1].x += 0;//pos.x + ofRandom(-0.1, 0.1);
		 pts[1].y += - 0.03;
		 
		 pts[1] *= 0.2;
		 
		 ofxVec2f diff;
		 float len;
		 for(int i=2; i<ptsCount; i++) {
		 diff = pts[i] - pts[i-2];
		 len = diff.length();
		 pts[i] = pts[i-1] + (diff * 20) / len;
		 }
		 
		 
		 // time to grow
		 ptsCount ++;
		 if(ptsCount >= ptsCount) ptsCount = numTreePts-1;
		 
				 */
		tree.update();

	}
	
	
	// --------------------------------------------	
	void drawTheTree() {
		/*
		int drawMode = 1;
		
		if(drawMode == 0) {
			glBegin(GL_LINE_STRIP);
			for(int i=1; i<ptsCount; i++) {
				glVertex2f(pts[i].x, pts[i].y);
				glVertex2f(pts[i-1].x, pts[i-1].y);
			}
			glEnd();
		}
		
		if(drawMode == 1) {
			
			// the plant
			ofxVec3f pos1, pos2;
			ofxVec3f perp;
			ofxVec3f scr;
			scr.set(0, 0, 1);
			
			
			ofNoFill();
			ofSetColor(255, 255, 255);
			glBegin(GL_QUAD_STRIP);
			for(int i=1; i<ptsCount; i++) {
				
				pos1 = pts[i];
				pos2 = pts[i-1];
				perp.set(pos1 - pos2);
				perp.perpendicular(scr);
				
				float n = ofMap((float)i, 1.0, (float)ptsCount-1, 0.0, 1.0);
				float thick     = area * 220;
				float thickness =  thick - (n * thick);
				
				float xOff        = perp.x * thickness;
				float yOff        = perp.y * thickness;
				
				glVertex2f(pos1.x - xOff, pos1.y - yOff);
				glVertex2f(pos1.x + xOff, pos1.y + yOff);
				
			}
			glEnd();
			
		}
	*/	
	}
	
	// --------------------------------------------
	void draw() {
		
		ofNoFill();
		ofRect(rect.x, rect.y, rect.width, rect.height);
		
		tree.draw();
		
		
		ofSetColor(25, 55, 225);
		ofDrawBitmapString(ofToString(area), pos.x, pos.y);
		drawTheTree();
		
		
	}
	
	
};