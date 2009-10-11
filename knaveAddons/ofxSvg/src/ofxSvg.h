#pragma once

#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"
#include "ofxSvgDrawable.h"
#include "ofxSvgPath.h"

class ofxSvg {
protected:
	float width, height;
	ofxPoint2f anchor;
	vector<ofxSvgDrawable*> parts;

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
		int paths = xml.getNumTags("path");
		for(int i = 0; i < paths; i++)
			add(new ofxSvgPath(xml, i));
		xml.popTag();
	}
	int size() const {
		return parts.size();
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
				parts[i]->draw();
			ofPopStyle();
			ofPopMatrix();
			glEndList();
			displayListReady = true;
		}
	}
	void update() {
		displayListReady = false;
	}
	void add(ofxSvgDrawable* part) {
		parts.push_back(part);
	}
};
