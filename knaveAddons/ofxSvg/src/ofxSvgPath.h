#pragma once

#include "ofxColor.h"
#include "ofxSvgCommand.h"
#include "ofxSvgPathContext.h"
#include "ofxSvgMoveTo.h"
#include "ofxSvgCurveTo.h"
#include "ofxSvgSmoothCurveTo.h"
#include "ofxSvgLineTo.h"
#include "ofxSvgHorizontalLineTo.h"
#include "ofxSvgVerticalLineTo.h"

inline float readFloat(istringstream& stream) {
	float x;
	stream >> x;
	if(stream.peek() == ',')
		stream.ignore(1);
	return x;
}

inline ofPoint readPoint(istringstream& stream) {
	ofPoint point;
	point.x = readFloat(stream);
	point.y = readFloat(stream);
	return point;
}

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
	ofxSvgPath(ofxXmlSettings& xml, int i) {
		setFill(xml.getAttribute("path", "fill", "none", i));
		setStroke(xml.getAttribute("path", "stroke", "none", i));
		setStrokeWidth(xml.getAttribute("path", "stroke-width", 1., i));
		setOpacity(xml.getAttribute("path", "opacity", 1., i));

		string d = xml.getAttribute("path", "d", "", i);
		istringstream data(d);
		while(!data.eof()) {
			char command;
			data.get(command);
			if(command == 'm' || command == 'M')
				add(new ofxSvgMoveTo(readPoint(data), command == 'm'));
			if(command == 'c' || command == 'C') {
				ofPoint control1 = readPoint(data);
				ofPoint control2 = readPoint(data);
				ofPoint point = readPoint(data);
				add(new ofxSvgCurveTo(control1, control2, point, command == 'c'));
			}
			if(command == 's' || command == 'S') {
				ofPoint control2 = readPoint(data);
				ofPoint point = readPoint(data);
				add(new ofxSvgSmoothCurveTo(control2, point, command == 's'));
			}
			if(command == 'l' || command == 'L')
				add(new ofxSvgLineTo(readPoint(data), command == 'l'));
			if(command == 'z' || command == 'Z')
				add(new ofxSvgClosePath());
			if(command == 'h' || command == 'H')
				add(new ofxSvgHorizontalLineTo(readFloat(data), command == 'h'));
			if(command == 'v' || command == 'V')
				add(new ofxSvgVerticalLineTo(readFloat(data), command == 'v'));
		}
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
