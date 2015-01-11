#include "Particle.h"

Particle::Particle(ofVec3f _pos, ofVec3f _force)
{
    trailLength = 20;

	for( int i = 0; i < trailLength; i++ ) {		pos.push_back( _pos );	}
    pos.push_back(_pos);
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
    trailLength = p.trailLength;
	for( int i = 0; i < trailLength; i++ ) {        pos.push_back(p.pos[0]);	}
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
	pos[0] += vel * timeStep;

	lifetime += 1;

}

void Particle::resetForce()
{
	force = ofVec3f::zero();
}
