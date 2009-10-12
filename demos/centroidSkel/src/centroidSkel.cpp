#include "centroidSkel.h"





//---------------------------------------------------------------- t o d o 
//---------------------------------------------------------------- - - - - 

// 	ipp optimization
// 	wierd scale by 3 things right now...
// 	delete all lines, delete selected line?
// 	how to pass the data out?


//---------------------------------------------------------------
centroidSkel::centroidSkel(int _nLevels, int _width, int _height){	

	nLevels				= _nLevels;
	nCentroids 			= pow(2, nLevels-1) * 2;
	cent 					= new centroid[ nCentroids ];
	width					= _width;
	height				= _height;
	
	for (int i = 0; i < nCentroids; i++){
		cent[i].bOn 		= false;
	}
	
	bDrawInOrder 		= true;
	bDrawingLines		= false;
	bLinesVisible		= true;
	bPointsVisible		= false;
	nLines				= 0;
}


//---------------------------------------------------------------
void centroidSkel::loadFromXML(char * fileName){
	
	
	char handyStr[255];
	int  counter = 0;
	
	//------------------------------------------ count on points
	int nPointsOn = 0;
	
	ofxXmlSettings inXml;
	inXml.loadFile(fileName); // Finally read the file to disk
		
	nPointsOn = inXml.getValue("params:numPointsTracked", 0);

	printf(" nPointsOn %i  \n", nPointsOn);
	//------------------------------------------------------------------ save out points
	int onId;
	for (int i = 0; i < nPointsOn; i++){
		sprintf(handyStr, "params:point_%i:id", i);
		onId = inXml.getValue(handyStr, -1);
		if (onId != -1){
			cent[onId].bOn = true;
		}
	}
	
	//------------------------------------------------------------------ save out lines
	
	nLines = inXml.getValue("params:num_lines", 0);
	printf("numLines = %i \n", nLines);
	int tempa, tempb;
	
	for (int i = 0; i < nLines; i++){
	
			sprintf(handyStr, "params:line_%i:pta", i);
			tempa = inXml.getValue(handyStr, -1);
			if (tempa != -1){
				 lines[i].centa = tempa;
			}
			sprintf(handyStr, "params:line_%i:ptb", i);
			tempb = inXml.getValue(handyStr, -1);
			if (tempb != -1){
				 lines[i].centb = tempb;
			}
	}
	
}

//---------------------------------------------------------------
void centroidSkel::saveToXML(char * fileName){

	char handyStr[255];
	int  counter = 0;
	//------------------------------------------ count on points
	int nPointsOn = 0;
	for (int i = 0; i < nCentroids; i++){
		if (	cent[i].bOn ) nPointsOn++;
	}

	ofxXmlSettings outXml;
	outXml.addValue("params:numPointsTracked", nPointsOn);
	
	//------------------------------------------------------------------ save out points
	for (int i = 0; i < nCentroids; i++){
		if (	cent[i].bOn ) {
			sprintf(handyStr, "params:point_%i:id", counter);
			outXml.addValue(handyStr,   i);
			counter++;
		}
	}
	
	//------------------------------------------------------------------ save out lines
	
	int nLinesBounded = nLines;
	if (nLinesBounded > MAX_NUM_CENTROID_LINES) nLinesBounded = MAX_NUM_CENTROID_LINES;
	
	outXml.addValue("params:num_lines", nLinesBounded);
	for (int i = 0; i < nLinesBounded; i++){
	
			sprintf(handyStr, "params:line_%i:pta", i);
			outXml.addValue(handyStr,   lines[i].centa);
			sprintf(handyStr, "params:line_%i:ptb", i);
			outXml.addValue(handyStr,   lines[i].centb);
			counter++;
	
	}
	
	outXml.saveFile(fileName); // Finally write the file to disk
	
}

//---------------------------------------------------------------
void centroidSkel::draw(){
	
	
	if (bPointsVisible){
		if (bDrawInOrder){
			for (int i = 0; i < nCentroids; i++){
				ofFill();
				if (i < nCentroids/2) glColor3f(0.3,0.3,0.3);
				else glColor3f(0.6,0.6,0.6);
				
				if (	cent[i].bOn ) glColor3f(1,0,0);
				ofRect(cent[i].posx - 5,cent[i].posy-5,10,10);
			}
		} else {
			for (int i = nCentroids-1; i >= 0; i--){
				ofFill();
				if (i < nCentroids/2) glColor3f(0.3,0.3,0.3);
				else glColor3f(0.6,0.6,0.6);
				
				if (	cent[i].bOn ) glColor3f(1,0,0);
				ofRect(cent[i].posx - 5,cent[i].posy-5,10,10);
			}
		}
		
		
		for (int i = 0; i < nCentroids; i++){
			if (	cent[i].bOn ){ glColor3f(1,0,0);
				
				ofRect(cent[i].posx  - 5,cent[i].posy-5,10,10);
			}	
		}
	}

	if (bLinesVisible){		
		int nToDraw = nLines;
		if (nToDraw > MAX_NUM_CENTROID_LINES) nToDraw = MAX_NUM_CENTROID_LINES;
		for (int i = 0; i < nToDraw; i++){
			int pta = lines[i].centa;
			int ptb = lines[i].centb;
			
			glColor3f(0,1,1);
			ofLine(cent[pta].posx,cent[pta].posy,cent[ptb].posx,cent[ptb].posy);
		}
	}
}



//---------------------------------------------------------------
void centroidSkel::handleMouseDown(float x, float y){
	
	if (true){
		//------------------------------------------------------ selection
		if (!bDrawInOrder){
			for (int i = 0; i < nCentroids; i++){
			int x0 = cent[i].posx  - 5;
			int x1 = cent[i].posx  + 10;
			int y0 = cent[i].posy  - 5;
			int y1 = cent[i].posy  + 10;
			if (x > x0 && x < x1) 
				if (y > y0 && y < y1){
					if (bDrawingLines){
						if (cent[i].bOn){
							cent[i].bSelected = true;
						}
					} else {
						cent[i].bOn = !cent[i].bOn;
					}	
					return;
				}
			}
		} else {
			for (int i = nCentroids-1; i >= 0; i--){
				int x0 = cent[i].posx  - 5;
				int x1 = cent[i].posx  + 10;
				int y0 = cent[i].posy  - 5;
				int y1 = cent[i].posy  + 10;
				if (x > x0 && x < x1) 
				if (y > y0 && y < y1){
					if (bDrawingLines){
						if (cent[i].bOn){
							cent[i].bSelected = true;
						}
					} else {
						cent[i].bOn = !cent[i].bOn;
					}	
					return;
				}
			}
		
		}
	}
		
		if (!bDrawingLines){
			for (int i = 0; i < nCentroids; i++) cent[i].bSelected = false;
		}
	
}


//---------------------------------------------------------------
void centroidSkel::handleMouseUp(float x, float y){
	
	if (!bDrawingLines){
		for (int i = 0; i < nCentroids; i++) cent[i].bSelected = false;
		return; 
	}
	
	bool 	bAnySelected = false;
	int  	nSelected	 = -1;
	
	for (int i = 0; i < nCentroids; i++){
		if (cent[i].bSelected == true){
			bAnySelected = true;
			nSelected = i;
		}
	}
	
	if (!bAnySelected){
		return;
	} else {
		for (int i = 0; i < nCentroids; i++) cent[i].bSelected = false;
	}

	if (!bDrawInOrder){
		for (int i = 0; i < nCentroids; i++){
		int x0 = cent[i].posx  - 5;
		int x1 = cent[i].posx  + 10;
		int y0 = cent[i].posy  - 5;
		int y1 = cent[i].posy  + 10;
		if (x > x0 && x < x1) 
		if (y > y0 && y < y1){
			if (i != nSelected && cent[i].bOn){
				lines[nLines % MAX_NUM_CENTROID_LINES].centa = i;
				lines[nLines % MAX_NUM_CENTROID_LINES].centb = nSelected;
				nLines++;
				return;
			}
			
		}
	}
	} else {
		for (int i = nCentroids-1; i >= 0; i--){
			int x0 = cent[i].posx  - 5;
			int x1 = cent[i].posx  + 10;
			int y0 = cent[i].posy  - 5;
			int y1 = cent[i].posy  + 10;
			if (x > x0 && x < x1) 
			if (y > y0 && y < y1){
				if (i != nSelected && cent[i].bOn){			
					lines[nLines % MAX_NUM_CENTROID_LINES].centa = i;
					lines[nLines % MAX_NUM_CENTROID_LINES].centb = nSelected;
					nLines++;
					return;
				}
				
			}
		}
	}
}
	


//---------------------------------------------------------------
void centroidSkel::computeCentroid (unsigned char * data){
	computeCentroid(0,0, width, height, data, true, 0, 1, 0);
	computeCentroid(0,0, width, height, data, false, 0, 1, nCentroids/2);
}


//---------------------------------------------------------------
void centroidSkel::computeCentroid(int x, int y, int w, int h, unsigned char * data, bool splitOnUpDown, int level, int id, int addToId){

	if (level == nLevels-1 ) return;

	int xPos = 0;
	int yPos = 0;
	int nPix = 0;
	for (int i = x; i < x+w; i++){
		for (int j = y; j < y+h; j++){
			int pos = j * width + i;
			if (data[pos] == 255){
				xPos += i;
				yPos += j;
				nPix++;
			}
		}	
	}
	
	if (nPix > 0){
	
		xPos = (int)((float)xPos / (float)nPix);
		yPos = (int)((float)yPos / (float)nPix);
		
		cent[id + addToId].posx 	= xPos;
		cent[id + addToId].posy 	= yPos;
		cent[id + addToId].level		= level;
		
		splitOnUpDown = !splitOnUpDown;
		level = level+1;
		if (splitOnUpDown){
			id *= 2;
			computeCentroid(x,y,w,(yPos - y), data,  splitOnUpDown, level, id, addToId);
			computeCentroid(x,yPos,w,h-(yPos-y),  data,  splitOnUpDown, level, id+1, addToId);
		} else {
			id *= 2;
			computeCentroid(x,y,(xPos-x),h,  data,  splitOnUpDown, level, id, addToId);
			computeCentroid(xPos,y,w - (xPos-x),h,   data,  splitOnUpDown, level, id+1, addToId);
		}
	}
}
