

#include "particleManager.h"
#include "renderConstants.h"




particleManager::particleManager(){
	maxPtsSize = 20.0;
	maxAlpha = 1.0;
	mode = 0;
	minNoise = 0.499;
	maxNoise = 0.501;
	impNoise = new ImprovedNoise();
}

//---------------------------------------------------------------------------------
void particleManager::setupWorld(){
	
	setupBins(OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	
	
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
		
		// get the pos from the particle system
		if(j < NUM_PARTICLES_VBO) {
			vboPts[j][0] = particles[j].pos.x;
			vboPts[j][1] = particles[j].pos.y;
			vboPts[j][2] = particles[j].pos.z;
			
			vboColor[j][0] = 1.0;
			vboColor[j][1] = 1.0;
			vboColor[j][2] = 1.0;
			vboColor[j][3] = maxAlpha;
			vboPtsSize[j] = 10.0;
		}
	}
	
	shader.loadShader("sceneAssets/stars/VBOShader");
	ofDisableArbTex();
	theDot.loadImage("images/spot.png");
	
	// set up the vbo
	glGenBuffersARB(2, &vbo[0]);
	
	
	// VBO for color
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, NUM_PARTICLES_VBO*4*sizeof(float), vboColor, GL_STREAM_DRAW_ARB);
	
	// VBO for vertex positions
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, NUM_PARTICLES_VBO*3*sizeof(float), vboPts, GL_STREAM_DRAW_ARB);
	
}


void particleManager::computeStatistics(){
	
	bool bSkipFirst200 = false;
	
	
	avgVelocity.set(0,0);
	avgPosition.set(0,0);
	stdDeviation.set(0,0);
	
	int nParticles = particles.size() - (bSkipFirst200 ? 200 : 0);
	
	for(int i = (bSkipFirst200 ? 200 : 0); i < particles.size(); i ++){
		
		
		avgPosition += particles[i].pos;
		avgVelocity += particles[i].vel;
		//stdDeviation += particles[i].pos;
	}
	
	avgPosition /= nParticles;
	avgVelocity /= nParticles;
	
	//http://en.wikipedia.org/wiki/Standard_deviation
	
	for(int i = (bSkipFirst200 ? 200 : 0); i < particles.size(); i ++){
		stdDeviation.x += (avgPosition.x - particles[i].pos.x) * (avgPosition.x - particles[i].pos.x);
		stdDeviation.y += (avgPosition.y - particles[i].pos.y) * (avgPosition.y - particles[i].pos.y);
	}
	
	stdDeviation /= nParticles;
	stdDeviation.x = sqrt(stdDeviation.x);
	stdDeviation.y = sqrt(stdDeviation.y);
	
	
	avgPosition.x /= (float)OFFSCREEN_WIDTH;
	avgPosition.y /= (float)OFFSCREEN_HEIGHT;
	avgVelocity.x /= (float)OFFSCREEN_WIDTH;
	avgVelocity.y /= (float)OFFSCREEN_HEIGHT;
	stdDeviation.x /= (float)OFFSCREEN_WIDTH;
	stdDeviation.y /= (float)OFFSCREEN_WIDTH;
	
	ofxOscMessage msg;
	msg.setAddress("/continuous");									//	bang
	msg.addStringArg("statistics");					//	handTrigger
	msg.addIntArg(4);									//	SCENE 4 (?)
	
	avgPosition.x	= ofClamp(avgPosition.x ,	0,1);
	avgPosition.y	= ofClamp(avgPosition.y,	0,1);
	stdDeviation.x	= ofClamp(stdDeviation.x ,	0,1);
	stdDeviation.y	= ofClamp(stdDeviation.y,	0,1);
	
	msg.addFloatArg(avgPosition.x);											// avgPosition.x
	msg.addFloatArg(avgPosition.y);											// avgPosition.y
	msg.addFloatArg(avgVelocity.x);											// avgVelocity.x
	msg.addFloatArg(avgVelocity.y);											// avgVelocity.y
	msg.addFloatArg(stdDeviation.x);											// stdDeviation.x
	msg.addFloatArg(stdDeviation.y);											// stdDeviation.y
	
	//DAITO --- >   /continuous /statistics /4 /avgPosition.x /avgPosition.y /avgVelocity.x /avgVelocity.y /stdDeviation.x /stdDeviation.y
	
	
	ofxDaito::sendCustom(msg);
	
	
	
	
	/*
	 printf("avg position %f %f \n", avgPosition.x, avgPosition.y);
	 printf("avg vel %f %f \n", avgVelocity.x, avgVelocity.y);
	 printf("std dev  %f %f \n", stdDeviation.x, stdDeviation.y);
	 */
	/*
	 avg position 0.485056 0.520628
	 avg vel 0.000000 0.000045
	 std dev  0.247757 0.145714
	 
	 */
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
	
	
	if (ofGetFrameNum() % 4 == 0){
		computeStatistics();
	}
	
	
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
		
        ofxVec2f velFromVF = VF.readFromField(particles[i].pos.x / (float)OFFSCREEN_WIDTH, particles[i].pos.y / (float)OFFSCREEN_HEIGHT);
		
        particles[i].resetForce();
        particles[i].bitFlagW = 0;
        particles[i].bitFlagH = 0;
        computeBinPosition((int)(particles[i].pos.x), (int)(particles[i].pos.y), &particles[i].bitFlagW, &particles[i].bitFlagH);
		
        if (!particles[i].bBeenChosen){
            particles[i].vel.x += velFromVF.x*3;
            particles[i].vel.y += velFromVF.y*3;
        }
		
		// get the pos from the particle system
		if(i < NUM_PARTICLES_VBO) {
			vboPts[i][0] = particles[i].pos.x;
			vboPts[i][1] = particles[i].pos.y;
			vboPts[i][2] = particles[i].pos.z;
			
			vboPtsSize[i] = MIN(particles[i].radius/5.0, 1.0) * maxPtsSize; 
			
			
			vboColor[i][0] = 1.0;
			vboColor[i][1] = MIN(particles[i].radius/5.0, 1.0);
			vboColor[i][2] = 1.0;
			vboColor[i][3] = MIN(particles[i].radius/5.0, maxAlpha);
			
			
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
			VF.addIntoField( (VFs[i].point.x)/(float)OFFSCREEN_WIDTH, VFs[i].point.y/(float)OFFSCREEN_HEIGHT, VFs[i].diffPoint*0.001  , 0.093 * 1.6);
		}
	}
	
}


//---------------------------------------------------------------------------------
void particleManager::draw(){
	
	shader.setShaderActive(true); 
	
	// Get the attribute and bind it
	GLint pixel_loc = glGetAttribLocationARB(shader.shader, "particleSize");
	glVertexAttribPointerARB(pixel_loc, 1, GL_FLOAT, false, 0, vboPtsSize);
	glBindAttribLocationARB(shader.shader, pixel_loc, "particleSize");
	
	
	// Enable Sprites and texture replace
	glEnable(GL_POINT_SPRITE);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	
	
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);			// Enable Vertex Points
	glEnableClientState(GL_VERTEX_ARRAY);			// Enable Vertex
	glEnableClientState(GL_COLOR_ARRAY);			// Enable Color
	
	
	// Color
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);	// Bind the color VBO
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, NUM_PARTICLES_VBO * 4 * sizeof(float), vboColor);
	glColorPointer(4, GL_FLOAT, 0, 0);
	
	
	// Pnts
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);	// Bind the pnts VBO
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, NUM_PARTICLES_VBO * 3 * sizeof(float), vboPts);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	
	// Enable the PixelSize Attribute
	glEnableVertexAttribArrayARB(pixel_loc);
	
	
	ofDisableArbTex();
	ofEnableAlphaBlending();
	
	// Additive Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	//	shader.setUniformVariable3f("theColor", 1.0, 1.0, 0.0);	// Set Shader to Yellow
	theDot.getTextureReference().bind();
	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES_VBO);				// Draw Just Points
	theDot.getTextureReference().unbind();
	//shader.setUniformVariable3f("theColor", 1.0, 1.0, 1.0);	// Set Shader to White
	ofDisableAlphaBlending();
	ofEnableArbTex();
	
	// Clean up
	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_POINT_SPRITE);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY); 
	glDisableVertexAttribArrayARB(pixel_loc);
	
	// bind other buffer
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	shader.setShaderActive(false);
	
	
	
	glPopMatrix();
	
	
	
	
	
	
	float time = ofGetElapsedTimeMillis();
	float dotR = 10;
	
    ofSetColor(255,255,255);
	ofFill();
	for(int i = 0; i < particles.size(); i++){
	
		if (mode!=5){
			ofCircle(particles[i].pos.x, particles[i].pos.y, particles[i].radius * 0.7);
		} else {
			ofCircle(particles[i].pos.x, particles[i].pos.y, particles[i].radius * 2 * 0.7 + particles[i].fiveEnergy * 2.5);
		}
	}
	
	// VF.draw(0,0,ofGetWidth(), ofGetHeight(), 20);
	
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
