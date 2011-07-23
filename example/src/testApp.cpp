/**
 *  testApp.cpp
 *
 *	ofxImageSequence example project
 *
 * Created by James George, http://www.jamesgeorge.org
 * in collaboration with FlightPhase http://www.flightphase.com
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

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//File format for the example frames is
	//frame01.png 
	//this creates a method call where the parameters
	//prefix is frame, file type is png, from frame 1 to 11, 2 digits in the number
	sequence.loadSequence("frame", "png", 1, 11, 2);
	sequence.preloadAllFrames();	//this way there is no stutter when loading frames
	sequence.setFrameRate(10); //set to ten frames per second for Muybridge's horse.
	
	playing = false; //controls if playing automatically, or controlled by the mouse
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	
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

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	//hit any key to toggle playing
	playing = !playing;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

