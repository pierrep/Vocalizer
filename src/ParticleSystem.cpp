#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() :
    timeStep(0.5f)
{
    billboards.getVertices().reserve(1000);
    billboards.getColors().reserve(1000);
    billboards.getNormals().reserve(1000);

   	billboards.setUsage( GL_DYNAMIC_DRAW );
	billboards.setMode(OF_PRIMITIVE_POINTS);

}

void ParticleSystem::setTimeStep(float _timeStep)
{
	timeStep = _timeStep;
}

void ParticleSystem::add(Particle& p) {
	particles.push_back(p);

	int kNumParticles = particles.size();
	//cout << "num particles=" << kNumParticles << endl;
    billboards.getVertices().resize(kNumParticles);
	billboards.getColors().resize(kNumParticles);
	billboards.getNormals().resize(kNumParticles,ofVec3f(0));

	billboards.getColors()[kNumParticles-1].set(particles[kNumParticles-1].colour);
	billboards.setNormal(kNumParticles-1,particles[kNumParticles-1].scale);
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

//	list< pair<int, float> > depthList;
//
//	// put indexed points and z-values into the list
//	for(int i = 0; i < particles.size(); i++) {
//		depthList.push_back( make_pair(i, particles[i].pos.z) );
//	}
//
//	// sort the list
//	depthList.sort(DepthSortPredicate);
//
//	// iterate through list
//	std::list<pair<int, float> >::iterator it;
	int j = 0;
//    for(it = depthList.begin(); it != depthList.end(); it++) {

	for(int i = 0; i < particles.size(); i++) {
	//int i = it->first;
		particles[i].update(timeStep);


        billboards.getVertices()[j] = particles[i].pos;
        billboards.getColors()[j] = particles[i].colour;
        billboards.setNormal(j,particles[i].scale);

        if(particles[i].vel.length() > 1) {
            Particle p = particles[i];
            p.force = ofVec3f(0,0,0);
            p.scale = ofVec3f(5,0,0);
            add(p);
        }

		if(particles[i].lifetime > 150) {
            particles[i].colour.a *= 0.995f;
            if(particles[i].colour.a < 0.1f) {
            ///kill particle
            particles.erase(particles.begin()+i);
            int kNumParticles = particles.size();
            billboards.getVertices().resize(kNumParticles);
            billboards.getColors().resize(kNumParticles);
            billboards.getNormals().resize(kNumParticles,ofVec3f(0));
            }
		}
		j++;
	}

}

void ParticleSystem::draw() {

//	for(int i = 0; i < particles.size(); i++) {
//		particles[i].draw();
//	}
	billboards.draw();

}
