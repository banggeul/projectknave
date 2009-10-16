

#include "groupableParticle.h"

//------------------------------------------------------------------------
groupableParticle::groupableParticle(){
    bBeenChosen     = false;
    dragTarget      = 0.94f;
    bFrozen         = false;
    starParticle::starParticle();
}

//------------------------------------------------------------------------
void groupableParticle::update() {
    if (dragTarget < drag){
        drag = 0.98f * drag + 0.02f * dragTarget;
    } else {
        drag = 0.0f * drag + 1.0f * dragTarget;
    }

    if (bFrozen) return;
    starParticle::update();
};
