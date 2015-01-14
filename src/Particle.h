#pragma once

#include "ofMain.h"

class Particle {
public:

    Particle(ofVec3f _pos, ofVec3f _force);
    Particle(const Particle &p);
    void update(float timeStep);
    void updateAge();
    void addForce(ofVec3f _force);
    void resetForce();
    void updateTrails();
    void renderTrails(ofVboMesh& trails);
    void renderTrailPoints(ofVboMesh& trails);

    ofVec3f pos;        // array of position vectors
	ofVec3f vel;
	ofVec3f force;
	vector<ofVec3f> trailpos;
    float damping;
    float lifetime;
    float age;
    float ageRatio;
    bool bIsDead;
    ofVec3f scale;
    ofColor colour;

    int trailLength;
    enum TrailType {TRAIL_NONE,TRAIL_DOTS,TRAIL_TAIL};
    TrailType trailType;

protected:


};
