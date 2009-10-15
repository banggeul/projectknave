

#include "renderManager.h"


	
void renderManager::setup(){
	
	FBO.allocate(OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT, true);
	FBO.setClearColor(40,40,40,255);
	
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

	float ratio = (float)OFFSCREEN_HEIGHT / (float)OFFSCREEN_WIDTH;
	
	float width = ofGetWidth();
	float height = ratio * width;
	float diff = ofGetHeight() - height;
	if (diff < 0) diff = 0;
	
	FBO.draw(0,diff,width, height);
}

ofPoint renderManager::getPointInPreview(float x, float y, bool bCap){
	

	
	
	float ratio = (float)OFFSCREEN_HEIGHT / (float)OFFSCREEN_WIDTH;
	
	float width = ofGetWidth();
	float height = ratio * width;
	float diff = ofGetHeight() - height;
	if (diff < 0) diff = 0;
	//FBO.draw(0,diff,width, height);
		
	ofRectangle rect;
	rect.x = 0;
	rect.y = diff;
	rect.width = width;
	rect.height = height;
	
	float pctx = (x - rect.x) / rect.width;
	float pcty = (y - rect.y) / rect.height;
	
	if (bCap == true){
		pctx = ofClamp(pctx, 0,1);
		pcty = ofClamp(pcty, 0, 1);
	}
	
	ofPoint pos;
	pos.x = pctx * OFFSCREEN_WIDTH;
	pos.y = pcty * OFFSCREEN_HEIGHT;
	return pos;
}


		