#pragma once

#include "ofxSvgCommand.h"

class ofxSvgCurveTo : public ofxSvgCommand {
public:
	ofxPoint2f control1, control2, point;
	ofxSvgCurveTo(ofPoint control1, ofPoint control2, ofPoint point, bool relative = true) {
		this->control1.set(control1);
		this->control2.set(control2);
		this->point.set(point);
		this->relative = relative;
	}
	void draw(ofxSvgPathContext& context) const {
		ofxPoint2f& lastPoint = context.lastPoint;
		if(relative) {
			ofBezierVertex(
				lastPoint.x + control1.x, lastPoint.y + control1.y,
				lastPoint.x + control2.x, lastPoint.y + control2.y,
				lastPoint.x + point.x, lastPoint.y + point.y);
			/*
			ofBezier(
				lastPoint.x, lastPoint.y,
				lastPoint.x + control1.x, lastPoint.y + control1.y,
				lastPoint.x + control2.x, lastPoint.y + control2.y,
				lastPoint.x + point.x, lastPoint.y + point.y);*/
			lastPoint += point;
		} else {
			ofBezier(
				lastPoint.x, lastPoint.y,
				control1.x, control1.y,
				control2.x, control2.y,
				point.x, point.y);
			lastPoint.set(point);
		}
	}
	ostream& put(ostream &out) const {
		if(relative)
			out << "c";
		else
			out << "C";
		out << control1.x << "," << control1.y << ",";
		out << control2.x << "," << control2.y << ",";
		out << point.x << "," << point.y;
		return out;
	}
};
