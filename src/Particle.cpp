#include "Particle.h"

Particle::Particle(ofVec3f _pos, ofVec3f _force)
{
    pos.set(_pos);
	force.set(_force);
	damping = 0.2f;
	lifetime = 0;
	colour = ofColor(255.0f*ofRandomuf(),100,100);
	scale = ofVec3f(1000,0,0)* 2.0f/force.length();
	scale = ofVec3f(ofClamp(scale.x,5,70),0,0);
	vel = ofVec3f::zero();
}

///copy constructor
Particle::Particle(const Particle &p) {
    pos.set(p.pos);
	force.set(p.force);
	damping = 0.2f;
	lifetime = 0;
	colour = p.colour;
	scale = p.scale;
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
