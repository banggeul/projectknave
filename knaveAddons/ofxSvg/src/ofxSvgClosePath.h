#pragma once

#include "ofxSvgCommand.h"

class ofxSvgClosePath : public ofxSvgCommand {
public:
	bool relative;
	ofxSvgClosePath(bool relative = true) {
		this->relative = relative;
	}
	void draw(ofxSvgPathContext& context) const {
		ofxPoint2f& firstPoint = context.firstPoint;
		ofVertex(firstPoint.x, firstPoint.y);
		context.lastPoint = context.firstPoint;
	}
	ostream& put(ostream &out) const {
		if(relative)
			out << "z";
		else
			out << "Z";
		return out;
	}
};
