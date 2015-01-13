#pragma once

#include "Particle.h"
#include "ofxTween.h"

namespace {
    inline bool DepthSortPredicate(const pair<int, float>& lhs, const pair<int, float>& rhs) {return lhs.second > rhs.second;}
}

class ParticleSystem {

public:
	ParticleSystem();
	unsigned size() const;
	Particle& operator[](unsigned i);

	void resetForces();
	void setTimeStep(float timeStep);
	void addParticle(float force);
	void addParticle(Particle& p);
	void update(ofCamera& cam);
	void draw();
	void renderTrails();
	void depthSort(ofCamera& cam);
	void eraseParticle(int i);

protected:
	float timeStep;
	vector<Particle> particles;

    ofVboMesh billboards;
    ofShader    billboardShader;
    ofImage     sprite;
    ofVboMesh   trails;
};
