#pragma once

#include "ofMain.h"
#include "baseScene.h"
#include "ImageWarper.h"
#include "ofxControlPanel.h"

class WarpScene : public baseScene {
public:
	ImageWarper warper;
	ofxControlPanel panel;

	void setup();
	void update();
	void draw();
	void drawTop();

	void keyPressed(int key);
	void mouseDragged(int wx, int wy, int x, int y, int button);
	void mousePressed(int wx, int wy, int x, int y, int button);
	void mouseReleased(int wx, int wy, int x, int y, int button);
};
