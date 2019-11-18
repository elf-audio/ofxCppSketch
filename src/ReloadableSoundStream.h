//
//  ReloadableSoundStream.h
//  deathstortion
//
//  Created by Marek Bereza on 13/11/2019.
//

#pragma once
#include "ofRtAudioSoundStream.h"

class ReloadableSoundStream : public ofBaseSoundStream, public ofBaseSoundInput, public ofBaseSoundOutput {
public:
	std::vector<ofSoundDevice> getDeviceList(ofSoundDevice::Api api) const override {return rtaStream.getDeviceList(api); }
	bool isSetup = false;
	bool isRunning = false;
	
	
	
	
	void setInput(ofBaseSoundInput * soundInput) override {
		printf("You can't use setInput with ofxCppSketchSoundStream - use ofSoundStreamSettings instead\n");
		assert(0);
	}

	void setOutput(ofBaseSoundOutput * soundOutput) override {
		printf("You can't use setOutput with ofxCppSketchSoundStream - use ofSoundStreamSettings instead\n");
		assert(0);
	}
	std::function<void(ofSoundBuffer &)> inCallback;
	std::function<void(ofSoundBuffer &)> outCallback;

	bool setup(const ofSoundStreamSettings & settings) override {
		printf("reloadable sound stream at %x\n", this);
		if(!isSetup) {
			isSetup = true;
			auto s = settings; // non-const copy of settings
			
			//ofxCppSketchSoundStream::audioMutex.lock();
			this->inCallback = s.inCallback;
			this->outCallback = s.outCallback;
			//ofxCppSketchSoundStream::audioMutex.unlock();
			
			s.setInListener(this);
			s.setOutListener(this);
			return rtaStream.setup(s);
		} else {
		//	ofxCppSketchSoundStream::audioMutex.lock();
			
			this->inCallback = settings.inCallback;
			this->outCallback = settings.outCallback;
		//	ofxCppSketchSoundStream::audioMutex.unlock();
			return true;
		}
	}

	void audioIn( ofSoundBuffer& buffer ) override {
		if(audioMutex.try_lock()) {
			if(inCallback) inCallback(buffer);
			audioMutex.unlock();
		}
		
	}
	
	void audioOut( ofSoundBuffer& buffer ) override {
		if(audioMutex.try_lock()) {
			if(outCallback) outCallback(buffer);
			audioMutex.unlock();
		}
	}

	
	void start() override {
		if(!isRunning) {
			rtaStream.start();
			isRunning = true;
		}
	}
	void stop() override { /*rtaStream.stop();*/ }
	void close() override { /*rtaStream.close();*/ }

	uint64_t getTickCount() const override { return rtaStream.getTickCount(); }

	int getNumInputChannels() const override {return rtaStream.getNumInputChannels(); }
	int getNumOutputChannels() const override { return rtaStream.getNumOutputChannels(); }
	int getSampleRate() const override {return rtaStream.getSampleRate(); }
	int getBufferSize() const override {return rtaStream.getBufferSize(); }
	ofSoundDevice getInDevice() const override {return rtaStream.getInDevice(); }
	ofSoundDevice getOutDevice() const override { return rtaStream.getOutDevice(); }
	
	ofRtAudioSoundStream rtaStream;
	mutex audioMutex;

};


