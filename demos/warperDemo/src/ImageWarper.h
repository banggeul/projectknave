#pragma once

#include "ofxBox2d.h"

class ImageWarper {
protected:
	bool warpDebug;

	ofxBox2d box2d;
	vector<ofxBox2dCircle> grid;
	vector<ofxBox2dJoint> springs;

	float warpWidth, warpHeight;
	int xSize, ySize;

	ofImage img;
public:
	ImageWarper() {
		warpDebug = false;
		box2d.init();
		box2d.setGravity(0, 0);
	}
	int getWidth() {
		return (int) img.getWidth();
	}
	int getHeight() {
		return (int) img.getHeight();
	}
	void setup(string filename, int fps, float resolution) {
		img.loadImage(filename);

		box2d.setFPS(fps);

		warpWidth = img.getWidth();
		warpHeight = img.getHeight();

		xSize = (int) resolution;
		ySize = (int) (resolution * warpHeight / warpWidth);

		float radius = ((float) warpWidth / (float) xSize) * .3;
		float mass = 1;
		float bounce = 0.4; // irrelevant
		float friction = 0.1; // irrelevant

		float stiffness = ((float) warpWidth / (float) xSize) * .05;
		float damping = .1; // harder to pull around, rests faster

		for (int y = 0; y < ySize; y++) {
			for (int x = 0; x < xSize; x++) {
				grid.push_back(ofxBox2dCircle());
				bool isFixed = x == 0 || y == 0 || x == xSize - 1 || y == ySize - 1;
				grid.back().setPhysics(mass, bounce, friction);
				grid.back().disableCollistion();
				grid.back().setup(box2d.getWorld(),
					ofMap(x, 0, xSize - 1, 0, warpWidth),
					ofMap(y, 0, ySize - 1, 0, warpHeight),
					radius, isFixed);
			}
		}

		for (int y = 0; y < ySize - 1; y++) {
			for (int x = 0; x < xSize - 1; x++) {
				int i = y * xSize + x;
				ofxBox2dCircle& cur = grid[i];
				ofxBox2dCircle& right = grid[i + 1];
				ofxBox2dCircle& below = grid[i + xSize];

				if (y > 0) {
					springs.push_back(ofxBox2dJoint());
					springs.back().setWorld(box2d.getWorld());
					springs.back().addJoint(cur.body, right.body, stiffness, damping);
				}

				if (x > 0) {
					springs.push_back(ofxBox2dJoint());
					springs.back().setWorld(box2d.getWorld());
					springs.back().addJoint(cur.body, below.body, stiffness, damping);
				}
			}
		}
	}
	void addAttraction(float x, float y, float attraction, float minDistance) {
		for(int i = 0; i < grid.size(); i++)
			grid[i].addAttractionPoint(x, y, attraction, minDistance);
	}
	void update() {
		box2d.update();
	}
	void draw(float x, float y, float width, float height) {
		ofPushMatrix();
		ofTranslate(x, y);
		ofScale(width / warpWidth, height / warpHeight);

		float texScaleX = (float) img.getWidth() / (xSize - 1);
		float texScaleY = (float) img.getHeight() / (ySize - 1);

		ofSetColor(255, 255, 255);
		img.getTextureReference().bind();
		glBegin(GL_QUADS);
		for(int y = 0; y < ySize - 1; y++) {
			for(int x = 0; x < xSize - 1; x++) {
				int i = y * xSize + x;
				glTexCoord2f(x * texScaleX, y * texScaleY);
				ofPoint cur = grid[i].getPosition();
				glVertex2f(cur.x, cur.y);

				glTexCoord2f((x + 1) * texScaleX, y * texScaleY);
				ofPoint right = grid[i + 1].getPosition();
				glVertex2f(right.x, right.y);

				glTexCoord2f((x + 1) * texScaleX, (y + 1) * texScaleY);
				ofPoint bottomRight = grid[i + xSize + 1].getPosition();
				glVertex2f(bottomRight.x, bottomRight.y);

				glTexCoord2f(x * texScaleX, (y + 1) * texScaleY);
				ofPoint below = grid[i + xSize].getPosition();
				glVertex2f(below.x, below.y);
			}
		}
		glEnd();
		img.getTextureReference().unbind();

		if(warpDebug) {
			ofFill();
			ofSetColor(0, 0, 0);
			for(int y = 0; y < ySize; y++) {
				for(int x = 0; x < xSize; x++) {
					int i = y * xSize + x;
					float radius = grid[i].getRadius();
					ofPoint cur = grid[i].getPosition();
					ofCircle(cur.x, cur.y, radius);
				}
			}

			for(int i = 0; i < springs.size(); i++) {
				springs[i].draw();
			}

			ofSetWindowTitle(ofToString(ofGetFrameRate()));
		}

		ofPopMatrix();
	}
};
