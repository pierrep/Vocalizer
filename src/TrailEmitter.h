/*
The emitter is just an object that follows the cursor and
can spawn new particle objects. It would be easier to just make
the location vector match the cursor position but I have opted
to use a velocity vector because later I will be allowing for
multiple emitters.
*/

#pragma once
#include "TrailParticle.h"
#include <list>

class Emitter {
 public:
	Emitter();
	void exist( ofVec2f mouseLoc );
	void setVelToMouse( ofVec2f mouseLoc );
	void findVelocity();
	void setPosition();
	void iterateListExist();
	void render();
	void iterateListRenderTrails();
	void addParticles( int _amt );

	ofVec3f loc;
	ofVec3f vel;
	ofVec3f velToMouse;
	ofColor myColor;
	std::list<Particle>	particles;
};
