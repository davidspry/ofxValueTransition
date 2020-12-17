//  ofxValueTransitionDemo
//  Created by David Spry on 18/12/20.

#ifndef SIMPLEOSCILLATOR_H
#define SIMPLEOSCILLATOR_H

class SimpleOscillator
{
public:
    SimpleOscillator(float frequency)
    {
        setFrequency(frequency);
    }

public:
    inline float pull()
    {
        incrementPhase();
        return std::sinf(phase);
    }
    
public:
    inline void setFrequency(float frequency)
    {
        hertz = frequency;
        shift = frequency * TWO_PI / sampleRate;
    }

    inline void setSampleRate(float frequency)
    {
        sampleRate = frequency;
        setFrequency(hertz);
    }

private:
    inline void incrementPhase()
    {
        phase = phase + shift;
        phase = phase - static_cast<int>(phase > TWO_PI) * TWO_PI;
    }

private:
    float phase;
    float shift;
    float hertz;
    float sampleRate = 44100.0f;
};

#endif
