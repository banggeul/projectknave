#pragma once


class TreeBlob {

public:
	ofRectangle rect;
	ofPoint center;
	int id;
	float initTime;
	float age;
	
	bool  bAlive;
	
	TreeBlob() {
		rect.x = 0;
		rect.y = 0;
		rect.width = 0;
		rect.height = 0;
		center = 0;
		bAlive = false;	
		id = -1;
		age = 0;
		initTime = 0;
	}
	
	
};