/**
 *
 *	ofxImageSequence example project
 *
 *  Created by James George ( http://www.jamesgeorge.org ) on 6/15/10.
 *  In collaboration with FlightPhase ( http://www.flightphase.com )
 *		- Updated for 0.8.4 by James George on 12/10/2014 for Specular (http://specular.cc) (how time flies!) 
 *
 *  Copyright (c) 2010-2015
 */

#pragma onc

#include "ofMain.h"
#include "ofxImageSequence.h"

class ofApp : public ofBaseApp
{

  public:
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
	
	ofxImageSequence sequence;
	ofImage background;
	bool playing;
};

