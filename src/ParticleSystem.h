#pragma once

#include "Particle.h"
#include "ofxTween.h"

namespace {
    inline bool DepthSortPredicate(const pair<int, float>& lhs, const pair<int, float>& rhs) {return lhs.second > rhs.second;}
}

class ParticleSystem {

public:
    enum TrailType {TRAIL_NONE = 0,TRAIL_LINE,TRAIL_TAIL,TRAIL_QUADS};

	ParticleSystem();
	unsigned size() const;
	Particle& operator[](unsigned i);

	void resetForces();
	void setTimeStep(float timeStep);
    void setTrailType(TrailType _ttype);
    void setSheetWidth(int s_width);
	void addParticle(float force, float spectrum);
	void addParticle(Particle& p);
	void update(ofCamera& cam);
	void draw();
	void renderTrails();
	void depthSort(ofCamera& cam);
	void noDepthSort(ofCamera& cam);
	void eraseParticle(int i);
	void loadResources();
	vector<Particle>& getParticles() {return particles;}
    int getParticleRate() {return particleRate; }
    void setParticleRate(int rate) {particleRate = rate; }
    
    ofVboMesh billboards;

    /* Parameters */
    float spriteDamping;
    float spriteLifetime;
    float spriteScale;
    float spriteMass;
    float forceMultiplier;
    ofColor spriteColour;
    float spriteInitialRotation;
    int spriteRotationDir;
    int spriteSheetWidth;
    bool returnToOrigin;
    float perlinAmount;
    float perlinThreshold;
    float spriteAnimationSpeed;
    bool perlinNoise;
    TrailType trailType;
    float trailLength;
    ofFloatColor trailStartColour;
    ofFloatColor trailEndColour;
    bool trailTaperWidth;
    float trailWidth;
    string spriteName;

protected:
    friend Particle;

	float timeStep;
	vector<Particle> particles;

    ofShader    billboardShader;
    ofShader    spriteShader;
    ofShader    trailShader;
    ofShader    trailBillboardShader;

    ofImage     sprite;
    ofImage     spriteTrail;
    ofVboMesh   trails;
    vector<float> rotations; ///rotation attributes

    /* sprite sheets */
    int         totalSprites;
    int         particleRate;


};
