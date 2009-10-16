#pragma once


class MonsterWindow {
	
public:
	
	ofImage *		img;
	ofPoint			pos;
	bool			bHit;
	
	MonsterWindow() {
		bHit = 0;
		pos = 0;
	}
	
	void draw() {
		ofFill();
		ofSetColor(255, 255, 3);
		if(img) {
			img->draw(pos.x, pos.y, 50, 50);	
		}
	}
	
};