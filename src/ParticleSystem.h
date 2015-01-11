#pragma once

#include "Particle.h"

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
	void add(Particle& p);
	void update(ofCamera& cam);
	void draw();

protected:
	float timeStep;
	vector<Particle> particles;

    ofVboMesh billboards;

};
