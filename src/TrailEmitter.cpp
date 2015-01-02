#include "TrailEmitter.h"


Emitter::Emitter()
{
	myColor = ofColor( 255, 255, 255 );
	loc = ofVec3f::zero();
	vel = ofVec3f::zero();
}

void Emitter::exist( ofVec2f mouseLoc )
{
	setVelToMouse( mouseLoc );
	findVelocity();
	setPosition();
	iterateListExist();
	render();

	glDisable( GL_TEXTURE_2D );
    iterateListRenderTrails();
}

void Emitter::setVelToMouse( ofVec2f mouseLoc )
{
	velToMouse.set( mouseLoc.x - loc.x, mouseLoc.y - loc.y, 0 );
}

void Emitter::findVelocity()
{
	vel += ( velToMouse - vel ) * 0.25f;
}

void Emitter::setPosition()
{
	loc += vel;
}

void Emitter::iterateListExist()
{
	glEnable( GL_TEXTURE_2D );
	///particleImg->bind();

	for( list<Particle>::iterator it = particles.begin(); it != particles.end(); ) {
		if( ! it->ISDEAD ) {
			it->exist();
			++it;
		}
		else {
			it = particles.erase( it );
		}
	}
}

void Emitter::render()
{
	///emitterImg->bind();
	///renderImage( loc, 150, myColor, 1.0 );
}

void Emitter::iterateListRenderTrails()
{
	for( list<Particle>::iterator it = particles.begin(); it != particles.end(); ++it ) {
		it->renderTrails();
	}
}

void Emitter::addParticles( int _amt )
{
	for( int i = 0; i < _amt; i++ ) {
		particles.push_back( Particle( loc, vel ) );
	}
}
