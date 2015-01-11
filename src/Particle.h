#pragma once

#include "ofMain.h"

class Particle {
public:

    Particle(ofVec3f _pos, ofVec3f _force);
    Particle(const Particle &p);
    void update(float timeStep);
    void addForce(ofVec3f _force);
    void resetForce();

    vector<ofVec3f> pos;        // array of position vectors
	ofVec3f vel;
	ofVec3f force;
    float damping;
    float lifetime;
    ofVec3f scale;
    ofColor colour;

    int trailLength;

protected:


};
