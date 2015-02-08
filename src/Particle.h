#pragma once

#include "ofMain.h"
class ParticleSystem;

class Particle {
public:

    Particle(ofVec3f _pos, ofVec3f _force, ParticleSystem* ps);
    //Particle(const Particle &p);
    void update(float timeStep);
    void updateAge();
    void addForce(ofVec3f _force);
    void resetForce();
    void calculatePerlin();
    void updateTrails();
    void renderTrails(ofVboMesh& trails);
    void renderTrailPoints(ofVboMesh& trails);

    ParticleSystem* ps;
    ofVec3f pos;        // array of position vectors
	ofVec3f vel;
	ofVec3f force;
	ofVec3f perlin;
	vector<ofVec3f> trailpos;
    float age;
    float ageRatio;
    bool bIsDead;
    bool bPerlin;
    bool bReturnOrigin;
    float damping;
    float lifetime;
    float scale;
    float mass;
    float forceMult;
    float perlinAmount;
    float perlinThreshold;
    ofColor colour;

    int trailLength;

    float rotation;
    int rotationDir;

    ///sprite sheet
    int spriteCount;
    int animSpeed;


protected:

};
