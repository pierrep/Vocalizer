#include "Particle.h"

Particle::Particle(ofVec3f _pos, ofVec3f _force)
{
    pos.set(_pos);
	force.set(_force);
	damping = 0.2f;
	lifetime = 0;
	colour = ofColor(255.0f*ofRandomuf(),100,100);
}

void Particle::addForce(ofVec3f _force)
{
    force = _force;
}

void Particle::update(float timeStep)
{
    	///damping
	force = force - vel * damping;

    ///update velocity
	vel += force;

	///update position
	pos += vel * timeStep;

	lifetime += 1;

}

void Particle::resetForce()
{
	force = ofVec3f::zero();
}

void Particle::draw()
{
	ofSetColor(colour);
	ofDrawSphere(pos.x,pos.y,pos.z,10);

	//glVertex3f(pos.x,pos.y,pos.z);
}
