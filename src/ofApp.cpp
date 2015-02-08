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

    //ofLogToFile("myLogFile.txt", false);
   // ofSoundStreamListDevices();

    audioThreshold.set("audioThreshold", 1.0, 0.0, 1.0);
    audioPeakDecay.set("audioPeakDecay", 0.915, 0.0, 1.0);
    audioMaxDecay.set("audioMaxDecay", 0.995, 0.0, 1.0);
    audioMirror.set("audioMirror", true);
    numOfBands = 64;

    audioData = new float[numOfBands];

    fft.setBufferSize(numOfBands);
    fft.setup(10);
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
            particleSystem.addParticle(audioValue,i/(float)numOfBands);
            rotation += audioValue;
            doRotate = false;
            rotFade = audioValue;
        }
    }

    //particleSystem.setTimeStep(deltaTime);
    particleSystem.update(cam);
    particleSystem.resetForces();

    if(doRotate) rotation = rotation + (rotFade*=0.95);

//    int n = 0;
//    int d = 0;
//    for(unsigned int i = 0; i < particleSystem.getParticles().size();i++) {
//        if(!particleSystem.getParticles()[i].bIsDead) {
//            n++;
//        }
//        else d++;
//    }
    //cout << "particles  alive: " << n << "  dead: " <<  d << "  total: " <<  particleSystem.getParticles().size() << "  num vertices: " << particleSystem.billboards.getNumVertices() << endl;
}

//--------------------------------------------------------------
void ofApp::draw() {

    cam.begin();
    //cam.setPosition(0,0,500);

    particleSystem.draw();

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
    particleSystem.setTrailType(ParticleSystem::TRAIL_TAIL);
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
