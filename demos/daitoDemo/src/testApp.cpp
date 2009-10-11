#include "testApp.h"

void testApp::setup(){
	ofxDaito::setup("daito.xml");
}

void testApp::update(){
}

void testApp::draw(){
}

void testApp::keyPressed  (int key){
	switch(key) {
		case '1': ofxDaito::start(1); break;
		case '2': ofxDaito::start(2); break;
		case 'q': ofxDaito::stop(1); break;
		case 'w': ofxDaito::stop(2); break;
		case 'a': ofxDaito::bang(1, "growth", .8, .1); break;
		case 's': ofxDaito::bang(2, "death", .7, .9); break;
	}
}
