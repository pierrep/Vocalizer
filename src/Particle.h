#pragma once

#include "ofMain.h"

class Particle {
public:

    Particle(ofVec3f _pos, ofVec3f _force);
    void update(float timeStep);
    void addForce(ofVec3f _force);
    void resetForce();
    void draw();

    ofVec3f pos;
	ofVec3f vel;
	ofVec3f force;
    float damping;
    float lifetime;
    ofColor colour;

protected:


};
