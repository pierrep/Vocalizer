#include "ofApp.h"


//--------------------------------------------------------------
ofApp::~ofApp()
{
    delete[] audioData;
}

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(253,214,1);
    ofSetVerticalSync(true);
    ofSeedRandom(ofGetUnixTime());
    ofEnableAntiAliasing();
    //ofSoundStreamListDevices();
    //ofLogToFile("myLogFile.txt", false);

    audioThreshold.set("audioThreshold", 1.0, 0.0, 1.0);
    audioPeakDecay.set("audioPeakDecay", 0.915, 0.0, 1.0);
    audioMaxDecay.set("audioMaxDecay", 0.995, 0.0, 1.0);
    audioMirror.set("audioMirror", true);
    numOfBands = 64;

    audioData = new float[numOfBands];

    fft.setBufferSize(numOfBands);
    fft.setup(7);
    //fft.setup();

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
                if(ofGetFrameNum() % 2 == 0)
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
    ParticleSystem* p = new ParticleSystem();
    p->damping = 0.21f;
    p->lifetime = 250;
    p->forceMultiplier = 100.0f;
    p->returnToOrigin = true;
    p->perlinAmount = 0.9;
    p->perlinThreshold = 1.0;
    p->spriteName = "circle.png";

    p->setTrailType(ParticleSystem::TRAIL_LINE);
    p->setSheetWidth(1);
    p->loadResources();

    ps.push_back(p);

    ParticleSystem* p2 = new ParticleSystem();
    p2->damping = 0.01f;
    p2->lifetime = 100;
    p2->forceMultiplier = 10.0f;
    p2->returnToOrigin = false;
    p2->perlinAmount = 0.4;
    p2->perlinThreshold = 0.01;
    p2->spriteName = "flower_01.png";

    p2->setTrailType(ParticleSystem::TRAIL_QUADS);
    p2->setSheetWidth(1);
    p2->loadResources();

    ps.push_back(p2);
}

//--------------------------------------------------------------
void ofApp::loadSettings()
{
    for(unsigned int i=0; i < ps.size();i++) {

    }
}

//--------------------------------------------------------------
void ofApp::saveSettings()
{
    int idx;

    for(unsigned int i=0; i < ps.size();i++) {

        idx = xml.addTag("ParticleSystem");
        xml.pushTag("ParticleSystem",idx);
        xml.addValue("Damping", ps[i]->damping);
        xml.addValue("Lifetime", ps[i]->lifetime);
        xml.addValue("ForceMultiplier", ps[i]->forceMultiplier);
        xml.addValue("TrailType", ps[i]->trailType);
        xml.addValue("ReturnToOrigin", ps[i]->returnToOrigin);

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
