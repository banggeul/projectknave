#pragma once
#include "ofMain.h"
#include "ofxSvg.h"


#define NUM_SVG_EYES 9


class MonsterSVGParts {
	
public:
	
	
	//sceneAssets/monsters/eyes
	vector <ofxSvg>		eyes;
	
	// ----------------------------------------- MMmmm Monster Parts
	MonsterSVGParts() {
		eyes.clear();	
		//eyes.resize(NUM_SVG_EYES);
	}
	
	
	// ----------------------------------------- load them in
	void loadAllParts() {
		printf("----------------------------------------\n");
		printf("\n--- loading all the monster parts ---\n");
		printf("----------------------------------------\n");		
		for(int i=0; i<NUM_SVG_EYES; i++) {
			
			string fileToLoad = "sceneAssets/monsters/eyes/Monster_eye_"+ofToString(i)+".svg";
			
			ofxSvg svgFile;
			if(svgFile.loadFile(fileToLoad)) {
				
				eyes.push_back(svgFile);
				printf("file:%s\n", fileToLoad.c_str());
			
			}
			else {
				printf("-- error loading [%i] eye \n", i);	
			}
		}
		
	}
	
	
	// ----------------------------------------- draw all parts	
	void drawAllParts() {
		
		ofSetColor(255, 255, 255);
		ofFill();
		ofRect(0, 0, 600, 300);
		
		for(int i=0; i<NUM_SVG_EYES; i++) {
			ofSetColor(0xffffff);
			eyes[i].draw(i*eyes[i].getWidth(), 0);	
		}
	}
	
};




