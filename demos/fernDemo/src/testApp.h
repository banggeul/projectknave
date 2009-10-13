#include "ofMain.h"

#include "RandomFern.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	RandomFern fern;
	float lastTime, regrowRate;
};
