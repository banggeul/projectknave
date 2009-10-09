

#include "renderManager.h"


	
void renderManager::setup(){
	
	FBO.allocate(OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT, true);
	FBO.setClearColor(127,127,127,255);
	
}

void renderManager::update(){
	
}

void renderManager::swapInFBO(){
	
	FBO.setupScreenForMe();
	FBO.swapIn();
	
}

void renderManager::swapOutFBO(){
	
	FBO.swapOut();
	FBO.setupScreenForThem();

}

void renderManager::drawForPreview(){

	
	float width = ofGetWidth();
	float height = 0.75 * width;
	float diff = ofGetHeight() - height;
	if (diff < 0) diff = 0;
	FBO.draw(0,diff,width, height);
}