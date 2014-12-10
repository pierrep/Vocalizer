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
    ofSetSphereResolution(12);

    audioThreshold.set("audioThreshold", 1.0, 0.0, 1.0);
    audioPeakDecay.set("audioPeakDecay", 0.915, 0.0, 1.0);
    audioMaxDecay.set("audioMaxDecay", 0.995, 0.0, 1.0);
    audioMirror.set("audioMirror", true);
    numOfBands = 64;

    audioData = new float[numOfBands];

    fft.setBufferSize(numOfBands);
    fft.setup();


    string guiPath = "audio.xml";
    gui.setup("audio", guiPath, 20, 20);
    gui.add(audioThreshold);
    gui.add(audioPeakDecay);
    gui.add(audioMaxDecay);
    gui.add(audioMirror);
    //gui.loadFromFile(guiPath);

}

//--------------------------------------------------------------
void ofApp::update() {
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    fft.setThreshold(audioThreshold);
    fft.setPeakDecay(audioPeakDecay);
    fft.setMaxDecay(audioMaxDecay);
    fft.setMirrorData(audioMirror);
    fft.update();

    memset(audioData,0,numOfBands*sizeof(float));
    fft.getFftData(audioData, numOfBands);

    for(int i=0; i<numOfBands; i++) {
        float audioValue = audioData[i];
        if (audioValue > 0.1f) {
            //add particle!
            addParticle(audioValue);
        }
    }

    particleSystem.update();
    particleSystem.resetForces();

}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255);

    int w = OFX_FFT_WIDTH;
    int h = OFX_FFT_HEIGHT;
    int x = 20;
    int y = ofGetHeight() - h - 20;
    fft.draw(x, y, w, h);

    ofSetColor(255);

    cam.begin();
    //meshWarped.drawWireframe();
    	particleSystem.draw();
    cam.end();

    gui.draw();
}

//--------------------------------------------------------------
void ofApp::addParticle(float force)
{
    float kGamma = 100.0f;
    Particle particle(ofVec3f::zero(),ofVec3f(ofRandomf()*kGamma*force, ofRandomf()*kGamma*force, ofRandomf()*kGamma*force) );
    particleSystem.add(particle);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
