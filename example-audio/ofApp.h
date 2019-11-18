#pragma once

#include "ofMain.h"
#include "ofxCppSketchSoundStream.h"
#include "DSP.h"
#include "Oscilloscope.h"

class ofApp : public ofBaseApp {

public:
	
	ofxCppSketchSoundStream stream;
	
	Oscillator osc;
	Filter filter;
	Oscilloscope oscilloscope;
	
	void setup() override {
		
		ofSoundStreamSettings settings;
		settings.numOutputChannels = 2;
		settings.setOutListener(this);
		
		stream.setup(settings);
		oscilloscope.setup(2048);
	}
	
	float targetFreq = 440.f;
	float targetCutoff = 0.1;
	
	void audioOut(ofSoundBuffer &buffer) override {
		for(int i = 0; i < buffer.size() / 2; i++) {
			osc.frequency = osc.frequency * 0.999 + targetFreq * 0.001;
			filter.cutoff = filter.cutoff * 0.999 + targetCutoff * 0.001;
			float out = filter.process(  osc.getSample()  ) * 0.2;
			buffer[i*2] = buffer[i*2+1] = out;
		}
		oscilloscope.process(buffer);
		buffer *= 0.2;
	}
	
	
	void draw() override {
		ofSetColor(255);
		ofDrawBitmapString("Mouse X: frequency  " + ofToString(targetFreq, 0) + "Hz", 20, 20);
		ofDrawBitmapString("Mouse Y: filter     " + ofToString(targetCutoff * 100.f, 0) + "%", 20, 40);
		oscilloscope.plot();
	}
	
	
	void mouseMoved(int x, int y) override {
		targetFreq = ofMap(x, 0, ofGetWidth(), 70, 1000, true);
		targetCutoff = ofMap(y, 0, ofGetHeight(), 0.5, 0.01, true);
	}
};
