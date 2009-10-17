#pragma once


class TreeBlob {

public:
	
	int id;
	float initTime;
	float age;
	
	bool  bAlive;
	
	TreeBlob() {
		bAlive = false;	
		id = -1;
		age = 0;
		initTime = 0;
	}
	
	
};