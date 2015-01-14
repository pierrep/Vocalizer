#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() :
    timeStep(0.5f)
{
	if(ofGetGLProgrammableRenderer()){
		billboardShader.load("shadersGL3/Billboard");
		trailShader.load("shadersGL3/Trail");
	}else{
		billboardShader.load("shadersGL2/Billboard");
		trailShader.load("shadersGL2/Trail");
	}

    particles.reserve(10000);

	/// we need to disable ARB textures in order to use normalized texcoords
    ofDisableArbTex();

	sprite.getTexture().enableMipmap();
	sprite.load("circle3.png");
	ofEnableAlphaBlending();

//    billboards.getVertices().reserve(1000);
//    billboards.getColors().reserve(1000);
//    billboards.getNormals().reserve(1000);

   	billboards.setUsage( GL_DYNAMIC_DRAW );
	billboards.setMode(OF_PRIMITIVE_POINTS);

    trails.setUsage( GL_DYNAMIC_DRAW );
	trails.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

}

void ParticleSystem::setTimeStep(float _timeStep)
{
	timeStep = _timeStep;
}

void ParticleSystem::addParticle(float force) {

    float kGamma = 100.0f;
    ofVec3f r = ofVec3f(ofRandomf(),ofRandomf(),ofRandomf());
    r.normalize();
    //ofLog() << "addParticle --  force: " << force << " r:" << r;
    Particle p(ofVec3f::zero(),r*kGamma*force);

    //addParticle(p);
    particles.push_back(p);

	int kNumParticles = particles.size();
	//cout << "num particles=" << kNumParticles << endl;
    billboards.getVertices().resize(kNumParticles);
	billboards.getColors().resize(kNumParticles);
	billboards.getNormals().resize(kNumParticles,ofVec3f(0));

	//ofLog() << "end addParticle";
}

void ParticleSystem::addParticle(Particle& p) {

//    billboards.addVertex(ofVec3f::zero());
//    billboards.addColor(ofFloatColor(0));
//    billboards.addNormal(ofVec3f::zero());

//	billboards.getColors()[kNumParticles-1].set(particles[kNumParticles-1].colour);
//	billboards.setNormal(kNumParticles-1,particles[kNumParticles-1].scale);
}

unsigned ParticleSystem::size() const {
	return particles.size();
}

Particle& ParticleSystem::operator[](unsigned i) {
	return particles[i];
}



void ParticleSystem::resetForces() {
	for(int i = 0; i < particles.size(); i++) {
		particles[i].resetForce();
	}
}



void ParticleSystem::update(ofCamera& cam) {
//ofLog() << "update ----------------------------";
	for(unsigned int i = 0; i < particles.size(); i++) {

		if(particles[i].bIsDead) {
            ofVec3f f = (ofVec3f::zero() - (particles[i].pos) / 20.0f);
            particles[i].addForce(f);
            //particles[i].scale *= 0.99f;

            if(particles[i].pos.distanceSquared(ofVec3f(0,0,0)) < 1.0f)
            {
                eraseParticle(i);
            } else {
                particles[i].update(timeStep);
            }
		} else {
            particles[i].update(timeStep);
           // ofLog() << "Particle position: " << particles[i].pos << " Total particles:" << particles.size();
		}


	}

    depthSort(cam);
    //noDepthSort(cam);

}

void ParticleSystem::draw() {

	renderTrails();

	billboardShader.begin();
	ofEnablePointSprites(); // not needed for GL3/4
	sprite.getTexture().bind();

	billboards.draw();

	sprite.getTexture().unbind();
	ofDisablePointSprites(); // not needed for GL3/4
	billboardShader.end();

}

void ParticleSystem::renderTrails()
{
	trails.clear();

	for( vector<Particle>::iterator it = particles.begin(); it != particles.end(); ++it ) {
		it->renderTrails(trails);
	}
	trails.setupIndicesAuto();

//	trailShader.begin();
//	ofEnablePointSprites();

    //trailImg.getTexture().bind();
	trails.draw(OF_MESH_FILL);
	//trailImg.getTexture().unbind();

//	ofDisablePointSprites();
  //  trailShader.end();
}

void ParticleSystem::noDepthSort(ofCamera& cam)
{
	for(unsigned int i = 0; i < particles.size(); i++) {
        billboards.getVertices()[i] = particles[i].pos;
        billboards.getColors()[i] = particles[i].colour;
        billboards.setNormal(i,particles[i].scale);
	}
}

void ParticleSystem::depthSort(ofCamera& cam)
{
        /* depth sorting */
	list< pair<int, float> > depthList;

	// put indexed points and z-values into the list
	for(unsigned int i = 0; i < particles.size(); i++) {
		depthList.push_back( make_pair(i, ofVec3f(particles[i].pos - cam.getPosition()).length() ));
	}

	// sort the list
	depthList.sort(DepthSortPredicate);

	std::list<pair<int, float> >::iterator it;
	int j = 0;
    for(it = depthList.begin(); it != depthList.end(); it++) {

	int i = it->first;
        billboards.getVertices()[j] = particles[i].pos;
        billboards.getColors()[j] = particles[i].colour;
        billboards.setNormal(j,particles[i].scale);
		j++;
	}
}

void ParticleSystem::eraseParticle(int i)
{
    ///kill particle
    //cout << "erase particle " << i << endl;
    particles.erase(particles.begin()+i);
    int kNumParticles = particles.size();
//    billboards.removeVertex(i);
//    billboards.removeColor(i);
//    billboards.removeNormal(i);
    billboards.getVertices().resize(kNumParticles);
    billboards.getColors().resize(kNumParticles);
    billboards.getNormals().resize(kNumParticles,ofVec3f(0));
}
