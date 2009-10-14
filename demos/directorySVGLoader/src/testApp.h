#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxSvg.h"
#include "ofxDirList.h"

class SVGDirectoryLoader{
public:
	SVGDirectoryLoader(){
		loaded = false;
		directoryLocation = "";
	}
	
	void load(string aDirectoryLocation){
		loaded = false;
		directoryLocation = aDirectoryLocation;
		
		dirLister.allowExt("svg");
		int nSvgs = dirLister.listDir(directoryLocation);
		
		svgs.resize(nSvgs);
		
		//you can now iterate through the files as you like
		for(int i = 0; i < svgs.size(); i++){
			svgs[i].loadFile(dirLister.getPath(i));
			svgs[i].setUseDisplayList(false);
		}
	
		loaded = true;
	}
	
	
	ofxSvg& getSvg(int i){
		return svgs[i];
	}
	ofxSvg& getRandomSvg(){
		int randomIndex = ofRandom(0, svgs.size()-1);
		
		cout << "Random index " << randomIndex << "out of " << svgs.size() << endl;
		
		return svgs[randomIndex];	
	}
	bool isLoaded(){
		return loaded;
	}
private:
	string			directoryLocation;
	ofxDirList		dirLister;
	vector <ofxSvg>	svgs;
	bool			loaded;
};


class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void resized(int w, int h);

private:
	SVGDirectoryLoader eyes;
	SVGDirectoryLoader confetti;
	SVGDirectoryLoader butterflies;
};

#endif
