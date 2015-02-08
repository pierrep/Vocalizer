#pragma once

#include "Particle.h"
#include "ofxTween.h"

namespace {
    inline bool DepthSortPredicate(const pair<int, float>& lhs, const pair<int, float>& rhs) {return lhs.second > rhs.second;}
}

class ParticleSystem {

public:
    enum TrailType {TRAIL_NONE,TRAIL_LINE,TRAIL_TAIL,TRAIL_QUADS};

	ParticleSystem();
	unsigned size() const;
	Particle& operator[](unsigned i);

	void resetForces();
	void setTimeStep(float timeStep);
    void setTrailType(TrailType _ttype);
	void addParticle(float force, float spectrum);
	void addParticle(Particle& p);
	void update(ofCamera& cam);
	void draw();
	void renderTrails();
	void depthSort(ofCamera& cam);
	void noDepthSort(ofCamera& cam);
	void eraseParticle(int i);
	vector<Particle>& getParticles() {return particles;}

    ofVboMesh billboards;

protected:
    friend Particle;

	float timeStep;
	vector<Particle> particles;
    TrailType   trailType;


    ofShader    billboardShader;
    ofShader    trailShader;
    ofImage     sprite;
    ofImage     spriteTrail;
    ofVboMesh   trails;

    /* sprite sheets */
    int         spriteNum;
};
