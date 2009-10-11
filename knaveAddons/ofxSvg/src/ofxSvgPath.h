#pragma once

#include "ofxColor.h"

#include "ofxSvgCommandHandle.h"

class ofxSvgPath : public ofxSvgDrawable {
private:
	float opacity, strokeWidth;
	bool hasFill, hasStroke;
	ofxColor fillColor, strokeColor;
	string fill, stroke;
	vector<ofxSvgCommandHandle> commandHandles;
public:
	ofxSvgPath(ofxXmlSettings& xml, int i) {
		setFill(xml.getAttribute("path", "fill", "none", i));
		setStroke(xml.getAttribute("path", "stroke", "none", i));
		setStrokeWidth(xml.getAttribute("path", "stroke-width", 1., i));
		setOpacity(xml.getAttribute("path", "opacity", 1., i));

		string d = xml.getAttribute("path", "d", "", i);

		istringstream handleData(d);
		while(!handleData.eof()) {
			char name;
			handleData.get(name);
			ofxSvgCommandHandle command(name, handleData);
			commandHandles.push_back(command);
		}
	}
	ofxSvgPath(const ofxSvgPath& path) {
		opacity = path.opacity;
		strokeWidth = path.strokeWidth;
		hasFill = path.hasFill;
		hasStroke = path.hasStroke;
		fillColor = path.fillColor;
		strokeColor = path.strokeColor;
		fill = path.fill;
		stroke = path.stroke;
		commandHandles = path.commandHandles;
	}
	void setOpacity(float opacity) {
		this->opacity = opacity;
		fillColor.a = opacity * 255;
		strokeColor.a = opacity * 255;
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
		for(int i = 0; i < commandHandles.size(); i++)
			commandHandles[i].draw(context);
		ofEndShape();
	}
	void draw() {
		ofPushStyle();
		if(opacity != 1.)
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
		if(opacity != 1.)
			ofDisableAlphaBlending();
		ofPopStyle();
	}
};
