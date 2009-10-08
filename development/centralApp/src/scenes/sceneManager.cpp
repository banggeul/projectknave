/*
 *  sceneManager.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "sceneManager.h"


void sceneManager::setup(){
	
	
	currentScene	= 0;
	numScenes		= 5;
	
	scenes			= new baseScene*[numScenes];
	
	scenes[0]		= new HandScene();				// OOP love it. 
	scenes[1]		= new TreeScene();
	scenes[2]		= new HandScene();
	scenes[3]		= new HandScene();
	scenes[4]		= new HandScene();
	
	
	for (int i = 0; i < numScenes; i++){
		scenes[i]->setup();
	}
}



void sceneManager::nextScene(){
	currentScene++;
	if (currentScene >= numScenes){
		currentScene = 0;
	}
}


void sceneManager::prevScene(){
	currentScene--;
	if (currentScene < 0){
		currentScene += numScenes;
	}
}

void sceneManager::passInPacket(computerVisionPacket * packet){
	memcpy(&(scenes[currentScene]->packet), packet, sizeof(computerVisionPacket));
}



void sceneManager::update(){
	scenes[currentScene]->update();
}

void sceneManager::draw(){
	scenes[currentScene]->draw();
}
void sceneManager::gotoScene(int theScene) {
	if(theScene < 0) theScene = 0;
	if(theScene > numScenes) theScene = numScenes;
	currentScene = theScene;	
}


// --------------------------------------------------- keyboard and mouse
void sceneManager::keyPressed(int key) {
	scenes[currentScene]->keyPressed(key);
}
void sceneManager::keyReleased(int key) {
	scenes[currentScene]->keyReleased(key);
}
void sceneManager::mouseMoved(int x, int y) {
	scenes[currentScene]->mouseMoved(x, y);
}
void sceneManager::mouseDragged(int x, int y, int button) {
	scenes[currentScene]->mouseDragged(x, y, button);
}
void sceneManager::mousePressed(int x, int y, int button) {
	scenes[currentScene]->mousePressed(x, y, button);
}
void sceneManager::mouseReleased(int x, int y, int button) {
	scenes[currentScene]->mouseReleased(x, y, button);
}
	














