/**
 *  ofxImageSequence.h
 *
 * Created by James George, http://www.jamesgeorge.org
 * in collaboration with Flightphase http://www.flightphase.com
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
 *  If you want truly random frame access with no lag on large images, ofxImageSequence is a good way to go
 *  If you need a movie with alpha channel the only readily available codec is Animation (PNG) which is slow at large resolutions, so this class can help with that
 *  If you want to easily access frames based on percents this class makes that easy
 * 
 * //TODO: Extend ofBaseDraws
 * //TODO: experiment with storing pixels intead of textures and doing upload every frame
 * //TODO: integrate ofDirectory to API
 */

#pragma once

#include "ofMain.h"

class ofxImageSequence : public ofBaseHasTexture{
  public:
	ofxImageSequence();
	~ofxImageSequence();
	
	/**
	 *	use this method to load sequences formatted like:
	 *	path/to/images/myImage8.png
	 *	path/to/images/myImage9.png
	 *	path/to/images/myImage10.png
	 *
	 *	for this sequence the parameters would be:
	 *	prefix		=> "path/to/images/myImage"
	 *	filetype	=> "png"
	 *	startIndex	=> 8
	 *	endIndex	=> 10
	 */
	void loadSequence(string prefix, string filetype, int startIndex, int endIndex);

	/**
	 *	Use this functiont o load sequences formatted like
	 *
	 *	path/to/images/myImage004.jpg
	 *	path/to/images/myImage005.jpg
	 *	path/to/images/myImage006.jpg
	 *	path/to/images/myImage007.jpg
	 *
	 *	for this sequence the parameters would be:
	 *	prefix		=> "path/to/images/myImage"
	 *	filetype	=> "jpg"
	 *	startIndex	=> 4
	 *	endIndex	=> 7
	 *	numDigits	=> 3
	 */
	void loadSequence(string prefix, string filetype, int startIndex, int endIndex, int numDigits);
	
	void unloadSequence();			//clears out all frames and frees up memory
	void preloadAllFrames();		//immediately loads all frames in the sequence, memory intensive but fastest scrubbing
	
	void setFrameRate(float rate); //used for getting frames by time, default is 30fps	

	//these get textures, but also change the
	ofTexture* getFrame(int index);					//returns a frame at a given index
	ofTexture* getFrameForTime(float time);			//returns a frame at a given time, used setFrameRate to set time
	ofTexture* getFrameAtPercent(float percent);	//returns a frame at a percent (0.0 - 1.0) based on the number of frames in the sequence

	//if usinsg getTextureRef() use these to change the internal state
	void setFrame(int index);					
	void setFrameForTime(float time);			
	void setFrameAtPercent(float percent);
	
	virtual ofTexture& getTextureReference();
	virtual void setUseTexture(bool bUseTex){/* not used */};
	
	int getFrameIndexAtPercent(float percent);	//returns percent (0.0 - 1.0) for a given frame
	float getPercentAtFrameIndex(int index);	//returns a frame index for a percent
	
	int getTotalFrames();					//returns how many frames are in the sequence
	float getLengthInSeconds();				//returns the sequence duration based on frame rate
	
	float getWidth();						//returns the width/height of the sequence
	float getHeight();
	bool isLoaded();						//returns true if the sequence has been loaded
	
	void loadFrame(int imageIndex);			//allows you to load (cache) a frame to avoid a stutter when loading. use this to "read ahead" if you want
	
	void setMinMagFilter(int minFilter, int magFilter);
	//allows you to scale all the images by a percent
	//Make sure to set scale before you load the sequence
	//the default is 1.0
	float scale;
	
  protected:
	
	int currentFrame;
	
	ofImage	loader;
	vector<ofTexture*> sequence;
	vector<string*> filenames;

	int imageTypeToGLType(int imageType);
	float width, height;
	bool loaded;
	int lastFrameLoaded;
	float frameRate;
	
	int minFilter;
	int magFilter;
	bool nonDefaultFiltersUsed;
};



