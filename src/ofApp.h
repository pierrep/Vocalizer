#pragma once

#include "ofMain.h"
#include "ofxFFTLive.h"
#include "ofxGui.h"
#include "ParticleSystem.h"

class ofApp : public ofBaseApp{

public:
    ~ofApp();
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void drawGUI();
    void updateFFT();

    void setupParticles();

    ofxFFTLive fft;
    int numOfBands;

    bool bDrawGui;
    ofxPanel gui;
    ofParameter<float> audioThreshold;
    ofParameter<float> audioPeakDecay;
    ofParameter<float> audioMaxDecay;
    ofParameter<bool> audioMirror;

    ofMesh meshOriginal;
    ofMesh meshWarped;
    ofCamera cam;

    float* audioData;

    ParticleSystem particleSystem;

    ParticleSystem spriteSystem;

    ofShader    billboardShader;
    ofImage     texture;

    float       rotation;
    float       rotFade;




};
