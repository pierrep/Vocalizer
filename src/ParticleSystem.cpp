#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
   timeStep = 0.5f;
}

void ParticleSystem::setTimeStep(float _timeStep)
{
	timeStep = _timeStep;
}

void ParticleSystem::add(Particle& p) {
	particles.push_back(p);
}

unsigned ParticleSystem::size() const {
	return particles.size();
}

Particle& ParticleSystem::operator[](unsigned i) {
	return particles[i];
}



void ParticleSystem::resetForces() {
	int n = particles.size();
	for(int i = 0; i < n; i++)
		particles[i].resetForce();
}



void ParticleSystem::update() {

	for(int i = 0; i < particles.size(); i++) {
		particles[i].update(timeStep);
		if(particles[i].lifetime > 600) {
            ///kill particle
            particles.erase(particles.begin()+i);
		}
	}

}

void ParticleSystem::draw() {

//	glEnable(GL_POINT_SIZE);
//	glPointSize(20);
//
//	glBegin(GL_POINTS);
//	ofPushStyle();
//	ofEnableBlendMode(OF_BLENDMODE_ADD);

	for(int i = 0; i < particles.size(); i++) {
		particles[i].draw();
	}
//	ofDisableBlendMode();
//	ofPopStyle();
//	glEnd();
//
//	glDisable(GL_POINT_SIZE);
}
