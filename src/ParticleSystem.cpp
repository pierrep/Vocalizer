#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() :
    timeStep(0.5f)
{
	if(ofGetGLProgrammableRenderer()){
		billboardShader.load("shadersGL3/Billboard");
	}else{
		billboardShader.load("shadersGL2/Billboard");
	}

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
        particles[i].update(timeStep);

		if(particles[i].lifetime > 150) {
            particles[i].addForce((ofVec3f::zero() - particles[i].pos) / 20.0f);
            particles[i].scale *= 0.99f;

            if(particles[i].pos.distanceSquared(ofVec3f(0,0,0)) < 1.0f)
            {
                eraseParticle(i);
            }
		}
	}

    depthSort(cam);

}

void ParticleSystem::draw() {

	billboardShader.begin();
	ofEnablePointSprites(); // not needed for GL3/4
	sprite.getTexture().bind();

	billboards.draw();

	sprite.getTexture().unbind();
	ofDisablePointSprites(); // not needed for GL3/4
	billboardShader.end();

	renderTrails();
}

void ParticleSystem::renderTrails()
{
	trails.clear();

	for( vector<Particle>::iterator it = particles.begin(); it != particles.end(); ++it ) {
		it->renderTrails(trails);
	}
	trails.setupIndicesAuto();

//	billboardShader.begin();
//	ofEnablePointSprites();

    //trailImg.getTexture().bind();
	trails.draw(OF_MESH_FILL);
	//trailImg.getTexture().unbind();

//	ofDisablePointSprites();
//    billboardShader.end();
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
    particles.erase(particles.begin()+i);
    int kNumParticles = particles.size();
    billboards.getVertices().resize(kNumParticles);
    billboards.getColors().resize(kNumParticles);
    billboards.getNormals().resize(kNumParticles,ofVec3f(0));
}
