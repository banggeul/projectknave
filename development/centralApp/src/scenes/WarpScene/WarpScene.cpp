#include "WarpScene.h"

void WarpScene::setup() {
	warper.setup("images/multicolor.jpg", 30, 50);
}

void WarpScene::update() {
	float attraction = -.1;
	float minDistance = 20;
	float width = packet.width;
	float height = packet.height;
	for(int i = 0; i < packet.nBlobs; i++) {
		int n = packet.nPts[i];
		for(int j = 0; j < n; j++) {
			ofPoint& cur = packet.pts[i][j];
			warper.addAttraction(cur.x, cur.y, width, height, attraction, minDistance);
		}
	}

	warper.update();
}

void WarpScene::draw() {
	warper.draw(0, 0, OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);

	float xScale = OFFSCREEN_WIDTH / packet.width;
	float yScale = OFFSCREEN_HEIGHT / packet.height;
	for(int i = 0; i < packet.nBlobs; i++) {
		int n = packet.nPts[i];
		ofNoFill();
		ofSetColor(255, 255, 255);
		glLineWidth(10.f * OFFSCREEN_SCALE);
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
}


void WarpScene::keyPressed(int key) {
}

void WarpScene::mouseDragged(int wx, int wy, int x, int y, int button) {
}

void WarpScene::mousePressed(int wx, int wy, int x, int y, int button) {
}

void WarpScene::mouseReleased(int wx, int wy, int x, int y, int button) {
}

