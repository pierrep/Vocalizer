#pragma once

#include "Particle.h"

class ParticleSystem {
protected:
	float timeStep;
	vector<Particle> particles;


public:
	ParticleSystem();

	void setTimeStep(float timeStep);

	void add(Particle& p);
	unsigned size() const;
	Particle& operator[](unsigned i);

	void resetForces();

	void update();
	void draw();
};
