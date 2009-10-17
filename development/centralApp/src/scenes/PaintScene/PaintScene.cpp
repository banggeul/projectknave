
#include "PaintScene.h"


void PaintScene::setup() {
	
	
	lastFrameRendered = -1;
	// pass in this : 
	
	for (int i = 0; i < MAX_PARTICLES; i++){
		particleSystem.particles[i].myApp = this;
	}
	
	particleSystem.myApp = this;
	
	// setup fluid stuff
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true).setFadeSpeed(0.005).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setup(&fluidSolver);
	
	
	fluidCellsX			= 150; //200;
	
	drawFluid			= true;
	drawParticles		= true;
	renderUsingVA		= true;
	
	
	window.width		= 1793;
	window.height		= 1024;
	
	window.invWidth		= 1.0f/window.width;
	window.invHeight	= 1.0f/window.height;
	window.aspectRatio	= window.width * window.invHeight;
	window.aspectRatio2 = window.aspectRatio * window.aspectRatio;
	
	resizeFluid = true;
	
	
	handImgW = 640;
	handImgH = 240;
	
	
	handImage.setUseTexture(false);
	handImage.allocate(handImgW, handImgH);
	
	handImageTemporallyBlurred.setUseTexture(false);
	handImageTemporallyBlurred.allocate(handImgW, handImgH);
	handImageTemporallyBlurredInvert.allocate(handImgW, handImgH);
	
	handImage.preAllocate(MAX_BLOB_LENGTH);
	
	
}

// ------------------------------------------------
void PaintScene::update(){
	
	
	if (lastFrameRendered !=  packet.frameNumber){
		
		handImage.set(0);
		
		for (int i = 0; i < packet.nBlobs; i++){
			int nPts = packet.nPts[i];
			if (!packet.bAmInner[i]){
				handImage.drawBlobIntoMeWithPreallocatedPts(packet.pts[i], nPts, 100);
			}
		}
		
		for (int i = 0; i < packet.nBlobs; i++){
			int nPts = packet.nPts[i];
			if (packet.bAmInner[i]){
				handImage.drawBlobIntoMeWithPreallocatedPts(packet.pts[i], nPts, 0);
			}
		}
		
		handImage.blur();
		
		handImageTemporallyBlurred -= 50;
		handImageTemporallyBlurred.blur();
		handImageTemporallyBlurred.blur();
		handImageTemporallyBlurred += handImage;
		
		handImageTemporallyBlurredInvert = handImageTemporallyBlurred;
		handImageTemporallyBlurredInvert.invert();
		

	}
	
	
	
	
	
	trackBlobs();
	
	if(resizeFluid) 	{
		fluidSolver.setSize(fluidCellsX, fluidCellsX / window.aspectRatio);
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
	}
	

	
	fluidSolver.update();
	
	
	float scalex =  (float)OFFSCREEN_WIDTH / (float)packet.width;
	float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;
	
	
	
	
	for (int i = 0; i < packet.nBlobs; i++){
		
		// try to find my color :
		
		ofPoint myColor;
		myColor.set(255,0,255);
		
		for (int j = 0; j < POBJ.size(); j++){
			if (POBJ[j].bFoundThisFrame && POBJ[j].whoThisFrame == i){
				
				// now, let's do some FUN stuff with the contours pts found this frame. 
				
				// seperate function?
				
				//printf("----------  drawing %i \n", i);
				drawPaintObjectIntoFluid( POBJ[j], i);
				
				
				
			}
		}
		
				
	}
	
	
	
	
	/*
	for (int i = 0; i < packet.nBlobs; i++){
		for (int j = 0; j < packet.nPts[i]; j+=10){
			
			if (ofGetFrameNum() % 3 == 0){
			int x = packet.pts[i][j].x * scalex;
			int y = packet.pts[i][j].y * scaley;
			float mouseNormX = x * window.invWidth;
			float mouseNormY = y * window.invHeight;
			 addToFluid(mouseNormX, mouseNormY, 0.001, 0, true);
				
			}
			//glVertex2f(packet.pts[i][j].x * scalex, packet.pts[i][j].y * scaley);
		}
	}
	 */
	
	
	/*
	 float mouseNormX = x * window.invWidth;
	 float mouseNormY = y * window.invHeight;
	 
	 
	 addToFluid(mouseNormX, mouseNormY, 0.1, 0, true);
	 
	 */
}


// ------------------------------------------------
void PaintScene::drawPaintObjectIntoFluid(paintObject & PO, int whichBlob){
	
	
	for (int i = 0; i < PO.COBJ.size(); i+=8){
		if ( PO.COBJ[i].bFoundThisFrame) {
			
			if (PO.COBJ[i].nFramesFound > 0){
				
				
				//printf ("drawing in %i stored pt %i \n", whichBlob, i);
				
				//	ofPoint		center;
				//ofPoint		previousCenter;
				
				float vx =  PO.COBJ[i].center.x -  PO.COBJ[i].previousCenter.x;
				float vy =  PO.COBJ[i].center.y -  PO.COBJ[i].previousCenter.y;
				float px = PO.COBJ[i].center.x ;
				float py = PO.COBJ[i].center.y;
				
				float scalex =  (float)OFFSCREEN_WIDTH / (float)packet.width;
				float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;
				
				
				vx *= window.invWidth * scalex;
				vy *= window.invHeight * scaley;
				px *= window.invWidth * scalex;
				py *= window.invHeight * scaley;
				
				addToFluid(px, py, vx*0.1, vy*0.1, true);
				
			} else {
				
				float px = PO.COBJ[i].center.x ;
				float py = PO.COBJ[i].center.y;
				
				float scalex =  (float)OFFSCREEN_WIDTH / (float)packet.width;
				float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;
				
				px *= window.invWidth * scalex;
				py *= window.invHeight * scaley;
				
				addToFluid(px, py, 0,0, true);
				
				
			}
		}
	}
		
	
	
	
	
}


// add force and dye to fluid, and create particles
void PaintScene::addToFluid(float x, float y, float dx, float dy, bool addColor, bool addForce) {
    float speed = dx * dx  + dy * dy * window.aspectRatio2;    // balance the x and y components of speed with the screen aspect ratio
	//printf("%f, %f\n", dx, dy);
    if(speed > 0) {
        if(x<0) x = 0; 
        else if(x>1) x = 1;
        if(y<0) y = 0; 
        else if(y>1) y = 1;
		
        float colorMult = 50;
        float velocityMult = 30;
		
        int index = fluidSolver.getIndexForNormalizedPosition(x, y);
		
		if(addColor) {
			msaColor drawColor;
			int hue = lroundf((x + y) * 180 + ofGetFrameNum()) % 360;
			drawColor.setHSV(hue, 1, 1);
			
			fluidSolver.r[index]  += drawColor.r * colorMult;
			fluidSolver.g[index]  += drawColor.g * colorMult;
			fluidSolver.b[index]  += drawColor.b * colorMult;
			
			//if(drawParticles) particleSystem.addParticles(x * window.width, y * window.height, 10);
		}
		
		if(addForce) {
			fluidSolver.u[index] += dx * velocityMult;
			fluidSolver.v[index] += dy * velocityMult;
		}
		
		//if(!drawFluid && ofGetFrameNum()%5 ==0) fadeToColor(0, 0, 0, 0.1);
    }
}



// ------------------------------------------------
void PaintScene::draw() {

	
	if(drawFluid) {
		glColor3f(1, 1, 1);
		fluidDrawer.draw(0, 0, window.width, window.height);
	}
	if(drawParticles) particleSystem.updateAndDraw();
	
	
	
	
	ofEnableAlphaBlending();
	glBlendFunc(GL_DST_COLOR, GL_ZERO);
	ofSetColor(255, 255, 255);
	handImageTemporallyBlurredInvert.draw(0,0,OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	ofEnableAlphaBlending();
	
	
	float scalex =  (float)OFFSCREEN_WIDTH / (float)packet.width;
	float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;
	
	
	
	ofPushStyle();
	glLineWidth(10);
	for (int i = 0; i < packet.nBlobs; i++){
		
		/*glBegin(GL_LINE_LOOP);
		 for (int j = 0; j < packet.nPts[i]; j++){
		 glVertex2f(packet.pts[i][j].x * scalex, packet.pts[i][j].y * scaley);
		 }
		 glEnd();
		 */
		
		ofSetColor(255, 255, 255, 100);
		ofNoFill();
		ofEnableSmoothing();
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < packet.nPts[i]; j++){
			glVertex2f(packet.pts[i][j].x * scalex, packet.pts[i][j].y * scaley);
		}
		glEnd();
		
		ofSetColor(255, 255, 255, 85);
		/*ofBeginShape();
		 for (int j = 1; j < packet.nPts[i]; j+=3){
		 ofCurveVertex(packet.pts[i][j].x * scalex, packet.pts[i][j].y * scaley);
		 }
		 ofEndShape(true);*/
	}
	ofPopStyle();
	

}



// ------------------------------------------------
void PaintScene::mousePressed(int wx, int wy, int x, int y, int button) {
	
	float mouseNormX = x * window.invWidth;
    float mouseNormY = y * window.invHeight;

	
	 addToFluid(mouseNormX, mouseNormY, 0.1, 0, true);
}

//--------------------------------------------------------------
void PaintScene::keyPressed(int key) {

}




static int trackerPaintCount = 0;

void PaintScene::trackBlobs(){
	
	// TRACKING
	int nBlobs = packet.nBlobs;
	bool bFoundThisFrame [nBlobs];
	for (int i = 0;  i < nBlobs; i++){
		bFoundThisFrame[i] = false;
	}
	
	for (int i = 0; i < POBJ.size(); i++){
		POBJ[i].bFoundThisFrame = false;
		POBJ[i].whoThisFrame = -1;
	}
	
	for (int i = 0; i < POBJ.size(); i++){
		
		
		float minDistance = 1000000;
		int minIndex = -1;
		
		for (int j = 0; j < nBlobs; j++){
			
			if (bFoundThisFrame[j]) continue;
			// try to find the closest "hand" to this object, withing a range
			float diffx = POBJ[i].center.x - packet.centroid[j].x;
			float diffy =  POBJ[i].center.y - packet.centroid[j].y;
			float distance = sqrt(diffx * diffx + diffy*diffy);
			
			if (minDistance > distance){
				minDistance = distance;
				minIndex = j;
			}
		}
		
		
		if (minIndex != -1 && minDistance < 200){
			bFoundThisFrame[minIndex] = true;
			// kk we are close, so let's mark and keep rolling:
			POBJ[i].center.set(packet.centroid[minIndex].x, packet.centroid[minIndex].y, 0);
			POBJ[i].bFoundThisFrame = true;
			POBJ[i].whoThisFrame = minIndex;
			POBJ[i].energy += 0.1f;
			POBJ[i].energy = MIN(POBJ[i].energy, 1);
			
			POBJ[i].trackContour(packet.pts[minIndex], packet.nPts[minIndex]);
		}
	}
	
	
	// ok for ALL non found blobs, add them to the vector
	for (int i = 0;  i < nBlobs; i++){
		if (bFoundThisFrame[i] == false){
			// make some $ in POBJ
			paintObject temp;
			POBJ.push_back( temp);
			POBJ[POBJ.size()-1].bFoundThisFrame = true;
			POBJ[POBJ.size()-1].whoThisFrame = i;
			POBJ[POBJ.size()-1].energy = 0.3;
			POBJ[POBJ.size()-1].id = trackerPaintCount;
			POBJ[POBJ.size()-1].center.x = packet.centroid[i].x;
			POBJ[POBJ.size()-1].center.y = packet.centroid[i].y;
			
			POBJ[POBJ.size()-1].trackContour(packet.pts[i], packet.nPts[i]);
			
			// let's pick a random color word:
			
			
			// this wasn't so great.  maybe if they were SUPER bright, the problem is I kind of need equality in terms of brightness
			// across the color spectrum.  so I bumped to something else....
			/*
			 float xp = (int)(ofRandom(0,1) * (neon.width-1));
			 float yp = (int)(ofRandom(0,1) * (neon.height-1));
			 int pixelPos = (yp*neon.width + xp) * 3;
			 */
			float random = ofRandom(0,1);
			if (random < 0.25){
				POBJ[POBJ.size()-1].myColor.set(255,255,0);
			} else if (random >= 0.25 && random < 0.5) {
				POBJ[POBJ.size()-1].myColor.set(255,0,255);
			} else if (random >= 0.5f && random < 0.75f){
				POBJ[POBJ.size()-1].myColor.set(0,255,255);
			} else {
				POBJ[POBJ.size()-1].myColor.set(255,255,255);
			}
			
			
			trackerPaintCount ++;
			bFoundThisFrame[i] = true;
		}
	}
	
	
	// do some other stuff, delete off old ones:
	for (int i = 0;  i < POBJ.size(); i++){
		if (POBJ[i].bFoundThisFrame == false){
			POBJ[i].energy *= 0.9f;
		}
	}
	
	std::vector<paintObject >::iterator iter = POBJ.begin();
	while (iter != POBJ.end())
	{
		if ((*iter).energy <= 0.05)
		{
			
			// seems to work!
			//printf("removing a packet with ID  %i \n", (*iter)->packetID);
			//delete (*iter);	// this should !!! free memory.  please check
			
			iter = POBJ.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}



