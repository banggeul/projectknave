#ifndef _VEC_FIELD
#define _VEC_FIELD

#define OF_ADDON_USING_OFXVECTORMATH

#include "ofMain.h"
#include "ofxVectorMath.h"

#define NUM_BINS_X 120
#define NUM_BINS_Y 80

class vectorField{

	public:
		vectorField();

		void addIntoField(float x, float y, ofxVec2f vec, float radius);
		void blur(float amnt);
		ofxVec2f readFromField(float x, float y);
		void normalize();
		void randomize(float mag, float pct);
		void clear();
		void fade(float pct);
		void draw(int x, int y, float width, float height, float scale);

        ofxVec2f field[NUM_BINS_X][NUM_BINS_Y];
        float binW, binH;

};

#endif

