#include "ofApp.h"


//--------------------------------------------------------------
ofApp::~ofApp()
{
    delete[] audioData;
}

//--------------------------------------------------------------
void ofApp::setup() {
    BgColour = ofColor(253,214,1);
    
    ofSetVerticalSync(true);
    ofSeedRandom(ofGetUnixTime());
    ofEnableAntiAliasing();
    ofSoundStreamListDevices();
    //ofLogToFile("myLogFile.txt", false);

    audioThreshold.set("audioThreshold", 1.0, 0.0, 1.0);
    audioPeakDecay.set("audioPeakDecay", 0.915, 0.0, 1.0);
    audioMaxDecay.set("audioMaxDecay", 0.995, 0.0, 1.0);
    audioMirror.set("audioMirror", true);
    numOfBands = 64;

    audioData = new float[numOfBands];

    fft.setBufferSize(numOfBands);
    //fft.setup(6);
    fft.setup();

    bDrawGui = true;
    string guiPath = "audio.xml";
    gui.setup("audio", guiPath, 20, 20);
    gui.add(audioThreshold);
    gui.add(audioPeakDecay);
    gui.add(audioMaxDecay);
    gui.add(audioMirror);
    //gui.loadFromFile(guiPath);

    setupParticles();
    rotation = 0.0f;

    ofBackground(BgColour);

}

//--------------------------------------------------------------
void ofApp::update() {
    curTime = ofGetElapsedTimeMillis()/15.0f;
    deltaTime = curTime - prevTime;
    prevTime = curTime;


    updateFFT();

    bool doRotate = true;
    float audioValue = 0;
    for(int i=5; i<numOfBands; i++) {
        audioValue = audioData[i];
        if (audioValue > 0.3f) {
            //add particle!
            for(unsigned int j=0; j < ps.size();j++) {
                //if(ofGetFrameNum() % 2 == 0)
                if(ofGetFrameNum() % ps[j]->getParticleRate() == 0)
                {
                    ps[j]->addParticle(audioValue,i/(float)numOfBands);
                }
            }

            rotation += audioValue;
            doRotate = false;
            rotFade = audioValue;
        }
    }

    for(unsigned int i=0; i < ps.size();i++) {
        //ps[i].setTimeStep(deltaTime);
        ps[i]->update(cam);
        ps[i]->resetForces();
    }

    if(doRotate) rotation = rotation + (rotFade*=0.95);

}

//--------------------------------------------------------------
void ofApp::draw() {

    cam.begin();
    //cam.setPosition(0,0,500);

    for(unsigned int i=0; i < ps.size();i++) {
        ps[i]->draw();
    }

    cam.orbit(rotation,0,500,ofVec3f(0,0,0));
    cam.end();

    ofSetColor(255,255,255);
    ofDrawBitmapString(ofToString(ofGetFrameRate()),20,20);
    if(bDrawGui) {
            drawGUI();
    }
}

//--------------------------------------------------------------
void ofApp::setupParticles()
{
    loadSettings();
}

//--------------------------------------------------------------
void ofApp::loadSettings()
{

    xml.load("settings.xml");
    
    BgColour = ofFromString<ofColor>(xml.getValue("Background","255,0,0,255"));
    
    int numPS = xml.getNumTags("ParticleSystem");
    if(numPS > 0) {
        for (int i = 0;i < numPS;i++) {
            xml.pushTag("ParticleSystem",i);

            ParticleSystem* p = new ParticleSystem();
            p->spriteDamping = xml.getValue("SpriteDamping",0.1f);
            p->spriteLifetime = xml.getValue("SpriteLifetime",100);
            p->spriteScale = xml.getValue("SpriteScale",30.0f);
            p->spriteMass = xml.getValue("SpriteMass",30.0f);
            p->spriteInitialRotation = xml.getValue("SpriteInitialRotation",0.0f);
            p->spriteRotationDir = xml.getValue("SpriteRotationDir",0);
            p->spriteColour = ofFromString<ofColor>(xml.getValue("SpriteColour","255,255,255,255"));
            p->forceMultiplier = xml.getValue("ForceMultiplier",50.0f);
            p->returnToOrigin = ofFromString<bool>(xml.getValue("ReturnToOrigin","true"));
            p->perlinNoise = ofFromString<bool>(xml.getValue("PerlinNoise","true"));
            p->perlinAmount = xml.getValue("PerlinAmount",0.5f);
            p->perlinThreshold = xml.getValue("PerlinThreshold",1.0f);
            p->spriteAnimationSpeed = xml.getValue("SpriteAnimationSpeed",0);
            p->trailLength = xml.getValue("TrailLength",50.0f);
            p->trailWidth = xml.getValue("TrailWidth",6.0f);
            p->trailTaperWidth = ofFromString<bool>(xml.getValue("TrailTaperWidth","true"));
            p->trailStartColour = ofFromString<ofFloatColor>(xml.getValue("TrailStartColour","1.0f,1.0f,1.0f,1.0f"));
            p->trailEndColour = ofFromString<ofFloatColor>(xml.getValue("TrailEndColour","1.0f,1.0f,1.0f,1.0f"));

            p->spriteName = xml.getValue("SpriteName","circle.png");
            ParticleSystem::TrailType trailtype = (ParticleSystem::TrailType) xml.getValue("TrailType",0);
            p->setTrailType(trailtype);
            p->setSheetWidth(xml.getValue("SpriteSheetWidth", 1));
            p->setParticleRate(xml.getValue("ParticleRate", 1));
            p->loadResources();

            ps.push_back(p);
            xml.popTag();
        }
    } else {
        ofLogError() << "No particle systems found in XML file";
    }
}

//--------------------------------------------------------------
void ofApp::saveSettings()
{
    int idx;

    xml.clear();

    for(unsigned int i=0; i < ps.size();i++) {

        idx = xml.addTag("ParticleSystem");
        xml.pushTag("ParticleSystem",idx);
        xml.addValue("SpriteName", ps[i]->spriteName);
        xml.addValue("SpriteDamping", ps[i]->spriteDamping);
        xml.addValue("SpriteLifetime", ps[i]->spriteLifetime);
        xml.addValue("SpriteScale", ps[i]->spriteScale);
        xml.addValue("SpriteMass", ps[i]->spriteMass);
        xml.addValue("ForceMultiplier", ps[i]->forceMultiplier);
        xml.addValue("SpriteColour", ofToString(ps[i]->spriteColour));
        xml.addValue("SpriteInitialRotation", ps[i]->spriteInitialRotation);
        xml.addValue("SpriteSheetWidth", ps[i]->spriteSheetWidth);
        xml.addValue("SpriteRotationDir", ps[i]->spriteRotationDir);
        xml.addValue("ReturnToOrigin", ps[i]->returnToOrigin);
        xml.addValue("SpriteAnimationSpeed", ps[i]->spriteAnimationSpeed);
        xml.addValue("PerlinNoise", ofToString(ps[i]->perlinNoise));
        xml.addValue("PerlinThreshold", ps[i]->perlinThreshold);
        xml.addValue("PerlinAmount", ps[i]->perlinAmount);
        xml.addValue("TrailType", ps[i]->trailType);
        xml.addValue("TrailLength", ps[i]->trailLength);
        xml.addValue("TrailStartColour", ofToString(ps[i]->trailStartColour));
        xml.addValue("TrailEndColour", ofToString(ps[i]->trailEndColour));
        xml.addValue("TrailTaperWidth", ofToString(ps[i]->trailTaperWidth));
        xml.addValue("TrailWidth", ps[i]->trailWidth);
        xml.addValue("ParticleRate", ps[i]->getParticleRate());
        xml.popTag();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f') {
        ofToggleFullscreen();
        bDrawGui = !bDrawGui;
    }
    if(key == 'g') {
        bDrawGui = !bDrawGui;
    }
    if((key == 's') || (key == 'S')) {
        saveSettings();
    }
}

//--------------------------------------------------------------
void ofApp::drawGUI() {
        ofSetColor(255);
        int w = OFX_FFT_WIDTH;
        int h = OFX_FFT_HEIGHT;
        int x = 20;
        int y = ofGetHeight() - h - 20;
        fft.draw(x, y, w, h);

        ofSetColor(255);
        gui.draw();
}

void ofApp::updateFFT()
{
    fft.setThreshold(audioThreshold);
    fft.setPeakDecay(audioPeakDecay);
    fft.setMaxDecay(audioMaxDecay);
    fft.setMirrorData(audioMirror);
    fft.update();

    memset(audioData,0,numOfBands*sizeof(float));
    fft.getFftData(audioData, numOfBands);
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if((key == 's' ) || (key == 'S')) {
        xml.save("settings.xml");
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
