#ifndef _CENTROID_SKEL
#define _CENTROID_SKEL


#include "ofMain.h"
#include "ofxXmlSettings.h"

typedef struct{
	int 	posx;
	int 	posy;
	bool 	bOn;
	bool 	bSelected;
	int 	level;
} centroid;

typedef struct{
	int 	centa;
	int 	centb;
	bool 	bSelected;
} centLine;

#define MAX_NUM_CENTROID_LINES				20

//---------------------------------------------------------------
class centroidSkel{
	
	public:
		
		centroidSkel(int _nLevels, int width, int height);
		void computeCentroid (unsigned char * data);
		void computeCentroid(int x, int y, int w, int h, unsigned char * data, bool splitOnUpDown, int level, int id, int addToId);
		void loadFromXML(char * fileName);
		void saveToXML(char * fileName);
		void draw();
		int width, height;	
		
		centroid * 		cent;	
		int 				nCentroids;
		int 				nLevels;
		bool 				bDrawInOrder;
		bool 				bDrawingLines;
		
		bool 				bLinesVisible;
		bool 				bPointsVisible;
		
		
		void handleMouseDown(float x, float y);
		void handleMouseUp(float x, float y);
		
		centLine			lines[MAX_NUM_CENTROID_LINES];
		int				nLines;
		
		void 				clearAllLines(){	nLines = 0; };
		
		
};
//---------------------------------------------------------------

#endif	// 