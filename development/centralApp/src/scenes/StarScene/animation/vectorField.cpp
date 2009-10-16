

#include "vectorField.h"



//-------------------------------------------------
vectorField::vectorField(){
    binW = 1.0/NUM_BINS_X;
    binH = 1.0/NUM_BINS_Y;
}

//-------------------------------------------------
void vectorField::addIntoField(float x, float y, ofxVec2f vec, float radius = 1.0){

    if(x >= 1.0 || x < 0 || y >= 1.0 || y < 0){
         return;
    }

    ofxVec2f pos(x,y);
    ofxVec2f raster;

    raster.x = pos.x;
    raster.y = pos.y;

    float startX = raster.x - radius;
    float startY = raster.y- radius;

    float endX = raster.x + radius;
    float endY = raster.y + radius;

    int sx = ((int)(startX * NUM_BINS_X))-1;
    int sy = ((int)(startY * NUM_BINS_Y))-1;

    int ex = ((int)(endX * NUM_BINS_X))+1;
    int ey = ((int)(endY * NUM_BINS_Y))+1;

    sx  = MAX(sx, 0);
    sy  = MAX(sy, 0);
    ex  = MAX(ex, 0);
    ey  = MAX(ey, 0);

    sx  = MIN(sx, NUM_BINS_X);
    sy  = MIN(sy, NUM_BINS_Y);
    ex  = MIN(ex, NUM_BINS_X);
    ey  = MIN(ey, NUM_BINS_Y);

    ofxVec2f binPos;
    ofxVec2f distVec;
    float dist = 0;
    float amnt = 0;

    for(int yy = sy; yy < ey; yy++){
        for(int xx = sx; xx < ex; xx++){

            binPos.set( (float)xx * binW, (float)yy * binH);
            distVec = binPos - pos;
            dist = distVec.length();

            if(dist > radius)continue;

            amnt = 1.0 - (dist - radius);

            //amnt * amnt makes the rolloff less linear
            field[xx][yy] += vec*amnt*amnt;
        }
    }
}

//-------------------------------------------------
void vectorField::blur(float amnt){

    if(amnt > 1.0)amnt = 1.0;
    else if(amnt < 0.0)amnt = 0.0;

    ofxVec2f blur;

    float ramnt = (1.0 - amnt) * 0.25;

    for(int yy = 1; yy < NUM_BINS_Y-1; yy++){
        for(int xx = 1; xx < NUM_BINS_X-1; xx++){

            blur = field[xx][yy] * amnt + field[xx+1][yy] * ramnt +  field[xx-1][yy] * ramnt + field[xx][yy+1] * ramnt + field[xx][yy-1] * ramnt;
            field[xx][yy] = blur;
        }
    }
}


//-------------------------------------------------
ofxVec2f vectorField::readFromField(float x, float y){
    if(x >= 1.0 || x < 0 || y >= 1.0 || y < 0){
         return ofxVec2f();
    }
    int rasterX = (int)(x * NUM_BINS_X);
    int rasterY = (int)(y * NUM_BINS_Y);
    ofxVec2f vec = field[rasterX][rasterY];
    return vec;
}


//-------------------------------------------------
void vectorField::normalize(){

    for(int yy = 0; yy < NUM_BINS_Y; yy++){
        for(int xx = 0; xx < NUM_BINS_X; xx++){
            field[xx][yy].normalize();
        }
    }
}

//-------------------------------------------------
void vectorField::randomize(float mag, float pct){

    pct = MIN(1.0, pct);
    pct = MAX(0.0, pct);

    for(int yy = 0; yy < NUM_BINS_Y; yy++){
        for(int xx = 0; xx < NUM_BINS_X; xx++){
            field[xx][yy] *= 1.0 - pct;
            field[xx][yy] += ofxVec2f( ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0)) * mag * pct;
        }
    }
}

//-------------------------------------------------
void vectorField::clear(){

    for(int yy = 0; yy < NUM_BINS_Y; yy++){
        for(int xx = 0; xx < NUM_BINS_X; xx++){
            field[xx][yy].set(0,0);
        }
    }
}

//-------------------------------------------------
void vectorField::fade(float pct){

    for(int yy = 0; yy < NUM_BINS_Y; yy++){
        for(int xx = 0; xx < NUM_BINS_X; xx++){
            field[xx][yy] *= pct;
        }
    }
}


//-------------------------------------------------
void vectorField::draw(int x, int y, float width, float height, float scale = 1.0){

    ofxVec2f pixelPos;
    ofxVec2f vec;

    glPushMatrix();
        glTranslatef(x, y, 0);

        for(int yy = 0; yy < NUM_BINS_Y; yy++){
            for(int xx = 0; xx < NUM_BINS_X; xx++){

                pixelPos.set( (float)xx * binW * width, (float)yy * binH * height );
                vec = (field[xx][yy]) * scale;


                glBegin(GL_LINES);
                    ofSetColor(0x000044);
                    glVertex2f(pixelPos.x, pixelPos.y);
                    ofSetColor(0xFFFF33);
                    glVertex2f(pixelPos.x + vec.x, pixelPos.y + vec.y);
                glEnd();

                //ofCircle(pixelPos.x, pixelPos.y, 2);
            }
        }

    glPopMatrix();
}

