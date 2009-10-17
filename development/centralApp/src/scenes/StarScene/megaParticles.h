#pragma once

#define NUM_PARTICLES 100000

// ----------------------- Classes for particles
class Pvert {
public:
	float x, y, z;
	Pvert() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
};
class Pcolor {
public:
	float r, g, b, a;
	Pcolor() {
		r = 0.0;
		g = 0.0;
		b = 0.0;
		a = 0.0;
	}
};

class megaParticles {
public:
	void setup() {

		// set the camera
		tranv.x = ofGetWidth() / 2;
		tranv.y = ofGetHeight() / 2;
		tranv.z = -600.0;


		// the world box size
		boxSize = 2000.0;

		// Load the particle texture
		ofDisableArbTex();
		ofImage tempImage;
		tempImage.loadImage("dot.png");
		texture.allocate(tempImage.getWidth(), tempImage.getHeight(), GL_RGBA);
		texture.loadData(tempImage.getPixels(), tempImage.getWidth(), tempImage.getHeight(), GL_RGBA);

		// Load the shader
		shader.loadShader("VBOShader");
		bShaderOn = true;
		shader.setShaderActive(bShaderOn);



		// Random Points
		for (int i = 0; i < NUM_PARTICLES; i++) {
			resetParticle(i);
		}


		// Generate the VBO
		glGenBuffersARB(2, &vbo[0]);

		// VBO for color
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, NUM_PARTICLES*4*sizeof(float), color, GL_STREAM_DRAW_ARB);

		// VBO for vertex positions
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, NUM_PARTICLES*3*sizeof(float), pnts, GL_STREAM_DRAW_ARB);

	}

	void resetParticle(int i) {
		// start in the middle
		pnts[i].x = 0;
		pnts[i].y = 0;
		pnts[i].z = 0;

		// particle velocity
		vel[i].x = ofRandom(-5.0, 5.0);
		vel[i].y = ofRandom(-5, 5.0);
		vel[i].z = ofRandom(-5.0, 5.0);

		// particle color
		color[i].r = 1.0;
		color[i].g = ofRandomf();
		color[i].b = 1.0 - ofRandomf();
		color[i].a = ofRandomf();

		// Random Point sizes
		pointSizes[i] = ofRandom(9.0, 180.0);
	}

	void update() {

		// Camera
		tran += (tranv - tran) * 0.2;
		rot  += (rotv - rot) * 0.002;



		for (int i = 0; i < NUM_PARTICLES; i++) {

			// update the postion via velocity
			pnts[i].x += vel[i].x;
			pnts[i].y += vel[i].y;
			pnts[i].z += vel[i].z;

			color[i].a -= 0.0002;


			// if we are at zero alpha
			// then reset the particle
			if (color[i].a <= 0.0) {
				resetParticle(i);
			}


			// Particle Wrapping
			if (pnts[i].x > boxSize || pnts[i].x < -boxSize ||
			    pnts[i].y > boxSize || pnts[i].y < -boxSize ||
			    pnts[i].z > boxSize || pnts[i].z < -boxSize) {
				resetParticle(i);
			}

		}

	}

	void draw() {

		// Rotate around and center
		glPushMatrix();
		glTranslatef(tran.x, tran.y, tran.z);
		glRotatef(rot.x, 1, 0, 0);
		glRotatef(rot.y, 0, 1, 0);
		glRotatef(rot.z, 0, 0, 1);


		// Draw a wire cube
		ofSetColor(100, 100, 100);
		{
			// Front
			glBegin(GL_LINE_LOOP);
			glVertex3f(-boxSize, -boxSize, boxSize);
			glVertex3f(-boxSize, boxSize, boxSize);
			glVertex3f(boxSize, boxSize, boxSize);
			glVertex3f(boxSize, -boxSize, boxSize);
			glEnd();

			// Back
			glBegin(GL_LINE_LOOP);
			glVertex3f(-boxSize, -boxSize, -boxSize);
			glVertex3f(-boxSize, boxSize, -boxSize);
			glVertex3f(boxSize, boxSize, -boxSize);
			glVertex3f(boxSize, -boxSize, -boxSize);
			glEnd();

			// Right
			glBegin(GL_LINES);
			glVertex3f(-boxSize, boxSize, boxSize);
			glVertex3f(-boxSize, boxSize, -boxSize);
			glVertex3f(-boxSize, -boxSize, boxSize);
			glVertex3f(-boxSize, -boxSize, -boxSize);
			glEnd();

			// Left
			glBegin(GL_LINES);
			glVertex3f(boxSize, boxSize, boxSize);
			glVertex3f(boxSize, boxSize, -boxSize);
			glVertex3f(boxSize, -boxSize, boxSize);
			glVertex3f(boxSize, -boxSize, -boxSize);
			glEnd();

		}


		shader.setShaderActive(true); // Turn on the Shader

		// Turn on depths
		//glEnable(GL_DEPTH_TEST);

		// Get the attribute and bind it
		GLint pixel_loc = glGetAttribLocationARB(shader.shader, "particleSize");
		glVertexAttribPointerARB(pixel_loc, 1, GL_FLOAT, false, 0, pointSizes);
		glBindAttribLocationARB(shader.shader, pixel_loc, "particleSize");
		// printf("Size Location: %i\n", pixel_loc);




		// Enable Sprites and texture replace
		glEnable(GL_POINT_SPRITE);
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);


		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);			// Enable Vertex Points
		glEnableClientState(GL_VERTEX_ARRAY);			// Enable Vertex
		glEnableClientState(GL_COLOR_ARRAY);			// Enable Color



		// Color
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);	// Bind the color VBO
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, NUM_PARTICLES * 4 * sizeof(float), color);
		glColorPointer(4, GL_FLOAT, 0, 0);


		// Pnts
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);	// Bind the pnts VBO
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, NUM_PARTICLES * 3 * sizeof(float), pnts);
		glVertexPointer(3, GL_FLOAT, 0, 0);


		// Enable the PixelSize Attribute
		glEnableVertexAttribArrayARB(pixel_loc);



		ofEnableAlphaBlending();

		// Additive Blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		//	shader.setUniformVariable3f("theColor", 1.0, 1.0, 0.0);	// Set Shader to Yellow
		texture.bind();
		glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);				// Draw Just Points
		texture.unbind();
		//shader.setUniformVariable3f("theColor", 1.0, 1.0, 1.0);	// Set Shader to White
		ofSetColor(255, 255, 255);
		//glDrawArrays(GL_LINES, 0, NUM_PARTICLES);				// Draw Lines
		ofDisableAlphaBlending();


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
	}

	// Camera
	ofxVec3f tran, tranv;
	ofxVec3f rot, rotv;


	// Color and Vertex VBO
	GLuint	 vbo[2];

	float   boxSize;

	Pvert	vel[NUM_PARTICLES];
	Pvert	pnts[NUM_PARTICLES];
	Pcolor	color[NUM_PARTICLES];

	float pointSizes[NUM_PARTICLES];


	ofTexture	texture;
	ofxShader	shader;
	bool		bShaderOn;

};
