#ifndef _STROKE_H
#define _STROKE_H

#include "ofMain.h"
#include "ofxVectorMath.h"


// this is for resampling the contour for network comunication 

class cvStrokeResampler {
	
	public:
		
		void	addPoint(float x, float y);
		void	clear();
		void	draw();
		float	getLength();
		
		void	update();
		void	resample(int newNumPts);


		vector <ofxVec2f> pts;

};

#endif
	
