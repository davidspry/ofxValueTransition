# ofxValueTransition

An add-on for [openFrameworks](https://github.com/openframeworks/openFrameworks) that provides a thread-safe class for smoothly transitioning between two positive values over an arbitrary amount of time.

To use this add-on with openFrameworks:
  - Download the repository;
  - Move the contents of the repository to `{OF_ROOT}/addons/ofxValueTransition/`;
  - Add ofxValueTransition as an add-on to your project using the openFrameworks project generator.

See the included Xcode project, 'ofxValueTransitionDemo', for a simple usage demo including audio and animation.

## Usage

There are several ways to initialise a `ofxValueTransition` object, but they must use a floating point type.

```
#include "ofxValueTransition.hpp"

ofxValueTransition<float>  transition_float;
ofxValueTransition<double> transition_double;
```

An `ofxValueTransition` has a current value, a target value, and a duration value, which can be specified in either seconds or 'frames'. Naturally, these parameters can be set in a constructor and updated during run-time.

```
// Transition from 1.0 to 5.0 over 2.0 seconds.
ofxValueTransition<float> transition_a = {1.0, 5.0, 2.0};

// Initialise with value 5.0 and a transition duration (for subsequent transitions) of 4.5 seconds.
ofxValueTransition<float> transition_b = {5.0, 4.5};

// Transition from 60.0 to 30.0 over 3.0 seconds.
ofxValueTransition<float> transition_c;
transition_c.setParametersImmediately(60.0, 30.0, 3.0);

// Update `transition_a`'s target value and transition duration
transition_a.setDurationInSeconds(5.0);
transition_a.setTargetValue(25.0);
```

An `ofxValueTransition` expects to be called continuously at the given 'frame rate', so it's important to specify an appropriate value. In an audio context, the 'frame rate' may be the sample rate (e.g. 44,100Hz). In an animation context, it may be the animation's frame rate (e.g., 60Hz).

```
ofSetFrameRate(60);
ofxValueTransition<float> animation_transition;
animation_transition.setFrameRate(60);

ofxValueTransition<float> audio_transition;
audio_transition.setFrameRate(44100);
```

A transition is updated by successively calling the `get` method, which returns the current output value.
```
void draw()
{
  const auto hertz = transition.get();
  synthesiser.set_frequency(hertz);
}
```
