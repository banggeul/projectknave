


#pragma once;

#include "ofMain.h"
#include "renderConstants.h"
#include "ofxFBOTexture.h"


class renderManager {
	
	public :
	
	void setup();
	void update();
	
	void swapInFBO();
	void swapOutFBO();
	void drawForPreview();
	
	ofxFBOTexture  FBO;
	
	
	
};