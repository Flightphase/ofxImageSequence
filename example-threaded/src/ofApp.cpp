/**
 *  ofApp.cpp
 *
 *	ofxImageSequence example project
 *
 * Created by James George, http://www.jamesgeorge.org
 * in collaboration with FlightPhase http://www.flightphase.com
 *		- Updated for 0.8.4 by James George on 12/10/2014 for Specular (http://specular.cc) (how time flies!) 
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------
 *
 * simple ofxImageSequence example using Muybridge horse
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//File format for the example frames is
	//frame01.png 
	//this creates a method call where the parameters
	//prefix is frame, file type is png, from frame 1 to 11, 2 digits in the number

	//sequence.loadSequence("frame", "png", 1, 11, 2);
	//sequence.preloadAllFrames();	//this way there is no stutter when loading frames
	sequence.enableThreadedLoad(true);
	sequence.setExtension("png");
	sequence.loadSequence("frames");

	//sequence.setFrameRate(10); //set to ten frames per second for Muybridge's horse.
	
	playing = false; //controls if playing automatically, or controlled by the mouse
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	if(sequence.isLoading()){
		ofBackground(255,0,0);
	}
	else{
		ofBackground(0);
		if(playing){
			//get the frame based on the current time and draw it
			sequence.getFrameForTime(ofGetElapsedTimef())->draw(0,0);
		}
		else {
			//get the sequence frame that maps to the mouseX position
			float percent = ofMap(mouseX, 0, ofGetWidth(), 0, 1.0, true);
		
			//draw it.
			sequence.getFrameAtPercent(percent)->draw(0, 0);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//hit any key to toggle playing
	playing = !playing;
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

