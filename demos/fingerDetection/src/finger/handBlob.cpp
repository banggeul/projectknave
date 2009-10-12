#include "handBlob.h"


double returnAngle(ofxPoint2f pointdata, ofxPoint2f pointdata1);
double returnAngle(ofxPoint2f pointdata, ofxPoint2f pointdata1){
	return (double)atan2((pointdata1.y - pointdata.y), pointdata1.x - pointdata.x);
}
		

//--------------------------------------------------------------
handBlob::handBlob(){
	nPts				= 0;
	currentAngle 		= 0;
	maxNumPts			= MAX_NUM_FINGER_CONTOUR_POINTS;
	lengthSection 		= 1.0f;	
	angleThreshold		= -1.1;
	minRun 				= 10;
}


//--------------------------------------------------------------
void handBlob::start (ofxPoint2f pt){
	startPosition.set(pt);
	currentPosition.set(pt);	
	currentAngle 		= 0;
	nPts				= 0;
	maxNumPts 			= MAX_NUM_FINGER_CONTOUR_POINTS;
	pts[0] 				= currentPosition;
	nPts++;
}

//--------------------------------------------------------------
void handBlob::draw(){
	
	/*ofSetColor(127,127,127);
	
	ofBeginShape();
	for (int i = 0; i < nPts; i++){
		ofVertex(pts[i].x, pts[i].y);
	}
	ofEndShape();
	*/
	
	for (int i = 0; i < nFingers; i++){
	
		if (fingers[i].bIsAGoodFinger) ofSetColor(0xff00ff);
		else ofSetColor(0x666666);
		ofEllipse(fingers[i].pos.x, fingers[i].pos.y, 3,3);
		
		if (fingers[i].bIsAGoodFinger) {
			ofSetColor(0x00ffff);
			float x1 = fingers[i].pos.x + 20 * cos(fingers[i].angle);
			float y1 = fingers[i].pos.y + 20 * sin(fingers[i].angle);
			ofLine(fingers[i].pos.x, fingers[i].pos.y,x1, y1);
		}
	}
	
	
	/*for (int i = 0; i < nFingers; i++){
		//if (fingers[i].bIsAGoodFinger) {
			char temp[255];
			sprintf(temp, "angle %0.2f\naverage %0.2f\nstd dev %0.2f", fingers[i].fingerAngle, fingers[i].angleAverage, fingers[i].angleStdDev);
			if (fingers[i].bIsAGoodFinger)  ofSetColor(0xffffff);
			else ofSetColor(0x333333);
			float x1 = fingers[i].pos.x + 60 * cos(fingers[i].angle);
			float y1 = fingers[i].pos.y + 60 * sin(fingers[i].angle);
			ofDrawBitmapString(temp, x1, y1);
		//}
	}*/
	
	
	
	ofSetColor(0,100,100);
	for (int i = 0; i < nPts; i++){
		if (bFingerPointsConsidered[i])	ofRect(pts[i].x, pts[i].y,1,1);
	}
	
	
}

//--------------------------------------------------------------
void handBlob::heavilySmoothContour(){
	for (int k=0; k < 10; k++){
		for (int j=1; j < nPts - 1; j++){
			
			pts[j].x =  0.8f * pts[j].x +
								  0.1f * pts[j-1].x +
								  0.1f * pts[j+1].x;
			pts[j].y =  0.8f * pts[j].y +
								  0.1f * pts[j-1].y +
								  0.1f * pts[j+1].y;
			
		}
	}
}

//--------------------------------------------------------------
void handBlob::processContour(){
	
	
	
	ofxPoint2f 	pta;
	ofxPoint2f 	ptb;
	ofxPoint2f 	ptc;
	ofxVec2f		ba;
	ofxVec2f		cb;

	int kConstant = 10; //(int)(nPts/30.0f);
	
	
	//heavilySmoothContour();
	
	for (int i = 0; i < nPts; i++){
		pta = pts[(i - kConstant + nPts) % nPts];
		ptb = pts[i  ];
		ptc = pts[(i + kConstant) % nPts];
		ba.set(ptb.x - pta.x, ptb.y - pta.y);
		cb.set(ptc.x - ptb.x, ptc.y - ptb.y);
		ba.normalize();
		cb.normalize();
		float dot = ba.x * cb.x + ba.y * cb.y;
		float cross = ba.x * cb.y - ba.y * cb.x;
		float theta = acos(dot);
		if (cross < 0) { theta = 0-theta; }
		angles[i] = theta;
		
		
		// now, if the curvature is good, let's mark it:
		if (angles[i] < angleThreshold){
				
				bFingerPointRuns[i] = true;
				
				// hack to turn off curve on borders
				// this is a big hack!!, but seems to work...
				
				if ((pts[i].x < 10 || pts[i].x > 630) ||			// 320,240 is hard coded ????
					(pts[i].y < 10 || pts[i].y > 470)){				// this sux lets' rplc.
						angles[i] = 0;
						bFingerPointRuns[i] = false;
					}
				
		} else {
			bFingerPointRuns[i] = false;
		}
	
	
		bFingerPointsConsidered [i] = false;
	}
	
	
	
	// now, search for runs, and the pt for fingers in that run:
	bool bInRun 		= false;
	int startRun 		= 0;
	int fingersFound 	= 0;
	nFingers 			= 0;
	
	
	//------------------------------  if we are in a run, jump ahead to next non-run....
	int begin 		= 0;
	int end 		= nPts;
	
	if (bFingerPointRuns[0] == true){
		for (int i = 0; i < nPts; i++){
			if (bFingerPointRuns[i] == false){
				begin = i;
				end = i + nPts;
				break;
			}
		}
	}
	//------------------------------
	

	
	float maximumMin 		= 0;
	int indexOfMaxMin 		= -1;
	
	for (int i=begin; i<end; i++){
		
		int j = i % nPts;	// j in range...
		
		if (!bInRun){
		
			nPtsRun = 0;
			
			if (bFingerPointRuns[j] == true){
				bInRun = true;
				startRun = j;
				
				maximumMin 		= 0;
				indexOfMaxMin 		= -1;
				
				nPtsRun = 0;
				//ptsRun[nPtsRun] = pts[j];
				//anglesRun[nPtsRun] = angles[j];
				nPtsRun++;
			}
		
		} else {
			
			
			if (bFingerPointRuns[j] == false || i == (end - 1)){
				
				
				if (nPtsRun > minRun){
					
					if (indexOfMaxMin != -1){
						
						fingerInfo blah;
						int startme = indexOfMaxMin - 30;
						int endme = indexOfMaxMin + 30;

						nPtsRun = 0;
						
						for (int i = startme; i < endme; i++){
							int ptt = i % nPts;
							if (ptt < 0) ptt += nPts;
							bFingerPointsConsidered[ptt] = true;
							ptsRun[nPtsRun] = pts[ptt];
							anglesRun[nPtsRun] = angles[ptt];
							nPtsRun++;
							
						}
						
						bool bIsAFingerRun = bIsThisRunAFinger(ptsRun, anglesRun, nPtsRun, blah);
						if (bIsAFingerRun){
						
							if (nFingers < MAX_NUM_FINGERS){
								fingers[nFingers] = blah; // sizeof(fingerInfo));
								fingers[nFingers].pos = pts[indexOfMaxMin%nPts];
								//fingers[nFingers] = blah.pos;
								//fingerAngles[nFingers] = blah.angle;
								//chisqr[nFingers] = 0;;
								nFingers ++;
							}
						}
					}
					
				}
				
				bInRun = false;
				
			} else {
				//ptsRun[nPtsRun] = pts[j];
				//anglesRun[nPtsRun] = angles[j];
				nPtsRun++;
				if (angles[j] < maximumMin){
					maximumMin = angles[j];
					indexOfMaxMin = j;
				}
			}
		}
	}
}
		
				

//--------------------------------------------------------------
void handBlob::addPoint(ofxPoint2f pt){
	
	
	if (nPts < maxNumPts){
		
		float distance = ((ofxVec2f)(pt - currentPosition)).length();	
		if (distance > lengthSection){
			int nPointsToAdd = (distance / lengthSection);
			for (int i = 0; i < nPointsToAdd; i++){
				if (nPts == 0){
					currentAngle = returnAngle(currentPosition,pt);
				} else {	
					float diff = (returnAngle(currentPosition,pt)) - currentAngle;
					while (diff < -PI) 	diff += TWO_PI;
					while (diff > PI) 	diff -= TWO_PI;
					currentAngle += diff;
				}			
				currentPosition.x += lengthSection * cos(currentAngle);
				currentPosition.y += lengthSection * sin(currentAngle);
				pts[nPts] = currentPosition;
				nPts++;
			} 
		}
	}
}



//--------------------------------------------------------------
bool handBlob::bIsThisRunAFinger( ofxPoint2f * ptsForRun, float * angsForRun, int nPtsForRun, fingerInfo & fingersInfo){


	fingersInfo.bIsAGoodFinger = true;
	
	
	
	// what is the midpt of this finger?
	ofxPoint2f median;
	for (int i = 0; i < nPtsForRun; i++){
		median += ptsForRun[i];
	}
	median /= (float)nPtsForRun;
	
	//----------------------------------------------------------------------------------------
	// put a line through it, via linear regression
	int nLinePts = 0;
	nLinePts 		= (int)((nPtsForRun) / 2.0f);
	int midPt 		= (int)((nPtsForRun) / 2.0f);	
	for (int j=1; j< nLinePts; j++){
		int myLpt = midPt + j;
		int myRpt = midPt - j;
		lineFitPts[j-1] = (ptsForRun[myLpt] + ptsForRun[myRpt])/2.0f;
	}
	
	float slope, intercept, chisqr;
	CVLF.fitLine(lineFitPts, nLinePts-1, slope, intercept, chisqr);
	
	if (fabs(slope) < 0.000001) slope = 0.000001; // please don't divide / by zero?
	
	ofxPoint2f pta;
	pta.x = 0;
	pta.y = intercept;        

	ofxPoint2f ptb;
	ptb.y = 0;
	ptb.x = -intercept / slope;

	ofxVec2f diff = pta - ptb;
	float angle = atan2(diff.y, diff.x);
	
	// ok, now check how bad this is, vs the 1st to last:
	ofxVec2f linePt = lineFitPts[0] - lineFitPts[nLinePts-2];
	
	// now, do something smooth, linear regression sometimes fits the line the wrong direction, so 
	// fix it with the dot product : 
	linePt.normalize();
	diff.normalize();
	float me = diff.dot(linePt);
	if (me < 0) angle = angle + PI;

	fingersInfo.angle = angle;
	fingersInfo.chisqr = chisqr;
	
	//----------------------------------------------------------------------------------------
	// computing the area of the blob...
	
	CvPoint * cvpoints;
	cvpoints = new CvPoint[nPtsForRun];
	// each point add to cvpoints
	for (int i = 0; i < nPtsForRun; i++){
		cvpoints[i].x = ptsForRun[i].x;
		cvpoints[i].y = ptsForRun[i].y;
	}
	CvMemStorage* MemStorage = cvCreateMemStorage(0);
	CvSeq * seq = cvCreateSeq(CV_SEQ_POLYGON,
	sizeof(CvSeq),sizeof(CvPoint), MemStorage);
	cvSeqPushMulti( seq, cvpoints, nPtsForRun);
	
	float area = fabs(cvContourArea(seq,CV_WHOLE_SEQ));;
	float arcl = fabs(cvArcLength(seq,CV_WHOLE_SEQ));;
	
	fingersInfo.arcl = arcl;
	fingersInfo.area = area;
	
	delete cvpoints; 
	cvReleaseMemStorage( &MemStorage );
	
	//----------------------------------------------------------------------------------------
	
	
	// compute the std deviation of the angles:
	
	float avg = 0;
	
	for (int i = 0; i < nPtsForRun; i++){
		avg += angsForRun[i];
	}
	avg /= (float)nPtsForRun;
	
	
	float stdDevTotal = 0;
	for (int i = 0; i < nPtsForRun; i++){
		stdDevTotal += ((angsForRun[i] - avg)*(angsForRun[i] - avg));
	}
	stdDevTotal /= (float)nPtsForRun;
	fingersInfo.angleStdDev = sqrt(stdDevTotal);;
	
	fingersInfo.angleAverage = avg;
	
	//cout << fingersInfo.angleAverage << " " << fingersInfo.angleAverage  << endl;
	//if (fingersInfo.angleStdDev > 1.15f) fingersInfo.bIsAGoodFinger = false;

	//if (fingersInfo.angleAverage < -1.2f) fingersInfo.bIsAGoodFinger = false;
	//if (fingersInfo.angleAverage >  -0.6f) fingersInfo.bIsAGoodFinger = false;

	
	//----------------------------------------------------------------------------------------
	
	
	
	ofxVec2f widthFingerEnd;
	widthFingerEnd.set(ptsForRun[nPtsForRun-1].x - ptsForRun[0].x, ptsForRun[nPtsForRun-1].y - ptsForRun[0].y); 
	
	ofxVec2f widthFingerMid;
	
	int posa = (int)(nPtsForRun * 0.15);
	int posb = (int)(nPtsForRun * 0.85);
	
	widthFingerMid.set(ptsForRun[posa].x - ptsForRun[posb].x, ptsForRun[posa].y - ptsForRun[posb].y); 
	
	float lena = widthFingerEnd.length();
	float lenb = widthFingerMid.length();
	
	///fingersInfo.chisqr = lena / blahLen;
	//if (fingersInfo.chisqr > 0.9f) return false;
	//if (len < 6 || len > 13) return false;
	//ofxPoint2f * ptsForRun, float * angsForRun, int nPtsForRun

	//fingersInfo.chisqr = linePt.length();
	//if (linePt.length() < 5) return false;
	//
	//fingersInfo.chisqr =lena /  arcl;
	//if ((lena /  arcl) > 0.31) return false;
	
	// do the angle of the finger to the line connecting the endpoints of the run:
	;
	
	ofxPoint2f ptat = ptsForRun[0];
	ofxPoint2f ptbt =  ptsForRun[(nPtsForRun-1)/2];
	ofxPoint2f ptct =  ptsForRun[nPtsForRun-1];
	ofxVec2f ba; ba.set(ptbt.x - ptat.x, ptbt.y - ptat.y);
	ofxVec2f cb; cb.set(ptct.x - ptbt.x, ptct.y - ptbt.y);
	ba.normalize();
	cb.normalize();
	float dot = ba.x * cb.x + ba.y * cb.y;
	float cross = ba.x * cb.y - ba.y * cb.x;
	float theta = acos(dot);
	if (cross < 0) { theta = 0-theta; }
	
	theta += PI;
	if (theta < 0) theta += TWO_PI;
	if (theta > TWO_PI) theta -= TWO_PI;
	fingersInfo.fingerAngle = theta;
	
	if (theta < 0.15f || theta > 0.99f) fingersInfo.bIsAGoodFinger = false;

	return true;

}		
