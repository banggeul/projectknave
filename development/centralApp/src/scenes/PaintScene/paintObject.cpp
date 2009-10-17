/*
 *  paintObject.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 10/16/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "paintObject.h"





void paintObject::trackContour(ofPoint * pts, int _nPts){
	
	// TRACKINGnPts
	int nPts = _nPts;
	bool bFoundThisFrame [nPts];
	for (int i = 0;  i < nPts; i++){
		bFoundThisFrame[i] = false;
	}
	
	for (int i = 0; i < COBJ.size(); i++){
		COBJ[i].bFoundThisFrame = false;
		COBJ[i].whoThisFrame = -1;
	}
	
	for (int i = 0; i < COBJ.size(); i++){
		
		
		float minDistance = 1000000;
		int minIndex = -1;
		
		for (int j = 0; j < nPts; j++){
			
			if (bFoundThisFrame[j]) continue;
			// try to find the closest "hand" to this object, withing a range
			float diffx = COBJ[i].center.x - pts[j].x;
			float diffy =  COBJ[i].center.y - pts[j].y;
			float distance = sqrt(diffx * diffx + diffy*diffy);
			
			if (minDistance > distance){
				minDistance = distance;
				minIndex = j;
			}
		}
		
		
		if (minIndex != -1 && minDistance < 40){
			bFoundThisFrame[minIndex] = true;
			// kk we are close, so let's mark and keep rolling:
			
			COBJ[i].previousCenter.set(COBJ[i].center.x, COBJ[i].center.y, 0);
			COBJ[i].center.set(pts[minIndex].x, pts[minIndex].y, 0);
			COBJ[i].bFoundThisFrame = true;
			COBJ[i].whoThisFrame = minIndex;
			COBJ[i].energy += 0.1f;
			COBJ[i].energy = MIN(COBJ[i].energy, 1);
			
			COBJ[i].nFramesFound++;
		}
	}
	
	
	// ok for ALL non found blobs, add them to the vector
	for (int i = 0;  i < nPts; i++){
		if (bFoundThisFrame[i] == false){
			// make some $ in COBJ
			contourObject temp;
			COBJ.push_back( temp);
			COBJ[COBJ.size()-1].bFoundThisFrame = true;
			COBJ[COBJ.size()-1].whoThisFrame = i;
			COBJ[COBJ.size()-1].energy = 0.3;
			COBJ[COBJ.size()-1].id = pointCount;
			COBJ[COBJ.size()-1].center.x = pts[i].x;
			COBJ[COBJ.size()-1].center.y = pts[i].y;
			COBJ[COBJ.size()-1].previousCenter.set(COBJ[COBJ.size()-1].center.x, COBJ[COBJ.size()-1].center.y, 0);
			COBJ[COBJ.size()-1].nFramesFound = 0;
			pointCount ++;
			bFoundThisFrame[i] = true;
			
			
		}
	}
	
	
	// do some other stuff, delete off old ones:
	for (int i = 0;  i < COBJ.size(); i++){
		if (COBJ[i].bFoundThisFrame == false){
			COBJ[i].energy *= 0.9f;
			COBJ[i].nFramesFound = 0;
		}
	}
	
	std::vector<contourObject >::iterator iter = COBJ.begin();
	while (iter != COBJ.end())
	{
		if ((*iter).energy <= 0.05)
		{
			
			// seems to work!
			//printf("removing a packet with ID  %i \n", (*iter)->packetID);
			//delete (*iter);	// this should !!! free memory.  please check
			
			iter = COBJ.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}