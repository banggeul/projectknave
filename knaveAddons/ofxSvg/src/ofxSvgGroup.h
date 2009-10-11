#pragma once

#include "ofxSvgDrawable.h"

class ofxSvgGroup : public ofxSvgDrawable {
protected:
	vector<ofxSvgDrawable*> parts;
public:
	int size() const {
		return parts.size();
	}
	void draw() {
		for(int i = 0; i < size(); i++)
			parts[i]->draw();
	}
};
