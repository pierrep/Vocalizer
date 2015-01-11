#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() :
    timeStep(0.5f)
{
//    billboards.getVertices().reserve(1000);
//    billboards.getColors().reserve(1000);
//    billboards.getNormals().reserve(1000);

   	billboards.setUsage( GL_DYNAMIC_DRAW );
	billboards.setMode(OF_PRIMITIVE_POINTS);

}

void ParticleSystem::setTimeStep(float _timeStep)
{
	timeStep = _timeStep;
}

void ParticleSystem::addParticle(float force) {
    float kGamma = 100.0f;
    Particle p(ofVec3f::zero(),ofVec3f(ofRandomf()*kGamma*force, ofRandomf()*kGamma*force, ofRandomf()*kGamma*force) );

    addParticle(p);
}

void ParticleSystem::addParticle(Particle& p) {

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



void ParticleSystem::update(ofCamera& cam) {
	for(unsigned int i = 0; i < particles.size(); i++) {
		if(particles[i].lifetime > 150) {
            particles[i].addForce((ofVec3f::zero() - particles[i].pos[0]) / 20.0f);
            particles[i].scale *= 0.99f;
            //particles[i].addForce(ofVec3f::zero());

            //particles[i].colour.a *= 0.995f;
            //if(particles[i].colour.a < 0.1f)
            if(particles[i].pos[0].distanceSquared(ofVec3f(0,0,0)) < 1.0f)
            {
                ///kill particle
                particles.erase(particles.begin()+i);
                int kNumParticles = particles.size();
                billboards.getVertices().resize(kNumParticles);
                billboards.getColors().resize(kNumParticles);
                billboards.getNormals().resize(kNumParticles,ofVec3f(0));
            }
		}
	}

    /* depth sorting */
	list< pair<int, float> > depthList;

	// put indexed points and z-values into the list
	for(unsigned int i = 0; i < particles.size(); i++) {
		depthList.push_back( make_pair(i, ofVec3f(particles[i].pos[0] - cam.getPosition()).length() ));
	}

	// sort the list
	depthList.sort(DepthSortPredicate);

	/// iterate through list
	std::list<pair<int, float> >::iterator it;
	int j = 0;
    for(it = depthList.begin(); it != depthList.end(); it++) {


	int i = it->first;
		particles[i].update(timeStep);


        billboards.getVertices()[j] = particles[i].pos[0];
        billboards.getColors()[j] = particles[i].colour;
        billboards.setNormal(j,particles[i].scale);

//        if((particles[j].vel.length() > 1) && (particles[j].lifetime < 150)) {
//            Particle p = particles[j];
//            p.force = ofVec3f(0,0,0);
//            p.scale = ofVec3f(5,0,0);
//            addParticle(p);
//        }
		j++;
	}

}

void ParticleSystem::draw() {

//	for(int i = 0; i < particles.size(); i++) {
//		particles[i].draw();
//	}
	billboards.draw();

}
