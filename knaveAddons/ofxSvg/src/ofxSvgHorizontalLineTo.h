#pragma once

#include "ofxSvgCommand.h"

class ofxSvgHorizontalLineTo : public ofxSvgCommand {
public:
	float x;
	bool relative;
	ofxSvgHorizontalLineTo(float x, bool relative = true) {
		this->x = x;
		this->relative = relative;
	}
	void draw(ofxSvgPathContext& context) const {
		ofxPoint2f& lastPoint = context.lastPoint;
		if(relative) {
			ofVertex(lastPoint.x + x, lastPoint.y);
			lastPoint.x += x;
		} else {
			ofVertex(x, lastPoint.y);
			lastPoint.x = x;
		}
	}
	ostream& put(ostream &out) const {
		if(relative)
			out << "h";
		else
			out << "H";
		out << x;
		return out;
	}
};
