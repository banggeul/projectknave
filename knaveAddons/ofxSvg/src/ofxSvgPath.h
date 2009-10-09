#pragma once

#include "ofxColor.h"
#include "ofxSvgCommand.h"
#include "ofxSvgPathContext.h"

class ofxSvgPath {
public:
	float opacity, strokeWidth;
	bool hasFill, hasStroke;
	ofxColor fillColor, strokeColor;
	string fill, stroke;
	vector<ofxSvgCommand*> commands;
	int size() const {
		return commands.size();
	}
	ofxSvgPath() {
	}
	ofxSvgPath(const ofxSvgPath& path) {
		commands = path.commands;
		opacity = path.opacity;
		strokeWidth = path.strokeWidth;
		fill = path.fill;
		stroke = path.stroke;
		hasFill = path.hasFill;
		hasStroke = path.hasStroke;
		fillColor = path.fillColor;
		strokeColor = path.strokeColor;
	}
	void setOpacity(float opacity) {
		this->opacity = opacity;
	}
	void setStrokeWidth(float strokeWidth) {
		this->strokeWidth = strokeWidth;
	}
	void setFill(ofxColor fill) {
		fillColor = fill;
	}
	void setFill(string fill) {
		this->fill = fill;
		hasFill = fill != "none";
		if(hasFill) {
			fillColor = fill;
			fillColor.a = opacity * 255;
		}
	}
	void setStroke(ofxColor stroke) {
		strokeColor = stroke;
	}
	void setStroke(string stroke) {
		this->stroke = stroke;
		hasStroke = stroke != "none";
		if(hasStroke) {
			strokeColor = stroke;
			strokeColor.a = opacity * 255;
		}
	}
	void drawShape() const {
		ofxSvgPathContext context;
		ofBeginShape();
		for(int i = 0; i < commands.size(); i++) {
			commands[i]->draw(context);
			context.lastCommand = commands[i];
		}
		ofEndShape();
	}
	void draw() const {
		ofPushStyle();
		if(opacity != 1)
			ofEnableAlphaBlending();
		if(hasFill) {
			ofFill();
			ofSetColor(fillColor);
			drawShape();
		}
		if(hasStroke) {
			glLineWidth(strokeWidth);
			ofNoFill();
			ofSetColor(strokeColor);
			drawShape();
		}
		if(opacity != 1)
			ofDisableAlphaBlending();
		ofPopStyle();
	}
	void add(ofxSvgCommand* command) {
		commands.push_back(command);
	}
	friend ostream& operator<<(ostream &out, const ofxSvgPath& path) {
		for(int i = 0; i < path.size(); i++)
			out << "(" << *(path.commands[i]) << ")";
		return out;
	}
};
