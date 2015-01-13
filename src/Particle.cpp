#include "Particle.h"

Particle::Particle(ofVec3f _pos, ofVec3f _force)
{
    trailLength = 20;

	for( int i = 0; i < trailLength; i++ ) {		trailpos.push_back( _pos );	}
    pos = _pos;
	force.set(_force);
	damping = 0.2f;
	lifetime = 0;
	colour = ofColor(255.0f*ofRandomuf(),100,100);
	scale = ofVec3f(1000,0,0)* 2.0f/force.length();
	scale = ofVec3f(ofClamp(scale.x,5,70),0,0);
	vel = ofVec3f::zero();
	trailType = TRAIL_TAIL;
}

///copy constructor
Particle::Particle(const Particle &p) {
    trailLength = p.trailLength;
	for( int i = 0; i < trailLength; i++ ) {        trailpos.push_back(p.pos);	}
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
    updateTrails();

    ///damping
	force = force - vel * damping;

    ///update velocity
	vel += force;

	///update position
	pos += vel * timeStep;

	lifetime += 1;

}

void Particle::updateTrails()
{
    if(trailType == TRAIL_TAIL) {
        trailpos.pop_back();
        trailpos.insert(trailpos.begin(),pos);
    }
    else if(trailType == TRAIL_DOTS) {
        if((vel.length() > 1) && (lifetime < 150)) {
            trailpos.insert(trailpos.begin(),pos);
        }
    }
}

void Particle::renderTrails(ofVboMesh& trails){
	for( unsigned int i = 0; i < trailpos.size() - 2; i++ ) {
        float per     = 1.0f - i / (float)(trailpos.size()-1);

		ofVec3f perp0 = trailpos[i] - trailpos[i+1];
		perp0.normalize();		ofVec3f perp1 = perp0.crossed( ofVec3f(0,1,0) );		ofVec3f perp2 = perp0.crossed( perp1 );		ofVec3f off = perp2 * 40.0f;//( radius * agePer * per * 0.1f );

        //ofFloatColor c = ofFloatColor(per, per, per,(per * 0.5f));
//        ofFloatColor c = ofFloatColor(per, (per *0.25f), (1.0f - per),(per * 0.5f));
        ofFloatColor c = ofFloatColor(255,0,0,255);

		ofVec3f pt = trailpos[i] - off;
        trails.addVertex(pt);
//        if(i%2 == 0)
//            trails.addTexCoord(ofVec2f(0,1));
//        else
//            trails.addTexCoord(ofVec2f(0,0));

        if(i == 0) {
            trails.addColor( ofFloatColor(0,0,0,0));
        }
        else if(i < (trailpos.size() - 3))
            trails.addColor( c);
        else {
            trails.addColor( ofFloatColor(0,0,0,0));
        }

		ofVec3f pt2 = trailpos[i] + off;
        trails.addVertex(pt2);
//        if(i%2 == 0)
//            trails.addTexCoord(ofVec2f(1,1));
//        else
//            trails.addTexCoord(ofVec2f(1,0));

        if(i == 0) {
            trails.addColor( ofFloatColor(0,0,0,0));
        }
        else if(i < (trailpos.size() - 3))
            trails.addColor( c);
        else {
            trails.addColor( ofFloatColor(0,0,0,0));
        }

        if(i == 0) {
            trails.addVertex(pt);
            trails.addVertex(pt2);
            trails.addColor( c);
            trails.addColor( c);
		}
	}
}

void Particle::resetForce()
{
	force = ofVec3f::zero();
}
