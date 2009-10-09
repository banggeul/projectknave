#pragma once

#include "ofxSvgPathContext.h"

class ofxSvgCommand {
public:
	ofxSvgCommand() {
		relative = true;
	}
	virtual void draw(ofxSvgPathContext& context) const = 0;
	virtual ostream& put(ostream& out) const = 0;
protected:
	bool relative;
};

inline ostream& operator<<(ostream &out, const ofxSvgCommand& command) {
	return command.put(out);
}
