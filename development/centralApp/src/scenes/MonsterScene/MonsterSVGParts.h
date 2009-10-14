#pragma once
#include "ofMain.h"
#include "ofxSvg.h"


#define NUM_SVG_EYES 1


class MonsterSVGParts {
	
public:
	
	
	//sceneAssets/monsters/eyes
	vector <ofxSvg>		eyes;
	
	// ----------------------------------------- MMmmm Monster Parts
	MonsterSVGParts() {
		eyes.clear();	
		eyes.assign(NUM_SVG_EYES, ofxSvg());
	}
	
	
	// ----------------------------------------- load them in
	void loadAllParts() {
		printf("----------------------------------------\n");
		printf("\n--- loading all the monster parts ---\n");
		printf("----------------------------------------\n");		
		for(int i=0; i<NUM_SVG_EYES; i++) {
			string fileToLoad = "sceneAssets/monsters/eyes/Monster_eye_011.svg";

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
		for(int i=0; i<NUM_SVG_EYES; i++) {
			ofSetColor(0xffffff);
			eyes[i].draw(i*eyes[i].getWidth(), 0, 100, 100);	
		}
	}
	
};




