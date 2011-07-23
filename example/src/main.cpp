
/**
 *  main.cpp
 *
 *	ofxImageSequence example project
 *
 *  Created by James George ( http://www.jamesgeorge.org ) on 6/15/10.
 *  In collaboration with FlightPhase ( http://www.flightphase.com )
 *
 *  Copyright (c) 2010 
 */

#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 366, 243, OF_WINDOW);
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
