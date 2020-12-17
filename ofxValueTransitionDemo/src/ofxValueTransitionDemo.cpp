//  ofxValueTransitionDemo
//  Created by David Spry on 18/12/20.

#include "ofxValueTransitionDemo.h"

void ofxValueTransitionDemo::setup()
{
    initialisePaths();
    
    constexpr int frameRate = 60;
    ofSetFrameRate(frameRate);
    animationTransition.setFrameRate(frameRate);
    animationTransition.setParametersImmediately(10.0, 220.0, 2.0);
    
    constexpr int sampleRate = 44100;
    initialiseSoundStream(sampleRate);
    oscillator.setSampleRate((float) sampleRate);
    oscillatorTransition.setFrameRate((double) sampleRate);
    oscillatorTransition.setParametersImmediately(10.0, 220.0, 2.0);
}

void ofxValueTransitionDemo::draw()
{
    int x = ofGetWidth () * 0.5f;
    int y = ofGetHeight() * 0.5f;
    track.draw(x, y);
    
    std::string f;
    f.append("Oscillator: ");
    f.append(ofToString(oscillatorTransition.getValueWithoutUpdating(), 2) + "Hz\n\n");
    f.append("Use keys 1-5 to change the frequency.");
    const auto bounds = ofBitmapFont().getBoundingBox(f, 0, 0);
    ofDrawBitmapStringHighlight(f, x - bounds.width / 2, y - bounds.height / 2);

    const double hertz = animationTransition.get();
    const double frame = (double) ofGetFrameNum();
    const double radians = hertz * (double) TWO_PI / (double) ofGetFrameRate();
    const double theta = frame * radians;

    x = x * 0.5f + trackRadius * std::cosf(theta);
    y = y * 1.0f + trackRadius * std::sinf(theta);
    index.draw(x, y);
}

void ofxValueTransitionDemo::keyPressed(int key)
{
    double f;

    switch (key)
    {
        case 49: { f =  65.41; break; }
        case 50: { f =  98.00; break; }
        case 51: { f = 146.83; break; }
        case 52: { f = 220.00; break; }
        case 53: { f = 329.63; break; }
        default: return;
    }
    
    animationTransition .setTargetValue(f);
    oscillatorTransition.setTargetValue(f);
}

void ofxValueTransitionDemo::audioOut(ofSoundBuffer& buffer)
{
    const int channels = buffer.getNumChannels();
    const int frames   = buffer.getNumFrames();
    
    for (int k = 0; k < frames; ++k)
    {
        const float sample = oscillator.pull() * 0.50f;
        const double hz    = oscillatorTransition.get();
        oscillator.setFrequency(hz);

        for (int c = 0; c < channels; ++c)
            buffer[k * channels + c] = sample;
    }
}

void ofxValueTransitionDemo::initialisePaths()
{
    const int W = ofGetWidth();
    const int H = ofGetHeight();
    const int r = std::min(W, H);
    
    trackRadius = r / 4;
    indexRadius = r / 16;
    
    track.setFilled(false);
    track.setStrokeWidth(3.0f);
    track.setColor(ofColor::white);
    track.setCircleResolution(256);
    track.circle(0.0f, 0.0f, trackRadius);
    
    index.setFilled(true);
    index.setStrokeWidth(0.0f);
    index.setColor(ofColor::lightPink);
    index.setCircleResolution(256);
    index.circle(trackRadius, 0.0f, indexRadius);
}

void ofxValueTransitionDemo::initialiseSoundStream(const int& sampleRate)
{
    ofSoundStreamSettings settings;
        settings.setOutListener(this);
        settings.numInputChannels  = 0;
        settings.numOutputChannels = 2;
        settings.sampleRate = sampleRate;
        settings.bufferSize = 512;
    soundstream.stop();
    soundstream.setup(settings);
}
