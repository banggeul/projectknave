

// this is to deal with some stuff that stefan removed in 0.06 AND 
// stuff from temptLA....



#ifndef OFX_CV_GRAYSCALE_ADV_IMAGE_H
#define OFX_CV_GRAYSCALE_ADV_IMAGE_H

#include "ofxCvGrayscaleImage.h"

#define DEFAULT_BRIGHTNESS 0.0
#define DEFAULT_CONTRAST 0.2


// note: 
//
// this is (a) a fix of some ROI stuff for swapTemp
// and a missing "drawBlobIntoMe" function
// and applyMinMaxGamma and applyBrightnessContrast()
// 
// I think many of these things will get back into SVN version of ofxOpenCv, but are 
// included here so that this app compiled sans too much work in OF 0.06.
// 

class ofxCvGrayscaleAdvanced : public ofxCvGrayscaleImage {


  public:

    ofxCvGrayscaleAdvanced();
    ~ofxCvGrayscaleAdvanced(){};
	
	void	swapTemp();			// this has a fix for ROI stuff needed for 
	
	void	drawBlobIntoMe( ofxCvBlob & blob, int color );
	
	int		maxPts;
	CvPoint* ptsPreallocated;
	void	preAllocate(int amount){
		maxPts = amount;
		ptsPreallocated = new CvPoint[amount];
	}
	void	drawBlobIntoMeWithPreallocatedPts( ofPoint * pts, int nPts, int color );
	
		
	void	applyMinMaxGamma(float gamma);
	void	applyBrightnessContrast(float brightness, float contrast);

	
	
	
private:
		
    uchar		gammaLut[256];
	CvMat*		gammaLutMatrix;
	
	uchar		briConLut[256];
	CvMat*		briConLutMatrix;

    
};



#endif

