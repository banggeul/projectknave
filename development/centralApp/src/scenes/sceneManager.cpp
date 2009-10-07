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
	numScenes		= 4;
	
	scenes			= new baseScene*[numScenes];
	
	scenes[0]		= new BodyScene();				// OOP love it. 
	scenes[1]		= new MonsterScene()
	scenes[2]		= new StarScene();
	scenes[3]		= new TreeScene();
	
	
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

void sceneManager::passInPacket(computerVisionPacket * packet){
	memcpy(&(scenes[currentScene]->packet), packet, sizeof(computerVisionPacket));
}



void sceneManager::update(){
	scenes[currentScene]->update();
}

void sceneManager::draw(){
	scenes[currentScene]->draw();
}


	