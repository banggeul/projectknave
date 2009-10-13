#include "Fern.h"

bool Fern::svgEnable = true;

ofxVec2f makeVariation(const ofxVec2f& variance) {
	return ofxVec2f(
		ofRandom(variance.x, -variance.x),
		ofRandom(variance.y, -variance.y));
}

ofxVec3f makeSpherical(float length, const ofxVec2f& rotation) {
	return ofxVec3f(
		length * cosf(ofDegToRad(rotation.x)) * sinf(ofDegToRad(rotation.y)),
		length * sinf(ofDegToRad(rotation.x)) * sinf(ofDegToRad(rotation.y)),
		length * cosf(ofDegToRad(rotation.y)));
}

Fern::Fern() {
}

void Fern::setup(const FernStyle* style, int level, float length, ofxVec2f angle, bool side) {
	this->style = style;
	this->level = level;
	this->length = length;
	this->angle = angle;
	if(level > 0 && length > style->minLength) {
		branches.push_back(Fern());
		branches.back().setup(
			style,
			level,
			length * style->trunkScaling,
			(side ? style->leftTrunkAngle : style->rightTrunkAngle) *
				style->trunkAngle + makeVariation(style->trunkVariation),
			side);

		branches.push_back(Fern());
		branches.back().setup(
			style,
			level - 1,
			length * style->branchScaling,
			(style->leftBranchAngle * style->branchAngle) + makeVariation(style->branchVariation),
			false);

		branches.push_back(Fern());
		branches.back().setup(
			style,
			level - 1,
			length * style->branchScaling,
			(style->rightBranchAngle * style->branchAngle) + makeVariation(style->branchVariation),
			true);
	}
}

void Fern::setup(const FernStyle& fernStyle, float baseLength) {
	branches.clear();
	setup(&fernStyle, 2, baseLength, ofxVec2f(0, 0), ofRandom(-1, 1) > 0);
}

void Fern::draw(ofxSvg& svg) {
	draw(1, svg, ofxPoint3f(0, 0, 0), ofxVec2f(0, 90));
}

void Fern::draw(float age, ofxSvg& svg) {
	draw(powf(age, style->overallGrowth), svg, ofxPoint3f(0, 0, 0), ofxVec2f(0, 90));
}

void Fern::draw(float age, ofxSvg& svg, ofxPoint3f translation, ofxVec2f rotation) {
	rotation += angle;

	float ageLength = age * length;

	ofPushMatrix();
	ofTranslate(translation.x, translation.y, translation.z);
	ofRotate(rotation.x, 0, 0, 1);
	ofRotate(rotation.y - 90, 0, 1, 0);
	if(svgEnable && level == 0) {
		float leafLength = powf(age, style->leafGrowth) * length;
		svg.drawPlain(style->leafScale * leafLength / svg.getWidth());
	} else {
		ofRect(
			0,
			ageLength * -(style->branchWidth) / 2,
			ageLength,
			ageLength * style->branchWidth);
	}
	ofPopMatrix();

	if(branches.size() != 0 && ageLength > style->minLength) {
		translation += makeSpherical(ageLength, rotation);
		for(int i = 0; i < branches.size(); i++)
			branches[i].draw(age, svg, translation, rotation);
	}
}
