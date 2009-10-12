#ifndef _CV_LINE_FITTER
#define _CV_LINE_FITTER


#include "ofMain.h"
#include "ofxVectorMath.h"
#include "Matrix.h"

//=================================================================================
class cvLineFitter {

	public:

		void fitLine(ofxPoint2f * pts, int nPts, float &slope, float &intercept, float &chiSqr);
		
		
};







#endif 
























