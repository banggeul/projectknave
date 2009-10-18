
#include "PaintScene.h"


void PaintScene::setup() {


	lastFrameRendered = -1;
	// pass in this :

	for (int i = 0; i < MAX_PARTICLES; i++){
		particleSystem.particles[i].myApp = this;
	}

	particleSystem.myApp = this;

	fluidCellsX			= 200; //200;

	// setup fluid stuff
	fluidSolver.setup(fluidCellsX, 200);
	fluidSolver.enableRGB(true).setFadeSpeed(0.0001).setDeltaT(1).setVisc(0.00035 * 3.8).setColorDiffusion(0);
	fluidDrawer.setup(&fluidSolver);

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


	paintColors.loadImage("images/Scene_03_Paint.jpg");
	paintColors.setImageType(OF_IMAGE_COLOR);
	paintColorsPixels = paintColors.getPixels();


	bAllocateDripPixels = false;

	/*


	 */

	for (int i = 0; i < 80; i++){
		ceilingDrip drip;
		drips.push_back(drip);
		drips[drips.size()-1].pctx = ofRandom(0,1);
		drips[drips.size()-1].pcty = 0.3f;
		drips[drips.size()-1].vely = ofRandom(0.02f, 0.015);
	}



}

// ------------------------------------------------
void PaintScene::update(){



	if (ofRandom(0,1) > 0.95){
		float temp = ofRandom(0,0.2);
		for (int i = 0; i < 5; i++){
			ceilingDrip drip;
			drips.push_back(drip);
			drips[drips.size()-1].pctx = ofRandom(0.4 + temp,0.42 + temp);
			drips[drips.size()-1].pcty = 0.0f;
			drips[drips.size()-1].vely = ofRandom(0.02f, 0.015);
			drips[drips.size()-1].colorMe.set(255,0,0);
		}
	}


	if(resizeFluid) 	{
		fluidSolver.setSize(fluidCellsX, fluidCellsX / window.aspectRatio);
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;

		dripWidth = fluidSolver._NX;
		dripHeight = fluidSolver._NY;
		dripImage.allocate(dripWidth, dripHeight);
		dripPixels = new unsigned char[dripWidth * dripHeight];

		memset(dripPixels, 0, dripWidth * dripHeight);
		dripImage.setFromPixels(dripPixels, dripWidth, dripHeight );
	}


	int index;
	for (int i = 0; i < drips.size(); i++){
		drips[i].pcty += drips[i].vely;
		if (drips[i].pcty > 1) drips[i].pcty = 1;
		drips[i].vely *= 0.97f;
	}


	std::vector<ceilingDrip >::iterator iter = drips.begin();
	while (iter != drips.end())
	{
		if ((*iter).vely <= 0.002)
		{

			iter = drips.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	//cout << "drips size " << drips.size() << endl;




	for (int i = 0; i < drips.size(); i++){

		if (drips[i].vely > 0.001f){
			int x1 = fluidSolver._NX * drips[i].pctx;
			int y1 = fluidSolver._NY * (drips[i].pcty - drips[i].vely);
			int y2 = fluidSolver._NY * drips[i].pcty;

			for (int j = y1; j < y2; j++){
				index =  ((x1) + (fluidSolver._NX + 2)  *(j));
				fluidSolver.r[index] = drips[i].colorMe.x;
				fluidSolver.g[index] = drips[i].colorMe.y;
				fluidSolver.b[index] = drips[i].colorMe.z;
			}
		}

	}




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




	fluidSolver.update();


	float scalex =  (float)OFFSCREEN_WIDTH / (float)packet.width;
	float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;




	for (int i = 0; i < packet.nBlobs; i++){

		// try to find my color :


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


	for (int i = 0; i < PO.COBJ.size(); i+=2){
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

				ofPoint myColor;
				myColor.set(PO.myColor.x/255.0f,PO.myColor.y/255.0f,PO.myColor.z/255.0f);
				//printf("myColor in pre fluid draw is %f %f %f \n", myColor.x, myColor.y, myColor.z);
				addToFluid(px, py, vx*0.8, vy*0.8, myColor, true);

				// love partciles here!!
				//if (i % 10 == 0) particleSystem.addParticles(px * window.width, py * window.height, 1);

			} else {

				float px = PO.COBJ[i].center.x ;
				float py = PO.COBJ[i].center.y;

				float scalex =  (float)OFFSCREEN_WIDTH / (float)packet.width;
				float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;

				px *= window.invWidth * scalex;
				py *= window.invHeight * scaley;

				ofPoint myColor;
				myColor.set(PO.myColor.x/255.0f,PO.myColor.y/255.0f,PO.myColor.z/255.0f);
				addToFluid(px, py, 0,0, myColor, true);
				//else addToFluid(px, py, 0,0, ofPoint(255,255,0), true);


			}
		}
	}





}


// add force and dye to fluid, and create particles
void PaintScene::addToFluid(float x, float y, float dx, float dy, ofPoint colorMe, bool addColor, bool addForce) {
    float speed = dx * dx  + dy * dy * window.aspectRatio2;    // balance the x and y components of speed with the screen aspect ratio
	//printf("%f, %f\n", dx, dy);
    if(speed > 0) {
        if(x<0) x = 0;
        else if(x>1) x = 1;
        if(y<0) y = 0;
        else if(y>1) y = 1;

        float colorMult = 1;
        float velocityMult = 30;

        int index = fluidSolver.getIndexForNormalizedPosition(x, y);

		if(addColor) {
			msaColor drawColor;
			int hue = lroundf((x + y) * 180 + ofGetFrameNum()) % 360;
			drawColor.setHSV(hue, 1, 1);

			/*fluidSolver.r[index]  += drawColor.r * colorMult;
			fluidSolver.g[index]  += drawColor.g * colorMult;
			fluidSolver.b[index]  += drawColor.b * colorMult;
			*/
			//cout << "in add " << colorMe.x << " " << colorMe.y << " " << colorMe.z << endl;

			fluidSolver.r[index]  =colorMe.x; //MIN(colorMe.x, fluidSolver.r[index] + colorMe.x * colorMult);
			fluidSolver.g[index]  = colorMe.y; //MIN(colorMe.y, fluidSolver.g[index] + colorMe.y * colorMult);
			fluidSolver.b[index]  = colorMe.z;//MIN(colorMe.z, fluidSolver.b[index] + colorMe.z * colorMult);


			if (ofRandom(0,1) > 0.99f){
				ceilingDrip drip;
				drips.push_back(drip);
				drips[drips.size()-1].pctx = x;
				drips[drips.size()-1].pcty = y;
				drips[drips.size()-1].vely = ofRandom(0.02f, 0.015);
				drips[drips.size()-1].colorMe.x = colorMe.x*0.5;
				drips[drips.size()-1].colorMe.y = colorMe.y*0.5;
				drips[drips.size()-1].colorMe.z = colorMe.z*0.5;
			}

			/*fluidSolver.r[index]  += colorMe.x * colorMult;
			fluidSolver.g[index]  += colorMe.y * colorMult;
			fluidSolver.b[index]  += colorMe.z * colorMult;
			*/
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

/*
	ofPushStyle();
	glLineWidth(10);
	for (int i = 0; i < packet.nBlobs; i++){

		ofSetColor(255, 255, 255, 100);
		ofNoFill();
		ofEnableSmoothing();
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < packet.nPts[i]; j++){
			glVertex2f(packet.pts[i][j].x * scalex, packet.pts[i][j].y * scaley);
		}
		glEnd();

	}
	ofPopStyle();
*/

}



// ------------------------------------------------
void PaintScene::mousePressed(int wx, int wy, int x, int y, int button) {

	float mouseNormX = x * window.invWidth;
    float mouseNormY = y * window.invHeight;


	 addToFluid(mouseNormX, mouseNormY, 0.1, 0, ofPoint(255,0,0), true);
}

//--------------------------------------------------------------
void PaintScene::keyPressed(int key) {

	/*for (int i = 0; i < 80; i++){
		ceilingDrip drip;
		drips.push_back(drip);
		drips[drips.size()-1].pctx = ofRandom(0,1);
		drips[drips.size()-1].pcty = 0.3f;
		drips[drips.size()-1].vely = ofRandom(0.02f, 0.015);
	}*/
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


		if (minIndex != -1 && minDistance < 80){
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

			 float xp = (int)(ofRandom(0,1) * (paintColors.width-1));
			 float yp = (int)(ofRandom(0,1) * (paintColors.height-1));
			 int pixelPos = (yp*paintColors.width + xp) * 3;

			//cout << (int)(paintColorsPixels[pixelPos])<<  " " << (int)(paintColorsPixels[pixelPos+1])<< " " << (int)(paintColorsPixels[pixelPos+2])<< endl;

			POBJ[POBJ.size()-1].myColor.set(   (int)(paintColorsPixels[pixelPos]), (int)(paintColorsPixels[pixelPos+1]), (int)(paintColorsPixels[pixelPos+2]));




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



