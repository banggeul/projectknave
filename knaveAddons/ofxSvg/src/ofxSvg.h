#pragma once

#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"
#include "ofxSvgPath.h"

class ofxSvg {
protected:
	float width, height;
	ofxPoint2f anchor;
	vector<ofxSvgPath> paths;

	GLuint displayList;
	bool displayListReady;
public:
	ofxSvg() {
		displayList = glGenLists(1);
		displayListReady = false;
	}
	~ofxSvg() {
		glDeleteLists(displayList, 1);
	}
	void loadFile(string filename) {
		ofxXmlSettings xml;
		xml.loadFile(filename);

		width = xml.getAttribute("svg", "width", 0., 0);
		height = xml.getAttribute("svg", "height", 0., 0);
		float xAnchor = xml.getAttribute("svg", "x", 0., 0);
		float yAnchor = xml.getAttribute("svg", "y", 0., 0);
		anchor.set(xAnchor, yAnchor);

		xml.pushTag("svg");
		loadPaths(xml);
		xml.popTag();
	}
	void loadPaths(ofxXmlSettings& xml) {
		int totalPaths = xml.getNumTags("path");
		for(int i = 0; i < totalPaths; i++)
			paths.push_back(ofxSvgPath(xml, i));
		int totalGroups = xml.getNumTags("g");
		for(int i = 0; i < totalGroups; i++) {
			xml.pushTag("g", i);
			loadPaths(xml);
			xml.popTag();
		}
	}
	int size() const {
		return paths.size();
	}
	ofxSvgPath& getPath(int i) {
		return paths[i];
	}
	float getWidth() {
		return width;
	}
	float getHeight() {
		return height;
	}
	void setAnchor(ofPoint& anchor) {
		this->anchor = anchor;
	}
	ofxPoint2f getAnchor() {
		return anchor;
	}
	void draw(float x, float y) {
		draw(x, y, width, height);
	}
	void draw(float x, float y, float width, float height) {
		ofPushMatrix();
		ofTranslate(x, y);
		ofScale(width / this->width, height / this->height);
		draw();
		ofPopMatrix();
	}
	void draw() {
		if(displayListReady) {
			glCallList(displayList);
		} else {
			glNewList(displayList, GL_COMPILE);
			ofPushMatrix();
			ofPushStyle();
			ofTranslate(-anchor.x, -anchor.y);
			for(int i = 0; i < size(); i++)
				paths[i].draw();
			ofPopStyle();
			ofPopMatrix();
			glEndList();
			displayListReady = true;
		}
	}
	void update() {
		displayListReady = false;
	}
};
