#include "Particle.h"
#include "Perlin.h"
#include "ParticleSystem.h"

Perlin sPerlin( 2 );

Particle::Particle(ofVec3f _pos, ofVec3f _force, ParticleSystem* _parent)  :
    ps(_parent),
    pos(_pos),
    age(0),
    ageRatio(1.0f),
    bIsDead(false),
    bPerlin(true),
    bReturnOrigin(true),
    damping(0.21f),
    lifetime(250),
    scale(50.0f),
    mass(50.0f),
    forceMult(100.0f),
    perlinAmount(0.5),
    perlinThreshold(0.5),
    colour(ofColor::pink),
    trailLength(30.0f),
    rotation(0),
    rotationDir(1),
    spriteCount(1),
    animSpeed(0)
{

	force.set(_force);
	colour = ofColor(255.0f*ofRandomuf(),100,100);
	scale = ofRandom(100.0f,200.0f);
	mass = scale;
	vel = ofVec3f::zero();

	spriteCount = floor(ofRandom(ps->totalSprites));
	animSpeed = 0;

    //rotation = ofRandom(TWO_PI);
    rotationDir = ceil(ofRandomf())*2 - 1;


    if((ps->trailType == ParticleSystem::TRAIL_TAIL) || (ps->trailType == ParticleSystem::TRAIL_QUADS)) {
        for( int i = 0; i < trailLength; i++ ) {
            trailpos.push_back( _pos );
        }
    }

}

void Particle::addForce(ofVec3f _force)
{
    force = _force/mass;
}

void Particle::updateAge()
{
    age += 1.0f;

	if( age > lifetime ) {
		bIsDead = true;
	}
	else {
		// When spawned, the ageRatio is 1.0.
		// When death occurs, the ageRatio is 0.0.
		ageRatio = 1.0f - age / (float)lifetime;
	}
}

void Particle::calculatePerlin()
{
    perlin = ofVec3f::zero();
   if(force.length() < perlinThreshold) return;
//
//    if((vel.x > 0.001) &&(vel.y > 0.001)) {
//        float xnoise = ofSignedNoise((ofGetElapsedTimeMillis()/100.0f)*5.0f/(vel.x*10.05f) );
//        float ynoise = ofSignedNoise((ofGetElapsedTimeMillis()/100.0f)*5.0f/(vel.y*10.05f) );
//        perlin = ofVec3f((xnoise),(ynoise),0).getNormalized() * 1.0f * force;
//    }

	ofVec3f noise = sPerlin.dfBm( pos * 0.01f + ofVec3f( 0, 0, ofGetElapsedTimeMillis() / 1000.0f ) );
	perlin = noise.normalize() * perlinAmount * ageRatio;


}

void Particle::update(float timeStep)
{
    if(bIsDead)
    {
        if(bReturnOrigin) {
            ofVec3f f = ofVec3f::zero() - pos;
            addForce(f*5.0f);
        }
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

    rotation += force.length() * rotationDir;
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
        int divisions = (int)dist + 2;

        for(int j = 0; j < divisions;j++)
        {
            ofVec3f t = trailpos[i]+ (j/(float)(divisions))*diff;

            /* needed when shinking back to the origin: */
            if(t.lengthSquared() > pos.lengthSquared()) {
                t = pos;
            }


            float percent = 1.0f - i / (float)(trailpos.size()-1);
            percent -= (j/(float)divisions) *1.0f/(float)(trailpos.size()-1);

            trails.addVertex(t);
            trails.addNormal(ofVec3f(6.0f*percent,0,0));

//            ofFloatColor c = trailEndColor*percent + trailStartColor*(1.0-percent);
            ofFloatColor c = trailEndColor.getLerped(trailStartColor,percent);
            //ofFloatColor c = ofFloatColor(0, 0, 0, percent);
            //ofFloatColor c = ofFloatColor(percent, (percent *0.25f), (1.0f - percent),(percent));
            trails.addColor( c);
        }

    }

}

void Particle::renderTrails(ofVboMesh& trails)
{

	for( unsigned int i = 0; i < trailpos.size() - 1; i++ ) {
        float percent     = 1.0f - i / (float)(trailpos.size()-2);

		ofVec3f perp0 = trailpos[i] - trailpos[i+1];
		perp0.normalize();
		ofVec3f perp1 = perp0.crossed( ofVec3f(0,1,0) );
		ofVec3f perp2 = perp0.crossed( perp1 );
		ofVec3f off = perp2 * scale * ageRatio * percent * 0.2f;

        //ofFloatColor c = ofFloatColor(per, per, per,(per ));
        //ofFloatColor c = ofFloatColor(per, (per *0.25f), (1.0f - per),(per * 0.5f));
        //ofFloatColor c = ofFloatColor(255,0,0,255);
        //ofFloatColor c = trailEndColor*percent + trailStartColor*(1.0-percent);
        ofFloatColor c = trailEndColor.getLerped(trailStartColor,percent);

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
