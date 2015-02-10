#include "ParticleSystem.h"
#include "Particle.h"

ParticleSystem::ParticleSystem() :
    spriteSheetWidth(1),
    trailType(TRAIL_NONE),
    timeStep(0.5f),
    totalSprites(1)
{
    particles.reserve(10000);

	/// we need to disable ARB textures in order to use normalized texcoords
    ofDisableArbTex();


	ofEnableAlphaBlending();

//    billboards.getVertices().reserve(1000);
//    billboards.getColors().reserve(1000);
//    billboards.getNormals().reserve(1000);

   	billboards.setUsage( GL_DYNAMIC_DRAW );
	billboards.setMode(OF_PRIMITIVE_POINTS);

    trails.setUsage( GL_DYNAMIC_DRAW );
    //trails.setMode(OF_PRIMITIVE_POINTS);

}

void ParticleSystem::loadResources()
{
	//if(ofGetGLProgrammableRenderer()){
    //  spriteShader.load("shadersGL3/Billboard");
	//	billboardShader.load("shadersGL3/Billboard");
	//	trailShader.load("shadersGL3/Trail");
	//}else
    {
	    if(spriteAnimationSpeed > 0) {
            spriteShader.load("shadersGL2/BillboardSheet");
	    } else {
            spriteShader.load("shadersGL2/Billboard");
	    }
		billboardShader.load("shadersGL2/Billboard");
		trailShader.load("shadersGL2/Trail");
	}

	sprite.getTexture().enableMipmap();
	sprite.load(spriteName);
	//sprite.load("sprite_sheet1.png");
	//sprite.load("sprite_sheet_anim.png");

	spriteTrail.getTexture().enableMipmap();
	spriteTrail.load("sprite-trails/dot.png");
}

void ParticleSystem::setTimeStep(float _timeStep)
{
	timeStep = _timeStep;
}

void ParticleSystem::setSheetWidth(int sw)
{
	spriteSheetWidth = sw;
	totalSprites = sw * sw;
}


void ParticleSystem::setTrailType(TrailType _ttype)
{
	trailType = _ttype;

   if ((trailType == TRAIL_LINE) || (trailType == TRAIL_TAIL))  {
        trails.setMode(OF_PRIMITIVE_POINTS);
    } else if(trailType == TRAIL_QUADS) {
        trails.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    }
}

void ParticleSystem::addParticle(float force, float spectrum) {

    ofVec3f r = ofVec3f(ofRandomf(),ofRandomf(),ofRandomf());
    r.normalize();
    //ofLog() << "addParticle --  force: " << force << " r:" << r;
    Particle p(ofVec3f::zero(),r*force*forceMultiplier,this);

    ofFloatColor c;
    c.setHsb(spectrum,0.8,1);
    //p.colour = c;
    

    p.colour = spriteColour;
    p.colour.setBrightness(ofClamp(1.0-spectrum,0,1)*300);
    p.scale *= (1.0f/(spectrum))*0.05f;
    p.damping = spriteDamping;
    p.forceMult = forceMultiplier;
    p.lifetime = spriteLifetime;
    p.scale = (spriteScale * ofClamp(0.5f*(1.0f/(spectrum))*0.5f,1,1000));
    p.mass = spriteMass;
    p.rotation = spriteInitialRotation;
    p.rotationDir = spriteRotationDir;
    p.bReturnOrigin = returnToOrigin;
    p.bPerlin = perlinNoise;
    p.perlinAmount = perlinAmount;
    p.perlinThreshold = perlinThreshold;
    p.animSpeed = spriteAnimationSpeed;
    p.trailStartColor = trailStartColour;
    p.trailEndColor = trailEndColour;

    particles.push_back(p);

	int kNumParticles = particles.size();
	//cout << "num particles=" << kNumParticles << endl;
    billboards.getVertices().resize(kNumParticles);
	billboards.getColors().resize(kNumParticles);
	billboards.getNormals().resize(kNumParticles,ofVec3f(0));
    rotations.resize(kNumParticles);

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
	for(unsigned int i = 0; i < particles.size(); i++) {
		particles[i].resetForce();
	}
}

void ParticleSystem::update(ofCamera& cam) {
//ofLog() << "update ----------------------------";

    for(vector<Particle>::iterator itr = particles.begin(); itr != particles.end(); ) {

		if((*itr).bIsDead) {

            if(( (*itr).pos.distanceSquared(ofVec3f(0,0,0)) < 1.0f) || ((*itr).scale < 0.1f))
            {
                itr = particles.erase(itr);
                int kNumParticles = particles.size();
                billboards.getVertices().resize(kNumParticles);
                billboards.getColors().resize(kNumParticles);
                billboards.getNormals().resize(kNumParticles,ofVec3f(0));
            } else {
                (*itr).update(timeStep);
                ++itr;
            }
		} else {
            (*itr).update(timeStep);
            ++itr;
		}

	}
    depthSort(cam);
    //noDepthSort(cam);
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

void ParticleSystem::draw() {

	renderTrails();

	spriteShader.begin();
	ofEnablePointSprites(); // not needed for GL3/4
	sprite.getTexture().bind();

    if(particles.size() > 0) {
        int angleLoc = spriteShader.getAttributeLocation("angle");
        billboards.getVbo().setAttributeData(angleLoc, &rotations[0], 1, particles.size(), GL_DYNAMIC_DRAW);
    }

	billboards.draw();

	sprite.getTexture().unbind();
	ofDisablePointSprites(); // not needed for GL3/4
	spriteShader.end();

}

void ParticleSystem::renderTrails()
{
   // ofBlendMode(OF_BLENDMODE_ADD);
    if(trailType == TRAIL_NONE) return;

	trails.clear();

	for( vector<Particle>::iterator it = particles.begin(); it != particles.end(); ++it ) {
        if((trailType == TRAIL_LINE) || (trailType == TRAIL_TAIL)) {
            it->renderTrailPoints(trails);
        } else if(trailType == TRAIL_QUADS) {
            it->renderTrails(trails);
        }
	}
	trails.setupIndicesAuto();

    if(trailType == TRAIL_QUADS) {
        trailShader.begin();

        spriteTrail.getTexture().bind();
        trails.draw(OF_MESH_FILL);
        spriteTrail.getTexture().unbind();

        trailShader.end();
    } else if ((trailType == TRAIL_LINE) || (trailType == TRAIL_TAIL)) {
        billboardShader.begin();
        ofEnablePointSprites();

        spriteTrail.getTexture().bind();
        trails.draw();
        spriteTrail.getTexture().unbind();

        ofDisablePointSprites();
        billboardShader.end();
    }
}

void ParticleSystem::noDepthSort(ofCamera& cam)
{
	for(unsigned int i = 0; i < particles.size(); i++) {
        billboards.getVertices()[i] = particles[i].pos;
        billboards.getColors()[i] = particles[i].colour;
        billboards.setNormal(i,ofVec3f(particles[i].scale,spriteSheetWidth,particles[i].spriteCount));
        rotations[i] = particles[i].rotation;

        if(particles[i].animSpeed > 0) {
            if((int)particles[i].age % particles[i].animSpeed == 0) particles[i].spriteCount++;
            if(particles[i].spriteCount >= totalSprites) particles[i].spriteCount = 0;
        }
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
        billboards.setNormal(j,ofVec3f(particles[i].scale,spriteSheetWidth,particles[i].spriteCount));
        rotations[j] = particles[i].rotation;

        if(particles[i].animSpeed > 0) {
            if((int)particles[i].age % particles[i].animSpeed == 0) particles[i].spriteCount++;
            if(particles[i].spriteCount >= totalSprites) particles[i].spriteCount = 0;
        }

		j++;
	}

}
