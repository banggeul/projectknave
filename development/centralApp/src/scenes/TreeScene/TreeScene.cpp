
#include "TreeScene.h"

// ------------------------------------------
void TreeScene::setup() {
	curStem = 0;
}

// ------------------------------------------

void TreeScene::mousePressed(int x, int y, int button) {

	curStem ++;
	curStem %= MAX_STEMS;
}

// ------------------------------------------
void TreeScene::update() {
	for(int i=0; i<MAX_STEMS; i++) {
		stems[i].update();
	}
}

// ------------------------------------------
void TreeScene::draw() {
	
	for(int i=0; i<MAX_STEMS; i++) {
		stems[i].draw();
	}	
	
	

}