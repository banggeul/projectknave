#pragma once

#include "ofxSvgCommand.h"

class ofxSvgMoveTo : public ofxSvgCommand {
public:
	ofxVec2f offset;
	ofxSvgMoveTo(ofPoint point, bool relative = false) {
		offset.set(point.x, point.y);
		this->relative = relative;
	}
	ofxSvgMoveTo(const ofxSvgMoveTo& from) {
		offset.set(from.offset.x, from.offset.y);
	}
	void draw(ofxSvgPathContext& context) const {
		if(relative) {
			ofVertex(
				context.lastPoint.x + offset.x,
				context.lastPoint.y + offset.y);
			context.lastPoint += offset;
		} else {
			ofVertex(
				offset.x,
				offset.y);
			context.lastPoint.set(offset);
		}
	}
	ostream& put(ostream &out) const {
		if(relative)
			out << "m";
		else
			out << "M";
		out << offset.x << "," << offset.y;
		return out;
	}
};
