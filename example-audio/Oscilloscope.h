//
//  Oscilloscope.h
//  SYNTH
//
//  Created by Marek Bereza on 18/11/2019.
//

#pragma once

/**
 * Draws signals live, tries to line up to nearest zero crossing like a real oscilloscope
 */
class Oscilloscope {
public:
	mutex mut;
	vector<float> a;
	vector<float> b;
	vector<float> *r;
	vector<float> *s;
	int size = 0;
	
	void setup(int size) {
		
		r = &a;
		s = &b;
		this->size = size;
	}
	float last = 0.f;
	void process(const ofSoundBuffer &buff) {
		
		if(r->size()<size) {
			for(int i = 0; i < buff.size(); i++) {
				if(r->size()==0) {
					// wait for zero crossing
					if(buff[i]>0 && last<=0) {
						r->push_back(buff[i]);
					}
				} else {
					r->push_back(buff[i]);
				}
				last = buff[i];
			}
			mut.lock();
			if(r->size()>=size) {
				r->resize(size);
				swap(r, s);
				r->clear();
			}
			mut.unlock();
		}
	}
	
	void plot() {
		ofNoFill();
		mut.lock();
		if(s->size()>0) {
			
			ofBeginShape();
			for(int i = 0; i < s->size(); i++) {
				float x = ofMap(i, 0, s->size(), 0, ofGetWidth());
				float y = ofMap(s->at(i), -1, 1, ofGetHeight(), 0, true);
				ofVertex(x, y);
			}
			
			ofEndShape();
		}
		mut.unlock();
		ofFill();
	}
};
