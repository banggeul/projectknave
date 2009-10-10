#pragma once

#include "ofxSvgCommand.h"

class ofxSvgVerticalLineTo : public ofxSvgCommand {
public:
	float y;
	bool relative;
	ofxSvgVerticalLineTo(float y, bool relative = true) {
		this->y = y;
		this->relative = relative;
	}
	void draw(ofxSvgPathContext& context) const {
		ofxPoint2f& lastPoint = context.lastPoint;
		if(relative) {
			ofVertex(lastPoint.x, lastPoint.y + y);
			lastPoint.y += y;
		} else {
			ofVertex(lastPoint.x, y);
			lastPoint.y = y;
		}
	}
	ostream& put(ostream &out) const {
		if(relative)
			out << "v";
		else
			out << "V";
		out << y;
		return out;
	}
};
