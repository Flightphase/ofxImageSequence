/**
 *  ofxImageSequence.cpp
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
 *  ofxImageSequence is a class for easily loading a series of image files
 *  and accessing them like you would frames of a movie.
 *  
 *  This class loads only textures to the graphics card and does not store pixel data in memory. This helps with
 *  fast, random access drawing of seuqences
 *  
 *  Why would you use this instead of a movie file? A few reasons,
 *  If you want truly random frame access with no lag on large images, ofxImageSequence allows it
 *  If you need a movie with alpha channel the only readily available codec is Animation (PNG) which is slow at large resolutions, so this class can help with that
 *  If you want to easily access frames based on percents this class makes that easy
 *  
 */

#include "ofxImageSequence.h"

ofxImageSequence::ofxImageSequence()
{
	loaded = false;
	scale = 1.0;
	frameRate = 30.0f;
	nonDefaultFiltersUsed = false;
	lastFrameLoaded = -1;
	loader.setUseTexture(false);
	currentFrame = 0;
}

ofxImageSequence::~ofxImageSequence()
{
	if(loaded){
		unloadSequence();
	}
}

bool ofxImageSequence::loadSequence(string prefix, string filetype,  int startDigit, int endDigit)
{
	return loadSequence(prefix, filetype, startDigit, endDigit, 0);
}

bool ofxImageSequence::loadSequence(string prefix, string filetype,  int startDigit, int endDigit, int numDigits)
{
	unloadSequence();

	char imagename[1024];
	stringstream format;
	int numFiles = endDigit - startDigit+1;
	if(numFiles <= 0 ){
		ofLogError("ofxImageSequence::loadSequence") << "No image files found.";
		return false;
	}

	if(numDigits != 0){
		format <<prefix<<"%0"<<numDigits<<"d."<<filetype;
	} else{
		format <<prefix<<"%d."<<filetype; 
	}
	
	for(int i = startDigit; i <= endDigit; i++){
		sprintf(imagename, format.str().c_str(), i);
		filenames.push_back(imagename);
		sequence.push_back(ofPixels());
	}
	
	loaded = true;
	
	lastFrameLoaded = -1;
	loadFrame(0);
	
	width  = sequence[0].getWidth();
	height = sequence[0].getHeight();
	return true;
}

bool ofxImageSequence::loadSequence(string _folder)
{
	
	unloadSequence();

    ofDirectory dir;
	if(extension != ""){
		dir.allowExt(extension);
	}
	
	if(!ofFile(_folder).exists()){
		ofLogError("ofxImageSequence::loadSequence") << "Could not find folder " << _folder;
		return false;
	}

    int numFiles = dir.listDir(_folder);
    if(numFiles == 0) {
		ofLogError("ofxImageSequence::loadSequence") << "No image files found in " << _folder;
		return false;
	}

    // read the directory for the images
    // we know that they are named in seq
	#ifdef TARGET_LINUX
	dir.sort();
	#endif
	for(int i = 0; i<dir.numFiles(); i++) {
        filenames.push_back( dir.getPath(i) );
		sequence.push_back(ofPixels());
    }
   	
    loaded = true;
	
	lastFrameLoaded = -1;
	loadFrame(0);
	
	width  = sequence[0].getWidth();
	height = sequence[0].getHeight();
}

void ofxImageSequence::setExtension(string ext)
{
	extension = ext;
}

void ofxImageSequence::setMinMagFilter(int newMinFilter, int newMagFilter)
{
	minFilter = newMinFilter;
	magFilter = newMagFilter;
	nonDefaultFiltersUsed = true;
}

void ofxImageSequence::preloadAllFrames()
{
	if(!loaded){
		ofLogError("ofxImageSequence::loadFrame") << "Calling preloadAllFrames on unitialized image sequence.";
		return;
	}
	
	ofImage m;
	m.setUseTexture(false);
	for(int i = 0; i < sequence.size(); i++){
		if(m.loadImage(filenames[i])){
			sequence[i] = m.getPixelsRef();
		}
		else{
			ofLogError("ofxImageSequence::loadFrame") << "Image failed to load: " << filenames[i];		
		}
	}
}

void ofxImageSequence::loadFrame(int imageIndex)
{
	if(lastFrameLoaded == imageIndex){
		return;
	}

	if(imageIndex < 0 || imageIndex >= sequence.size()){
		ofLogError("ofxImageSequence::loadFrame") << "Calling a frame out of bounds: " << imageIndex;
		return;
	}

	if(!sequence[imageIndex].isAllocated()){
		ofImage m;
		m.setUseTexture(false);

		if(m.loadImage(filenames[imageIndex])){
			sequence[imageIndex] = m.getPixelsRef();
		}
		else{
			ofLogError("ofxImageSequence::loadFrame") << "Image failed to load: " << filenames[imageIndex];		
		}
	}

	if(nonDefaultFiltersUsed){
		texture.setTextureMinMagFilter(minFilter, magFilter);
	}

	texture.loadData(sequence[imageIndex]);
	
	lastFrameLoaded = imageIndex;

	/*
	//if(sequence[imageIndex] != NULL){
	//	ofLogError("ofxImageSequence::loadFrame") << "warning calling load frame on a non null index";
	//	return;
	//}

	for(int i = lastFrameLoaded+1; i <= imageIndex; i++){
		sequence[i] = new ofTexture();
		if(!loader.loadImage( *filenames[i] )){
			ofLogError("ofxImageSequence::loadFrame") << "failed to load image %s. returning " << *filenames[i];
			return;
		}
		
		if(scale != 1.0){
			loader.resize(int(loader.getWidth()*scale), int(loader.getHeight()*scale));
		}
		
		sequence[i]->allocate( loader.getWidth(), loader.getHeight(), imageTypeToGLType(loader.type) );				
		sequence[i]->loadData( loader.getPixels(), loader.getWidth(), loader.getHeight(), imageTypeToGLType(loader.type) );
		if(nonDefaultFiltersUsed){
			sequence[i]->setTextureMinMagFilter(minFilter, magFilter);
		}
	}
	
	lastFrameLoaded = imageIndex;
	*/
}

float ofxImageSequence::getPercentAtFrameIndex(int index)
{
	return ofMap(index, 0, sequence.size()-1, 0, 1.0, true);
}

float ofxImageSequence::getWidth()
{
	return width;
}

float ofxImageSequence::getHeight()
{
	return height;
}

void ofxImageSequence::unloadSequence()
{
	/*
	for(int i = 0; i < sequence.size(); i++){
		if(sequence[i] != NULL){
			delete sequence[i];
		}
		delete filenames[i];
	}
	*/

	sequence.clear();
	filenames.clear();
	
	loaded = false;
	width = 0;
	height = 0;

}

void ofxImageSequence::setFrameRate(float rate)
{
	frameRate = rate;
}

int ofxImageSequence::getFrameIndexAtPercent(float percent)
{
    if (percent < 0.0 || percent > 1.0) percent -= floor(percent);

	return MIN((int)(percent*sequence.size()), sequence.size()-1);
}

//deprecated
ofTexture* ofxImageSequence::getFrameAtPercent(float percent)
{
	setFrameAtPercent(percent);
	return &getTextureReference();
}

//deprecated
ofTexture* ofxImageSequence::getFrameForTime(float time)
{
	setFrameForTime(time);
	return &getTextureReference();
}

//deprecated
ofTexture* ofxImageSequence::getFrame(int index)
{
	setFrame(index);
	return &getTextureReference();
}

ofTexture& ofxImageSequence::getTextureForFrame(int index)
{
	setFrame(index);
	return getTextureReference();
}

ofTexture& ofxImageSequence::getTextureForTime(float time)
{
	setFrameForTime(time);
	return getTextureReference();
}

ofTexture& ofxImageSequence::getTextureForPercent(float percent){
	setFrameAtPercent(percent);
	return getTextureReference();
}

void ofxImageSequence::setFrame(int index)
{
	if(!loaded){
		ofLog(OF_LOG_ERROR, "ofxImageSequence - Calling getFrame on unitialized image sequence.");
		return;
	}

	if(index < 0){
		ofLog(OF_LOG_ERROR, "ofxImageSequence - Asking for negative index.");
		return;
	}
	
	index %= getTotalFrames();
	
	//if(sequence[index] == NULL){
	//}
	
	loadFrame(index);
	currentFrame = index;
}

void ofxImageSequence::setFrameForTime(float time)
{
	float totalTime = sequence.size() / frameRate;
	float percent = time / totalTime;
	return setFrameAtPercent(percent);	
}

void ofxImageSequence::setFrameAtPercent(float percent)
{
	setFrame(getFrameIndexAtPercent(percent));	
}

ofTexture& ofxImageSequence::getTextureReference()
{
	return texture;
}

float ofxImageSequence::getLengthInSeconds()
{
	return getTotalFrames() / frameRate;
}

int ofxImageSequence::getTotalFrames()
{
	return sequence.size();
}

int ofxImageSequence::imageTypeToGLType(int imageType)
{
	switch (imageType) {
		case OF_IMAGE_GRAYSCALE:
			return GL_LUMINANCE;
		case OF_IMAGE_COLOR:
			return GL_RGB;
		case OF_IMAGE_COLOR_ALPHA:
			return GL_RGBA;
		default:
			ofLog(OF_LOG_ERROR, "ofxImageSequence - unsupported image type for image");
			return GL_RGB;
	}
}

bool ofxImageSequence::isLoaded(){						//returns true if the sequence has been loaded
    return loaded;
}