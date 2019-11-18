//
//  DSP.h
//  SYNTH
//
//  Created by Marek Bereza on 18/11/2019.
//

#pragma once


class Oscillator {
public:
	double phase = 0;
	float frequency = 440;
	
	float getSample() {
		phase += frequency*M_PI * 2.0 / 44100.0;
		if(phase>=M_PI * 2.0) phase -= M_PI * 2.0;
		return phase / M_PI;
	}
};

class Filter {
public:
	float f, p, q;             //filter coefficients
	float b0, b1, b2, b3, b4;  //filter buffers (beware denormals!)
	float t1, t2;              //temporary buffers
	
	// Set coefficients given cutoff & resonance [0.0...1.0]
	float cutoff = 0.2;
	float resonance = 0.5;
	
	// http://www.musicdsp.org/en/latest/Filters/25-moog-vcf-variation-1.html
	float process(float in) {
		// Moog 24 dB/oct resonant lowpass VCF
		// References: CSound source code, Stilson/Smith CCRMA paper.
		// Modified by paul.kellett@maxim.abel.co.uk July 2000
		q = 1.0f - cutoff;
		p = cutoff + 0.8f * cutoff * q;
		f = p + p - 1.0f;
		q = resonance * (1.0f + 0.5f * q * (1.0f - q + 5.6f * q * q));

		in -= q * b4;                          //feedback
		t1 = b1;  b1 = (in + b0) * p - b1 * f;
		t2 = b2;  b2 = (b1 + t1) * p - b2 * f;
		t1 = b3;  b3 = (b2 + t2) * p - b3 * f;
				b4 = (b3 + t1) * p - b4 * f;
		b4 = b4 - b4 * b4 * b4 * 0.166667f;    //clipping
		b0 = in;

		return b4;
	}
};


