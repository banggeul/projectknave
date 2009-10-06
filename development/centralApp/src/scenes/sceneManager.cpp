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
	numScenes		= 1;
	
	scenes = new baseScene*[numScenes];
	scenes[0] = new simpleScene();				// OOP love it. 
	
	
	for (int i = 0; i < numScenes; i++){
		scenes[i]->setup();
	}
}



void sceneManager::nextScene(){
	currentScene++;
	if (currentScene > numScenes){
		currentScene = 0;
	}
}


void sceneManager::prevScene(){
	currentScene--;
	if (currentScene < 0){
		currentScene += numScenes;
	}
}



void sceneManager::update(){
	scenes[currentScene]->update();
}

void sceneManager::draw(){
	scenes[currentScene]->draw();
}


	