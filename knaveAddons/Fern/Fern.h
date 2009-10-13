#pragma once

#include "ofMain.h"
#include "ofxSvg.h"
#include "FernStyle.h"

class Fern {
protected:
	vector<Fern> branches;
	int level;
	float length;
	ofxVec2f angle;
	bool side;

	const FernStyle* style;

	void setup(const FernStyle* style, int level, float length, ofxVec2f angle, bool side);
public:
	static bool svgEnable;

	Fern();
	void setup(const FernStyle& style, float baseLength);
	void draw(ofxSvg& svg);
	void draw(float age, ofxSvg& svg);
	void draw(float age, ofxSvg& svg, ofxPoint3f translation, ofxVec2f rotation);
};
