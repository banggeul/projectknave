#include "WarpScene.h"

void WarpScene::setup() {
	warper.setup("images/multicolor.jpg", 30, 50);

	panel.setup("Warp Scene", 700, 10, 300, 750);

	panel.addPanel("Settings", 1, false);
	panel.setWhichPanel("Settings");

	panel.addSlider("Attraction", "attraction", 0, -.3, .3, false);
	panel.addSlider("Min Distance", "minDistance", 20, 10, 50, false);
	panel.addSlider("Skip Amount", "skipAmount", 1, 1, 10, true);

	panel.loadSettings("settings/panels_xml/warpScenePanel.xml");
}

void WarpScene::update() {
	warper.update();

	float skipAmount = panel.getValueI("skipAmount");
	float attraction = panel.getValueF("attraction");
	float minDistance = panel.getValueF("minDistance");
	float xScale = (float) OFFSCREEN_WIDTH / (float) packet.width;
	float yScale = (float) OFFSCREEN_HEIGHT / (float) packet.height;
	for(int i = 0; i < packet.nBlobs; i++) {
		int n = packet.nPts[i];
		for(int j = 0; j < n; j += skipAmount) {
			ofPoint& cur = packet.pts[i][j];
			warper.addAttraction(cur.x * xScale, cur.y * yScale, attraction, minDistance);
		}
	}

	panel.update();
}

void WarpScene::draw() {
	warper.draw(0, 0, OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);

	float xScale = (float) OFFSCREEN_WIDTH / (float) packet.width;
	float yScale = (float) OFFSCREEN_HEIGHT / (float) packet.height;
	ofNoFill();
	ofSetColor(0, 0, 0);
	glLineWidth(8.f * OFFSCREEN_SCALE);
	for(int i = 0; i < packet.nBlobs; i++) {
		int n = packet.nPts[i];
		ofBeginShape();
		for(int j = 0; j < n; j++) {
			ofPoint& cur = packet.pts[i][j];
			ofVertex(
				cur.x * xScale,
				cur.y * yScale);
		}
		ofEndShape();
	}
}

void WarpScene::drawTop() {
	panel.draw();
}


void WarpScene::keyPressed(int key) {
}

void WarpScene::mouseDragged(int wx, int wy, int x, int y, int button) {
	panel.mouseDragged(wx, wy, button);
}

void WarpScene::mousePressed(int wx, int wy, int x, int y, int button) {
	panel.mousePressed(wx, wy, button);
}

void WarpScene::mouseReleased(int wx, int wy, int x, int y, int button) {
	panel.mouseReleased();
}

