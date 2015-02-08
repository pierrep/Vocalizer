#include "Particle.h"
#include "Perlin.h"
#include "ParticleSystem.h"

Perlin sPerlin( 2 );

Particle::Particle(ofVec3f _pos, ofVec3f _force, ParticleSystem* _parent)  :
    ps(_parent),
    pos(_pos),
    damping(0.21f),
    lifetime(250),
    age(0),
    ageRatio(1.0f),
    bIsDead(false),
    bPerlin(true),
    forceMult(100.0f),
    trailLength(50.0f),
    rotation(0),
    rotationDir(1),
    animSpeed(0)
{

	force.set(_force*forceMult);
	colour = ofColor(255.0f*ofRandomuf(),100,100);
	scale = ofRandom(120.0f,200.0f);
	mass = scale;
	vel = ofVec3f::zero();

	spriteNum = floor(ofRandom(ps->totalSprites));
	animSpeed = 0;

    rotation = ofRandom(TWO_PI);
    rotationDir = ceil(ofRandomf())*2 - 1;

    if((ps->trailType == ParticleSystem::TRAIL_TAIL) || (ps->trailType == ParticleSystem::TRAIL_QUADS)) {
        for( int i = 0; i < trailLength; i++ ) {            trailpos.push_back( _pos );        }
    }

}

void Particle::addForce(ofVec3f _force)
{
    force = _force/mass;
}

void Particle::updateAge(){    age += 1.0f;	if( age > lifetime ) {		bIsDead = true;	}	else {		// When spawned, the ageRatio is 1.0.		// When death occurs, the ageRatio is 0.0.		ageRatio = 1.0f - age / (float)lifetime;	}}

void Particle::calculatePerlin()
{
    perlin = ofVec3f::zero();
   if(force.length() < 0.9f) return;
//
//    if((vel.x > 0.001) &&(vel.y > 0.001)) {
//        float xnoise = ofSignedNoise((ofGetElapsedTimeMillis()/100.0f)*5.0f/(vel.x*10.05f) );
//        float ynoise = ofSignedNoise((ofGetElapsedTimeMillis()/100.0f)*5.0f/(vel.y*10.05f) );
//        perlin = ofVec3f((xnoise),(ynoise),0).getNormalized() * 1.0f * force;
//    }

	ofVec3f noise = sPerlin.dfBm( pos * 0.01f + ofVec3f( 0, 0, ofGetElapsedTimeMillis() / 1000.0f ) );
	perlin = noise.normalize() * 0.9f * ageRatio;


}

void Particle::update(float timeStep)
{
    if(bIsDead) {
        ofVec3f f = ofVec3f::zero() - pos;
        addForce(f*5.0f);
        scale *= 0.97f;
    }

    ///damping
	force = force - vel * damping;

    ///update velocity
	vel += force;

    calculatePerlin();
	///update perlin noise
	if(bPerlin) {
        vel += perlin;
	}

	///update position
	pos += vel * timeStep;

    updateTrails();

	updateAge();

    rotation += force.length()/10.0f * rotationDir;
}

void Particle::updateTrails()
{
    if((ps->trailType == ParticleSystem::TRAIL_TAIL) || (ps->trailType == ParticleSystem::TRAIL_QUADS)) {
        trailpos.pop_back();
        trailpos.insert(trailpos.begin(),pos);
    }
    else if(ps->trailType == ParticleSystem::TRAIL_LINE) {
        if(vel.length() > 1.0f) {
            if(!bIsDead) {
            trailpos.insert(trailpos.begin(),pos);
            }
//            else {
//                if(trailpos.size() > 1)
//                    trailpos.erase(trailpos.begin());
//            }
        }

    }
}

void Particle::renderTrailPoints(ofVboMesh& trails)
{

    for( unsigned int i = 0; i < trailpos.size() - 1; i++ ) {

        ofVec3f diff =  trailpos[i+1] - trailpos[i];
        float dist = diff.length();
        int divisions = (int)dist + 3;

        for(int j = 0; j < divisions;j++)
        {
            ofVec3f t = trailpos[i]+ (j/(float)(divisions))*diff;
            if(t.lengthSquared() > pos.lengthSquared()) t = pos;
            trails.addVertex(t);
            trails.addNormal(ofVec3f(5.0f,0,0));

            float percent = 1.0f - i / (float)(trailpos.size()-1);
            percent -= (j/(float)divisions) *1.0f/(float)(trailpos.size()-1);

            ofFloatColor c = ofFloatColor(0, 0, 0, percent);
            //ofFloatColor c = ofFloatColor(percent, (percent *0.25f), (1.0f - percent),(percent));
            trails.addColor( c);
        }

    }

}

void Particle::renderTrails(ofVboMesh& trails){

	for( unsigned int i = 0; i < trailpos.size() - 1; i++ ) {
        float per     = 1.0f - i / (float)(trailpos.size()-2);

		ofVec3f perp0 = trailpos[i] - trailpos[i+1];
		perp0.normalize();		ofVec3f perp1 = perp0.crossed( ofVec3f(0,1,0) );		ofVec3f perp2 = perp0.crossed( perp1 );		ofVec3f off = perp2 * scale * ageRatio * per * 0.2f;

        //ofFloatColor c = ofFloatColor(per, per, per,(per ));
        ofFloatColor c = ofFloatColor(per, (per *0.25f), (1.0f - per),(per * 0.5f));
        //ofFloatColor c = ofFloatColor(255,0,0,255);

		ofVec3f pt = trailpos[i] - off;
        trails.addVertex(pt);
        if(i%2 == 0)
            trails.addTexCoord(ofVec2f(0,1));
        else
            trails.addTexCoord(ofVec2f(0,0));

        if(i == 0) {
            trails.addColor( ofFloatColor(0,0,0,0));
        }
        else if(i < (trailpos.size() - 2))
            trails.addColor( c);
        else {
            trails.addColor( ofFloatColor(0,0,0,0));
        }

		ofVec3f pt2 = trailpos[i] + off;
        trails.addVertex(pt2);
        if(i%2 == 0)
            trails.addTexCoord(ofVec2f(1,1));
        else
            trails.addTexCoord(ofVec2f(1,0));

        if(i == 0) {
            trails.addColor( ofFloatColor(0,0,0,0));
        }
        else if(i < (trailpos.size() - 2))
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
