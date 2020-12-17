//  ofxValueTransitionDemo
//  Created by David Spry on 18/12/20.

#ifndef OFXVALUETRANSITIONDEMO_H
#define OFXVALUETRANSITIONDEMO_H

#include "ofMain.h"
#include "SimpleOscillator.h"
#include "ofxValueTransition.hpp"

class ofxValueTransitionDemo: public ofBaseApp
{
public:
    void setup() override;
    void draw()  override;
    
public:
    void keyPressed(int key) override;
    
public:
    void audioOut(ofSoundBuffer& buffer) override;

private:
    void initialisePaths();
    void initialiseSoundStream(const int& sampleRate);
    
private:
    ofSoundStream soundstream;

private:
    ofPath track;
    ofPath index;
    int trackRadius;
    std::string label;

private:
    SimpleOscillator oscillator = {110.0f};
    ofxValueTransition<double> animationTransition;
    ofxValueTransition<double> oscillatorTransition;
};

#endif
