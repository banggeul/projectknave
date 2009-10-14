
#include "FerryBuilding.h"



// ------------------------------------ setup
void FerryBuilding::setupBuilding(string fileToLoad) {
	
	selectedFile = 0;
	fileCount	 = 0;
	
	bSaved		 = true;
	bFirstPoint  = true;
	bEnable		 = false;
	
	file = fileToLoad;
	loadBuilding(fileToLoad);
	/*
	// list all files for buildings
	fileLister.allowExt("xml");
	int numFiles = fileLister.listDir(fileToLoad);
	if(numFiles > 0) {
		for(int i=0; i<numFiles; i++) {
			files.push_back(fileLister.getName(i));
			fileCount ++;
		}
		selectedFile = 0;
		
		
		// load the building from xml
		loadBuilding(files[0]);
	}
	*/
	
}


// ------------------------------------ load a building file
void FerryBuilding::loadBuilding(string theFile) {
	
	printf("--- loading: %s ---\n", theFile.c_str());
	
	clear();
	
	if(xmlSaver.loadFile(theFile)) {
		
		int numTags = xmlSaver.getNumTags("building:shape");
		if(numTags > 0) {
			
			xmlSaver.pushTag("building");
			
			int numShapes = xmlSaver.getNumTags("shape:pnt");
			for(int i=0; i<numShapes; i++) {
				
				xmlSaver.pushTag("shape", i);
				shapes.push_back(FerryShape());
				int numPnts = xmlSaver.getNumTags("pnt");
				for(int j=0; j<numPnts; j++) {
					
					float x = xmlSaver.getValue("pnt:x", 0.0, j);
					float y = xmlSaver.getValue("pnt:y", 0.0, j);
					shapes.back().pnts.push_back(ofPoint(x,y,0));
				}
				
				xmlSaver.popTag();
				
			}			
			xmlSaver.popTag();
		}
		
	}
	
}

// ------------------------------------ save building
void FerryBuilding::saveBuilding() {
	
	string fileToSave = file;//"building_"+ofToString(selectedFile)+".xml";
	
	xmlSaver.clear();
	
	xmlSaver.addTag("building");
	xmlSaver.pushTag("building");
	for(int i=0; i<shapes.size(); i++) {
		xmlSaver.addTag("shape");
		xmlSaver.pushTag("shape", i);
		for(int j=0; j<shapes[i].pnts.size(); j++) {
			int ptnum = xmlSaver.addTag("pnt");
			xmlSaver.setValue("pnt:x", shapes[i].pnts[j].x, ptnum);
			xmlSaver.setValue("pnt:y", shapes[i].pnts[j].y, ptnum);
		}
		xmlSaver.popTag();
	}
	
	xmlSaver.popTag();
	xmlSaver.saveFile(fileToSave);
	bSaved = true;
	
	printf("--- building saved [%s]---\n", file.c_str());
}

// ------------------------------------ draw
void FerryBuilding::draw() {
	
	ofSetColor(0xffffff);
	//mask.draw(0, 0);	
	//building.draw(0, 0);
}

// ------------------------------------ Draw the building contour
void FerryBuilding::drawContour() {
	
	glLineWidth(2.0);
	for(int i=0; i<shapes.size(); i++) {
		ofNoFill();
		ofSetColor(255, 0, 255);		
		ofBeginShape();
		for(int j=0; j<shapes[i].pnts.size(); j++) {
			ofVertex(shapes[i].pnts[j].x, shapes[i].pnts[j].y);
		}
		ofEndShape(false);
	}
	glLineWidth(1.0);
	
}

// ------------------------------------ draw info
void FerryBuilding::drawInfo() {
	
	ofSetColor(255, 0, 0);
	string info		 = "";
	string filesInfo = "";
	
	if(bEnable) {
		info = "";
		info = "		FPS: "+ofToString(ofGetFrameRate())+"\n";
		info += "\n	Click to draw building contour";
		info += "\n	[e] toggle on and off contours";
		info += "\n	[Space] to add new shape";
		info += "\n	[c] to clear the building";
		info += "\n	[s] to save the building to xml";
		info += "\n	[z] to undo last click";
		info += "\n\nWhen adding points always make\nsure to add counter clockwise.";

		if(!bSaved) info += "\n*** building not saved ***";
		if(bEnable) info +="\nGo ahead and add points.";
		
		
		
		/*
		filesInfo = "\n\nAdd a new file [n]";
		for(int i=0; i<files.size(); i++) {
			filesInfo += "\nFile: "+files[i];
			if(selectedFile == i) {
				filesInfo += "  <---";	
			}
		}
		*/
	}
	else {
		info = "To enable drawing press [e]";	
	}
	
	ofDrawBitmapString(info, 20, 15);
	//ofDrawBitmapString(filesInfo, 350, 15);
	
	
}

// ------------------------------------ clear
void FerryBuilding::clear() {
	for(int i=0; i<shapes.size(); i++) {
		shapes[i].pnts.clear();
	}
	shapes.clear();
}

// ------------------------------------ mouse/keyboard
void FerryBuilding::keyPressed(int key) {
	if(bEnable) {
		
		if(key == ' ') { 
			bSaved		= false;
			bFirstPoint = false;
			shapes.push_back(FerryShape());
		}
		if(key == 'c') clear();
		if(key == 's') saveBuilding();
		
		// undo the last click
		if(key == 'z') {
			if(shapes.size() > 0) {
				// no points left in shape then erase shape
				if(shapes.back().pnts.size() == 0) {
					int lastShape = shapes.size();
					shapes.erase(shapes.begin() + lastShape);
				} else {
					int lastPnt = shapes.back().pnts.size();
					shapes.back().pnts.erase(shapes.back().pnts.begin() + lastPnt);	
				}
			}
		}
		
		/*
		// add a new file
		if(key == 'n') {
			fileCount ++;
			selectedFile = fileCount-1;
			string fileToSave = "building_"+ofToString(selectedFile)+".xml";
			files.push_back(fileToSave);
			loadBuilding(files[selectedFile]);

		}
		
		
		// select the file
		if(key == OF_KEY_DOWN) {
			selectedFile ++;
			if(selectedFile > (files.size()-1)) {
				selectedFile = files.size()-1;
			}
			loadBuilding(files[selectedFile]);
		}
		if(key == OF_KEY_UP) {
			selectedFile --;
			if(selectedFile < 0) {
				selectedFile = 0;
			}
			loadBuilding(files[selectedFile]);
		}
		*/
		
	}
	
	// need to turn me on first
	if(key == 'e') bEnable = !bEnable;
	
}

// ------------------------------------ mouse pressed
void FerryBuilding::mousePressed(int x, int y, int button) {
	if(bEnable) {
		
		bSaved = false;
		
		// we need one to start
		if(shapes.size() == 0 || bFirstPoint) {
			shapes.push_back(FerryShape());
			shapes.back().pnts.push_back(ofPoint(x,y,0));
			bFirstPoint = false;
		}
		
		shapes.back().pnts.push_back(ofPoint(x,y,0));
	}
	
}



