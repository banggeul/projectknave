

#include "buildingShapeManager.h"


void buildingShapeManager::parseShapesFromFile(string fileName, int _type){
	
	fileName = ofToDataPath(fileName);
	
	//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ito likes exclamation points !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
	
	ifstream::pos_type size;
	char * memblock;
	ifstream checkfile (fileName.c_str(), ios::in|ios::binary|ios::ate);
	
	int temp;
	int nBlobs;
	int centroidx;
	int centroidy;
	int x,y,w,h;
	int nPts;
	int ptx, pty;
	
	
	if (checkfile.is_open()){
		
		checkfile.seekg (0);
		
		checkfile.read ((char*) &nBlobs, sizeof (int));
		//printf("nBlobs-- %i \n", nBlobs);
		
		for (int i = 0; i < nBlobs; i++){
			
			
			shapes.push_back( new buildingShape());
			shapes[shapes.size()-1]->type = _type;
			
			shapes[shapes.size()-1]->spot = &spot;
			
			checkfile.read ((char*) &nPts, sizeof (int));
			//printf("nPts-- %i \n", nPts);
			shapes[shapes.size()-1]->nPts = nPts;
			
			
			checkfile.read ((char*) &centroidx, sizeof (int));
			//printf("centroidx -- %i \n", centroidx);	
			shapes[shapes.size()-1]->centroid.x = centroidx;
			
			checkfile.read ((char*) &centroidy, sizeof (int));
			//printf("centroidy -- %i \n", centroidy);
			shapes[shapes.size()-1]->centroid.y = centroidy;
			
			checkfile.read ((char*) &x, sizeof (int));
			//printf("rectanglex -- %i \n", x);	
			shapes[shapes.size()-1]->boundingRect.x = x;
			
			checkfile.read ((char*) &y, sizeof (int));
			//printf("rectangley -- %i \n", y);
			shapes[shapes.size()-1]->boundingRect.y = y;
			
			checkfile.read ((char*) &w, sizeof (int));
			//printf("rectanglew -- %i \n", w);	
			shapes[shapes.size()-1]->boundingRect.width = w;
			
			checkfile.read ((char*) &h, sizeof (int));
			//printf("rectangleh -- %i \n", h);
			shapes[shapes.size()-1]->boundingRect.height = h;
			
			
			shapes[shapes.size()-1]->pts = new ofPoint[ shapes[shapes.size()-1]->nPts];
			
			float length = 0;
			for (int j = 0; j < nPts; j++){
				
				checkfile.read ((char*) &ptx, sizeof (int));
				checkfile.read ((char*) &pty, sizeof (int));
				shapes[shapes.size()-1]->pts[j].set(ptx, pty, 0);
				if (j > 0){
					float dx = ptx - shapes[shapes.size()-1]->pts[j-1].x;
					float dy = pty - shapes[shapes.size()-1]->pts[j-1].y;
					length += sqrt (dx*dx + dy*dy);
				}
				//printf("blob# -- %i, Pt# -- %i, ptx, pty -- %i, %i \n", i, j, ptx,pty);	
				
			}
			shapes[shapes.size()-1]->length = length;
		}
	}
	
	printf("shapes size = %i \n", shapes.size());
	
}

void buildingShapeManager::checkShapesForPoint(float x,float y, ofPoint colorMe){
	
	for (int i = 0; i < shapes.size(); i++){
		shapes[i]->checkInside(x,y, colorMe);
	}
	
	
}



