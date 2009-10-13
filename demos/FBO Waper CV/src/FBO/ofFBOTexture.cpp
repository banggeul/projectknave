/*
 *  ofFBOTexture.cpp
 *  openFrameworks
 *
 *  Created by Zach Gage on 3/28/08.
 *  Copyright 2008 STFJ.NET. All rights reserved.
 *
 */

#include "ofFBOTexture.h"

void ofFBOTexture::allocate(int w, int h, bool autoC)
{
    width = w;
    height = h;

	//-------------ofTexture-------------
	// 	can pass in anything (320x240) (10x5)
	// 	here we make it power of 2 for opengl (512x256), (16x8)

    if (GLEE_ARB_texture_rectangle){
        tex_w = w;
        tex_h = h;
        textureTarget = GL_TEXTURE_RECTANGLE_ARB;   // zach  -- added to get texture target right
    } else {
        tex_w = ofNextPow2(w);
        tex_h = ofNextPow2(h);
    }

	if (GLEE_ARB_texture_rectangle){
		tex_t = w;
		tex_u = h;
	} else {
		tex_t = 1.0f;
		tex_u = 1.0f;
	}

	// attempt to free the previous bound texture, if we can:
	clean();

	width = w;
	height = h;
	bFlipTexture = true;

	//--FBOTexture-------------------
	autoClear = autoC;
	// Setup our FBO
	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

	// Create the render buffer for depth
	glGenRenderbuffersEXT(1, &depthBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, tex_w, tex_h);

	// Now setup a texture to render to
	glGenTextures(1, (GLuint *)textureName);   // could be more then one, but for now, just one

	glEnable(textureTarget);

	glBindTexture(textureTarget, (GLuint)textureName[0]);
		glTexImage2D(textureTarget, 0, GL_RGBA, tex_w, tex_h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);  // init to black...
		glTexParameterf(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// attach it to the FBO so we can render to it

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, textureTarget, (GLuint)textureName[0], 0);


	// Attach the depth render buffer to the FBO as it's depth attachment
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);


	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		cout<<"glBufferTexture failed to initialize. Perhaps your graphics card doesnt support the framebuffer extension? If you are running osx prior to system 10.5, that could be the cause"<<endl;
		std::exit(1);
	}
	clear();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);	// Unbind the FBO for now

	glDisable(textureTarget);
}


void ofFBOTexture::setupScreenForMe(){

	int w = width;
	int h = height;

	float halfFov, theTan, screenFov, aspect;
	screenFov 		= 60.0f;

	float eyeX 		= (float)w / 2.0;
	float eyeY 		= (float)h / 2.0;
	halfFov 		= PI * screenFov / 360.0;
	theTan 			= tanf(halfFov);
	float dist 		= eyeY / theTan;
	float nearDist 	= dist / 10.0;	// near / far clip plane
	float farDist 	= dist * 10.0;
	aspect 			= (float)w/(float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(screenFov, aspect, nearDist, farDist);
	gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
  	glTranslatef(0, -h, 0);       // shift origin up to upper-left corner.

    glViewport(0,0,width, height);

}

void ofFBOTexture::setupScreenForThem(){

    int w, h;

	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);

	float halfFov, theTan, screenFov, aspect;
	screenFov 		= 60.0f;

	float eyeX 		= (float)w / 2.0;
	float eyeY 		= (float)h / 2.0;
	halfFov 		= PI * screenFov / 360.0;
	theTan 			= tanf(halfFov);
	float dist 		= eyeY / theTan;
	float nearDist 	= dist / 10.0;	// near / far clip plane
	float farDist 	= dist * 10.0;
	aspect 			= (float)w/(float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(screenFov, aspect, nearDist, farDist);
	gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
  	glTranslatef(0, -h, 0);       // shift origin up to upper-left corner.


    glViewport(0,0,w, h);
}

void ofFBOTexture::swapIn()
{

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); // bind the FBO to the screen so we can draw to it

	if(autoClear)
	{
		clear();
	}

	// Save the view port and set it to the size of the texture
}

void ofFBOTexture::swapOut()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); //unbind the FBO

}

void ofFBOTexture::clean()
{
	// try to free up the texture memory so we don't reallocate
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/deletetextures.html
	if (textureName[0] != 0){
		glDeleteTextures(1, (GLuint *)textureName);
	}
	width = 0;
	height = 0;
	bFlipTexture = false;
}

void ofFBOTexture::clear()
{
	glClearColor(1,1,1,0); //clear white
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
}
