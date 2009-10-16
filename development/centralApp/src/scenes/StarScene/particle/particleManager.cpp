

#include "particleManager.h"
#include "renderConstants.h"




particleManager::particleManager(){

	mode = 0;
	minNoise = 0.499;
	maxNoise = 0.501;
	impNoise = new ImprovedNoise();
}

//---------------------------------------------------------------------------------
void particleManager::setupWorld(){

setupBins(ofGetWidth(), ofGetHeight());


//------------------------------------- make some particles
particles.assign(1400, groupableParticle());

//------------------------------------- properties, like drag and pos, etc, etc.
for(int j = 0; j < particles.size(); j ++){
	particles[j].pos.set(ofxVec3f(ofRandom(0,1793), ofRandom(0,1024), 0));
	particles[j].vel.set(0,0,0);
	float drag = ofRandom(0.0717, 0.08);
	particles[j].drag = drag;
	
	particles[j].radius = 5;// + powf(ofRandom(0,1), 3) * 20;
	//if (j == 10)  particles[j].radius = 30;
}
}


float particleManager::getRads(float $val1, float $val2,  float zoff, float $mult, float $div){
	
	float rads = impNoise->noise($val1 * .0125f, $val2 * .0125f, zoff); 
	
	rads = ofMap(rads, -1, 1, 0, 1);
	
	if (rads < minNoise) minNoise = rads;
	if (rads > maxNoise) maxNoise = rads;
	
	rads -= minNoise;
	rads *= 1.0/(maxNoise - minNoise);
	
	return rads * $mult;
}


//---------------------------------------------------------------------------------
void particleManager::update(){
	//if we are meant to be in formation
	
	
	
	if (mode == 2){
		if (ofRandom(0,1)> 0.91f){
			int pos = (int)(ofRandom(0,0.99) * particles.size());
			particles[pos].peakMe(1);
			
			ofxOscMessage msg;
			msg.setAddress("/bang");								//	bang
			msg.addStringArg("explosion");					//	handTrigger
			msg.addIntArg(4);									//	SCENE 4 (?)
			
			float x = ofClamp(particles[pos].pos.x / OFFSCREEN_WIDTH, 0,1);
			float y = ofClamp(particles[pos].pos.y / OFFSCREEN_HEIGHT, 0,1);
			
			msg.addFloatArg(x);								// centroid x
			msg.addFloatArg(y);								// centroid y
			
			
			//DAITO --- >   /bang /explosion /4 /x(noramlized) /y(noramlized)
			
			ofxDaito::sendCustom(msg);
			
			
		}
	}
	
	if (mode == 3){
		if (ofRandom(0,1)> 0.85f){
			int pos = (int)(ofRandom(0,0.99) * particles.size());
			
			float angle = ofRandom(0, TWO_PI);
			particles[pos].directionallyPeakMe(1, angle);
			
			ofxOscMessage msg;
			msg.setAddress("/bang");									//	bang
			msg.addStringArg("explosionDirectional");					//	handTrigger
			msg.addIntArg(4);											//	SCENE 4 (?)
			
			float x = ofClamp(particles[pos].pos.x / OFFSCREEN_WIDTH, 0,1);
			float y = ofClamp(particles[pos].pos.y / OFFSCREEN_HEIGHT, 0,1);
			
			msg.addFloatArg(x);											// centroid x
			msg.addFloatArg(y);											// centroid y
			msg.addFloatArg(angle/TWO_PI);								// angle of explosion (0-1)
			
			
			//DAITO --- >   /bang /explosionDirectional /4 /x(noramlized) /y(noramlized) /angle
			
			ofxDaito::sendCustom(msg);
			
			
			
		}
	}
	
	if (mode == 4){
		if (ofRandom(0,1)> 0.82f){
			int pos = (int)(ofRandom(0,0.99) * particles.size());
			float angle = ofRandom(0, TWO_PI);
			particles[pos].directionallyPeakMe(1, angle);
			ofxOscMessage msg;
			msg.setAddress("/bang");									//	bang
			msg.addStringArg("explosionDirectional");					//	handTrigger
			msg.addIntArg(4);											//	SCENE 4 (?)
			
			float x = ofClamp(particles[pos].pos.x / OFFSCREEN_WIDTH, 0,1);
			float y = ofClamp(particles[pos].pos.y / OFFSCREEN_HEIGHT, 0,1);
			
			msg.addFloatArg(x);											// centroid x
			msg.addFloatArg(y);											// centroid y
			msg.addFloatArg(angle/TWO_PI);								// angle of explosion (0-1)
			
			
			//DAITO --- >   /bang /explosionDirectional /4 /x(noramlized) /y(noramlized) /angle
			
			ofxDaito::sendCustom(msg);
		}
		
		if (ofRandom(0,1)> 0.91f){
			int pos = (int)(ofRandom(0,0.99) * particles.size());
			particles[pos].peakMe(1);
			ofxOscMessage msg;
			msg.setAddress("/bang");								//	bang
			msg.addStringArg("explosion");					//	handTrigger
			msg.addIntArg(4);									//	SCENE 4 (?)
			
			float x = ofClamp(particles[pos].pos.x / OFFSCREEN_WIDTH, 0,1);
			float y = ofClamp(particles[pos].pos.y / OFFSCREEN_HEIGHT, 0,1);
			
			msg.addFloatArg(x);								// centroid x
			msg.addFloatArg(y);								// centroid y
			
			
			//DAITO --- >   /bang /explosion /4 /x(noramlized) /y(noramlized)
			
			ofxDaito::sendCustom(msg);
		}
	}
	
	
	if (mode != 5) {
		for(int j = 0; j < particles.size(); j ++){
			particles[j].radius = 2 + 20 * particles[j].energy; 
			particles[j].fiveEnergy *= 0.50;
		} 
		
		
	} else {
		for(int j = 0; j < particles.size(); j ++){ particles[j].radius = 1;
		
		particles[j].fiveEnergy = 0.94f * particles[j].fiveEnergy + 0.06f * 1.0;
		}
	}
	
	
	if (VFs.size() == 0){
		if (ofRandom(0, 1) > 0.16){
			vfAdder VA;
			VFs.push_back(VA);
			VFs[VFs.size()-1].setup();
		}
	} else {
	
		bool bErase = false;
		if (ofGetElapsedTimef() - VFs[0].startTime > VFs[0].timeOn){ 
			bErase = true;
		}
		if (bErase == true){
			VFs.erase(VFs.begin());
		}
	}
	
	if (VFs.size() > 0 && ofGetFrameNum() % 4 == 0){
		
		
		ofxOscMessage msg;
		msg.setAddress("/continuous");									//	bang
		msg.addStringArg("particlePusher");					//	handTrigger
		msg.addIntArg(4);											//	SCENE 4 (?)
		
		float x = ofClamp(VFs[0].point.x / OFFSCREEN_WIDTH, 0,1);
		float y = ofClamp(VFs[0].point.y / OFFSCREEN_HEIGHT, 0,1);
		
		msg.addFloatArg(x);											// centroid x
		msg.addFloatArg(y);											// centroid y
		
		//DAITO --- >   /continuous /particlePusher /4 /x(noramlized) /y(noramlized) 
		
		ofxDaito::sendCustom(msg);
	}
	
	
	
    VF.fade(0.99f);
	
	for(int i = 0; i < particles.size(); i++){
		
        ofxVec2f velFromVF = VF.readFromField(particles[i].pos.x / (float)ofGetWidth(), particles[i].pos.y / (float)ofGetHeight());
		
        particles[i].resetForce();
        particles[i].bitFlagW = 0;
        particles[i].bitFlagH = 0;
        computeBinPosition((int)(particles[i].pos.x), (int)(particles[i].pos.y), &particles[i].bitFlagW, &particles[i].bitFlagH);
		
        if (!particles[i].bBeenChosen){
            particles[i].vel.x += velFromVF.x*3;
            particles[i].vel.y += velFromVF.y*3;
        }
    }
	
	
	
    //---------------------------- delauny based forces??
    Delaunay::Point tempP;
	if ( v.size() == 0){
        for (int i = 0; i < particles.size(); i++){
            tempP[0] = particles[i].pos.x;
            tempP[1] = particles[i].pos.y;
            v.push_back(tempP);
        }
	} else {
	    for (int i = 0; i < particles.size(); i++){
            v[i][0] = particles[i].pos.x;
            v[i][1] = particles[i].pos.y;
        }
	}
	Delaunay delobject(v);
	delobject.Triangulate();
	
	
	//------------------------------------------ peak
	if (ofGetFrameNum() %5 == 0)
	for(Delaunay::fIterator fit  = delobject.fbegin();
	 fit != delobject.fend();
	 ++fit){
	 for(int i =0; i < 3; ++i){
	 
	 int pta = delobject.Org(fit);
	 int ptb = delobject.Sym(fit,i);
	 if (ptb != -1){
		 if (particles[pta].bBeenPeaked){
			 particles[ptb].peakMe(particles[pta].energy * 0.86f);
			 if (i == 2) particles[pta].bBeenPeaked = false;
		 }
	 }
	 }
	 }
	
	
	if (ofGetFrameNum() %2 == 0)
		for(Delaunay::fIterator fit  = delobject.fbegin();
			fit != delobject.fend();
			++fit){
			
			int pta = delobject.Org(fit);
			
			if (particles[pta].bBeenDirectionallyPeaked){
				
				float angle = particles[pta].direction;
				
				int minIndex = -1;
				float minAngle = 10000;
				for(int i =0; i < 3; ++i){
					
					int ptb = delobject.Sym(fit,i);
					if (ptb != -1){
						
						float angleB = atan2( particles[ptb].pos.y -  particles[pta].pos.y, particles[ptb].pos.x -  particles[pta].pos.x);
						float diffAngle = angle - angleB;
						if (diffAngle < -PI) diffAngle += TWO_PI;
						if (diffAngle > PI) diffAngle -= TWO_PI;
						if (fabs(diffAngle) < minAngle){
							minAngle = fabs(diffAngle);
							minIndex = delobject.Sym(fit,i);;
						}
						
					}
				}
				
				particles[pta].bBeenDirectionallyPeaked = false;
				
				if (minIndex != -1){
					particles[minIndex].directionallyPeakMe(particles[pta].energy*0.96, angle);
				}
				
				
			}
		}
	
	
	
	
	
	
	for(Delaunay::fIterator fit  = delobject.fbegin();
		fit != delobject.fend();
		++fit){
		for(int i =0; i < 3; ++i){
			
			int pta = delobject.Org(fit);
			int ptb = delobject.Sym(fit,i);
			if (ptb != -1){
                if (!(particles[pta].bBeenChosen && particles[ptb].bBeenChosen)){
					
                    bool bAnyArrow = particles[ptb].bBeenChosen || particles[ptb].bBeenChosen;
					
					float radius = particles[pta].radius + particles[ptb].radius;
					
					if (mode != 5) radius *= 2.5;
					
					
                    float distance = (particles[pta].pos - particles[ptb].pos).length();
					
                    if (!bAnyArrow){
                        if ( distance < radius) {
							if (mode != 5) particles[pta].addRepulsionForce(particles[ptb], radius, 0.1f);
							else particles[pta].addRepulsionForce(particles[ptb], radius, 0.026f);
						}
                        if ( distance < 100)  particles[pta].addAttractionForce(particles[ptb], 100, 0.001f);
                    } else {
                        if ( distance < 33) particles[pta].addRepulsionForce(particles[ptb], 33, 0.2f);
                    }
					
					
                }
            }
        }
    }
    for(int i = 0; i < particles.size(); i++){
		
		if (mode == 4){
			if (!particles[i].bBeenChosen) particles[i].addAttractionForce(1793/2, 1024/2+ 1024/4, 6000, 0.008);
		} else {
			if (mode != 5) {
				if (!particles[i].bBeenChosen) particles[i].addAttractionForce(1793/2, 1024/2+ 1024/4, 6000, 0.003);
			} else {
				particles[i].addAttractionForce( particles[i].lockTarget.x, particles[i].lockTarget.y, 6000, 0.06); //0.2f -0.2*sin(ofGetElapsedTimef()));
				//cout << particles[i].lockTarget.x <<  " " <<  particles[i].lockTarget.y << endl;
				
			}
		}
																	   
	}
	
	
	
	// some perlin forces: 
	
	
	if (mode == 1){
	for(int i = 0; i < particles.size(); i++){
		if (!particles[i].bBeenChosen) {
			
			float xyRads = getRads(particles[i].pos.x/3.0f, particles[i].pos.y/3.0f, ofGetElapsedTimef()/100, 10.f, 15.f);
			//float yRads = getRads(xLoc, yLoc, 10.f, 15.f);
			
			particles[i].addForce( cos(xyRads) * .05, -sin(xyRads) * .05);
		}
	}
	}
	
    for(int i = 0; i < particles.size(); i++){
		
		
        /*for(int j = 0; j < i; j++){
		 if (particles[i].bInSameBin(particles[j])){
		 
		 if (particles[i].bBeenChosen && particles[j].bBeenChosen) continue;
		 
		 if (particles[i].bBeenChosen || particles[j].bBeenChosen)  particles[i].addRepulsionForce(particles[j], 60, 0.8f);
		 else particles[i].addRepulsionForce(particles[j], 20, 0.2f);
		 particles[i].addAttractionForce(particles[j], 100, 0.0001f);
		 }
		 }*/
		particles[i].addDampingForce();
        particles[i].update();
		 particles[i].update();
		if (ofGetFrameRate() < 22) particles[i].update();
		
    }
	
    for(int i = 0; i < particles.size(); i++){
		
    }
	
	for (int i = 0; i< VFs.size(); i++){
		VFs[i].update();
	}
	
	
	for (int i = 0; i< VFs.size(); i++){

		if (mode !=5){
        VF.addIntoField( (VFs[i].point.x)/(float)ofGetWidth(), VFs[i].point.y/(float)ofGetHeight(), VFs[i].diffPoint*0.001  , 0.093 * 1.6);
		}
	}
	
}


//---------------------------------------------------------------------------------
void particleManager::draw(){
	
    float time = ofGetElapsedTimeMillis();
	
	
    ofSetColor(255,255,255);
	ofFill();
	for(int i = 0; i < particles.size(); i++){
		if (mode!=5){
			ofCircle(particles[i].pos.x, particles[i].pos.y, particles[i].radius * 0.7);
		} else {
			ofCircle(particles[i].pos.x, particles[i].pos.y, particles[i].radius * 2 * 0.7 + particles[i].fiveEnergy * 2.5);
		}
	}
	
    //VF.draw(0,0,ofGetWidth(), ofGetHeight(), 20);
	
    // triangle mother fucker
	
	
    //------------------------------------
	
    Delaunay::Point tempP;
	vector< Delaunay::Point > v;
	for (int i = 0; i < particles.size(); i++){
		tempP[0] = particles[i].pos.x;
		tempP[1] = particles[i].pos.y;
		v.push_back(tempP);
	}
    ofSetColor(255,255,255);
	Delaunay delobject(v);
	delobject.Triangulate();
	
	for(Delaunay::fIterator fit  = delobject.fbegin();
		fit != delobject.fend();
		++fit){
		for(int i =0; i < 3; ++i){
			
			int pta = delobject.Org(fit);
			int ptb = delobject.Sym(fit,i);
			if (ptb != -1){
				
				
				float radius = particles[pta].radius + particles[ptb].radius;
				
				if ((!particles[pta].bBeenChosen && !particles[ptb].bBeenChosen))
					if ((particles[pta].pos - particles[ptb].pos).length() < radius*2.0)
						ofLine(particles[pta].pos.x, particles[pta].pos.y, particles[ptb].pos.x, particles[ptb].pos.y);
			}
		}
	}
	ofSetColor(0xffffff);
	
	
	for (int i = 0; i< VFs.size(); i++){
		//VFs[i].draw();
	}
	
}

//--------------------------------------------------------------
void particleManager::setTrackedPoint(float x, float y){
    center.set(x, y, 0);
}

//--------------------------------------------------------------
void particleManager::assembleParticleGroups(){
	
}

//--------------------------------------------------------------
void particleManager::disassembleParticleGroup(){
	
}


void particleManager::handleMouseDrag(float x, float y){
	float pctx = (float)x / (float)ofGetWidth();
	float pcty = (float)y / (float)ofGetHeight();
	ofxVec2f curPos = ofxVec2f(pctx, pcty);
	ofxVec2f diff = curPos - prevPos;
	if (diff.length() < 4){
        ofxVec2f angle  = diff / 1.0f;
        VF.addIntoField(pctx, pcty,  angle, 0.093);
	}
	prevPos = curPos;
}


void particleManager::triangulateParticles(){
	
	Delaunay::Point tempP;
	vector< Delaunay::Point > v;
	
	for (int i = 0; i < particles.size(); i++){
		tempP[0] = particles[i].pos.x;
		tempP[1] = particles[i].pos.y;
		v.push_back(tempP);
	}
	
	Delaunay delobject(v);
	delobject.Triangulate();
	
	
	cout << "Number of vertices in file      = "
	<< delobject.nvertices() << endl;
	cout << "Number of edges in delaunay     = "
	<< delobject.nedges() << endl;
	cout << "Number of triangles in delaunay = "
	<< delobject.ntriangles() << endl;
	cout << "Hull size                       = "
	<< delobject.hull_size() << endl;
	
    /*
	 cout <<"======================================\n";
	 cout <<"     Vertices from vertex iterator    \n";
	 cout <<"======================================\n";
	 for(Delaunay::vIterator vit = delobject.vbegin();
	 vit != delobject.vend();
	 ++ vit){
	 cout << "ID : " << delobject.vertexId(vit)
	 << "\t" << *vit << endl;
	 }
	 */
	
	cout <<"======================================\n";
	cout <<"     Faces from face iterator         \n";
	cout <<"======================================\n";
	for(Delaunay::fIterator fit  = delobject.fbegin();
		fit != delobject.fend();
		++fit){
		cout << delobject.Org(fit)  << ", "
		<< delobject.Dest(fit) << ", "
		<< delobject.Apex(fit) << " \t: Area = "
		<< delobject.area(fit) << endl;
		
		cout << "Adjacent vertices are: ";
		for(int i =0; i < 3; ++i)
			cout << delobject.Sym(fit,i) << "\t";
		cout << endl;
	}
	
    /*
	 cout <<"======================================\n";
	 cout <<"     Testing point location           \n";
	 cout <<"======================================\n";
	 for(int i =0; i < delobject.nvertices(); ++i){
	 Delaunay::fIterator fit = delobject.locate(i);
	 cout << "Vertex " << i << " located in Triangle : "
	 << delobject.Org(fit)  << ", "
	 << delobject.Dest(fit) << ", "
	 << delobject.Apex(fit) << " \t: Area = "
	 << delobject.area(fit) << endl;
	 
	 }
	 
	 cout <<"======================================\n";
	 cout <<"     Testing vertex incidence         \n";
	 cout <<"======================================\n";
	 for (int j = 0 ; j < delobject.nvertices(); ++j){
	 vector<int> vav;
	 delobject.trianglesAroundVertex(j,vav);
	 cout << "Triangles around vertex [" << j << "]:\n";
	 for(unsigned int i =0; i < vav.size(); ++i){
	 if( i % 3 == 0 ) cout << endl;
	 cout << vav[i] << "\t";
	 }
	 cout << "\n---------------------------------\n";
	 }
	 */
}
