#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"

//#define USE_PROGRAMMABLE_RENDERER

#ifdef USE_PROGRAMMABLE_RENDERER
#include "ofGLProgrammableRenderer.h"
#endif
//========================================================================
int main( ){
    ofSetLogLevel(OF_LOG_VERBOSE);

    #ifdef USE_PROGRAMMABLE_RENDERER
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
#endif

    ofAppGlutWindow window;
    //window.setGlutDisplayString("rgba double samples>=8 depth");
	ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			// <-------- setup the GL context
	//ofSetupOpenGL(1024,768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
