#pragma once

#include "ofMain.h"
#include "ofxVectorMath.h"

#include "starParticle.h"

class groupableParticle : public starParticle {

	public:

        groupableParticle ();

        void update();

        bool            bBeenChosen;
        float           dragTarget;
        bool            bFrozen;
};





