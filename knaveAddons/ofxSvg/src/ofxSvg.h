#pragma once

#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"
#include "ofxSvgPath.h"
#include "ofxSvgMoveTo.h"
#include "ofxSvgCurveTo.h"
#include "ofxSvgSmoothCurveTo.h"
#include "ofxSvgLineTo.h"

inline ofPoint readPoint(istringstream& stream) {
	ofPoint point;
	stream >> point.x;
	if(stream.peek() == ',')
		stream.ignore(1);
	stream >> point.y;
	if(stream.peek() == ',')
		stream.ignore(1);
	return point;
}

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
		for(int i = 0; i < size(); i++)
			for(int j = 0; j < paths[i].size(); j++)
				delete paths[i].commands[j];
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
		for(int i = 0; i < paths; i++) {
			ofxSvgPath path;

			string fill = xml.getAttribute("path", "fill", "none", i);
			string stroke = xml.getAttribute("path", "stroke", "none", i);
			double strokeWidth = xml.getAttribute("path", "stroke-width", 1., i);
			double opacity = xml.getAttribute("path", "opacity", 1., i);

			path.setOpacity(opacity);
			path.setFill(fill);
			path.setStroke(stroke);
			path.setStrokeWidth(strokeWidth);

			string d = xml.getAttribute("path", "d", "", i);
			istringstream data(d);
			while(!data.eof()) {
				char command;
				data.get(command);
				if(command == 'M') {
					ofxSvgMoveTo* startOffset = new ofxSvgMoveTo(readPoint(data));
					path.add(startOffset);
				}
				if(command == 'c') {
					ofPoint control1 = readPoint(data);
					ofPoint control2 = readPoint(data);
					ofPoint point = readPoint(data);
					ofxSvgCurveTo* coord = new ofxSvgCurveTo(control1, control2, point);
					path.add(coord);
				}
				if(command == 's') {
					ofPoint control2 = readPoint(data);
					ofPoint point = readPoint(data);
					ofxSvgSmoothCurveTo* coord = new ofxSvgSmoothCurveTo(control2, point);
					path.add(coord);
				}
				if(command == 'l') {
					ofPoint point = readPoint(data);
					ofxSvgLineTo* coord = new ofxSvgLineTo(point);
					path.add(coord);
				}
			}
			add(path);
		}
		xml.popTag();
	}
	int size() const {
		return paths.size();
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
	void add(ofxSvgPath& path) {
		paths.push_back(path);
	}
	friend ostream& operator<<(ostream &out, const ofxSvg& svg) {
		for(int i = 0; i < svg.size(); i++)
			out << "[" << svg.paths[i] << "]";
		return out;
	}
};
