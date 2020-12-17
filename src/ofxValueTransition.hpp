#ifndef OFXVALUETRANSITION_HPP
#define OFXVALUETRANSITION_HPP

#include "ofMain.h"
#include <type_traits>

#define CLASS_LABEL "[ofxValueTransition]"
#define FLOAT_ERROR CLASS_LABEL " The type must be float, double, or long double."
#define VALUE_ERROR CLASS_LABEL " The initial and target values must be positive."
#define FRAME_ERROR CLASS_LABEL " The refresh rate must be a positive value."
#define CLOCK_ERROR CLASS_LABEL " The transition time must be a nonnegative value."

/// @brief An object representing a smooth transition between two positive values.

template <typename T>
class ofxValueTransition
{
public:
    /// @brief Construct a `ValueTransition` with output value 1.0, frame rate 60, and transition duration 500ms.

    ofxValueTransition()
    {
        static_assert(std::is_floating_point<T>::value, FLOAT_ERROR);
    }
    
    /// @brief Construct a `ValueTransition` with the given output value and the given duration in seconds.
    /// @param targetValue The desired output value.
    /// @param durationInSeconds The desired transition duration in seconds.

    ofxValueTransition(T targetValue, T durationInSeconds) noexcept(false)
    {
        static_assert(std::is_floating_point<T>::value, FLOAT_ERROR);
        
        setParametersImmediately(targetValue, durationInSeconds);
    }
    
    /// @brief Construct a `ValueTransition` with the given initial value, output value, and duration in seconds.
    /// @param initialValue An initial value.
    /// @param targetValue The desired output value.
    /// @param durationInSeconds The desired transition duration in seconds.

    ofxValueTransition(T initialValue, T targetValue, T durationInSeconds) noexcept(false)
    {
        static_assert(std::is_floating_point<T>::value, FLOAT_ERROR);
        
        setParametersImmediately(initialValue, targetValue, durationInSeconds);
    }
    
public:
    /// @brief Get the next value from the transition.

    [[nodiscard]] inline T get() noexcept
    {
        if (timeInFrames > 0)
        {
            value        = value * delta;
            timeInFrames = timeInFrames - 1;
            
            return value.load();
        }

        return target.load();
    }
    
    /// @brief Get the current output value without updating the state of the transition.

    [[nodiscard]] inline T getValueWithoutUpdating() const noexcept
    {
        return value.load();
    }

    /// @brief Get the output target value.

    [[nodiscard]] inline T getTarget() const noexcept
    {
        return target.load();
    }
    
    /// @brief Indicate whether the value transition has finished or not.

    [[nodiscard]] inline const bool isComplete() const noexcept
    {
        return timeInFrames <= 0;
    }

public:
    /// @brief Set a new transition duration in seconds.
    /// @param durationInSeconds The desired transition length in seconds.
    /// @note  `get` must be called continuously at the given frame rate in order to achieve this duration.

    inline void setDurationInSeconds(T durationInSeconds) noexcept(false)
    {
        setTargetValue(target.load(), durationInSeconds);
    }
    
    /// @brief Set a new transition duration time in 'frames'
    /// @param durationInFrames The desired transition length in frames.
    /// @seealso `void setFrameRate (unsigned int framesPerSecond)`

    inline void setDurationInFrames(unsigned int durationInFrames)
    {
        const T durationInSeconds = static_cast<T>(durationInFrames)
                                  / static_cast<T>(frameRate);

        setTargetValue(target.load(), durationInSeconds);
    }

    /// @brief Set a new target value
    /// @param targetValue The desired output target value

    inline void setTargetValue(T targetValue) noexcept(false)
    {
        setTargetValue(targetValue, timeInSeconds);
    }
    
    /// @brief Set a new target value and a new duration value.
    /// @param targetValue The desired output target value
    /// @param durationInSeconds The duration of the transition from the current output value (in seconds).

    inline void setTargetValue(T targetValue, T durationInSeconds) noexcept(false)
    {
        setParametersImmediately(value.load(), targetValue, durationInSeconds);
    }
    
    /// @brief Set the `ValueTransition`'s parameters immediately.
    /// @param targetValue The desired output value.
    /// @param durationInSeconds The desired duration value (in seconds).
    /// @note  The current value (i.e. the initial output value) will be set to the given target value.

    inline void setParametersImmediately(T targetValue, T durationInSeconds) noexcept(false)
    {
        setParametersImmediately(targetValue, targetValue, durationInSeconds);
    }
    
    /// @brief Set the `ValueTransition`'s parameters immediately.
    /// @param initialValue The initial output value.
    /// @param targetValue The desired output target value.
    /// @param durationInSeconds The desired duration value (in seconds).

    void setParametersImmediately(T initialValue, T targetValue, T durationInSeconds) noexcept(false)
    {
        if (!(value > 0.0 && targetValue > 0.0))
            throw std::invalid_argument(VALUE_ERROR);
        
        if (durationInSeconds < 0.0)
            throw std::invalid_argument(CLOCK_ERROR);
        
        value.store(initialValue);
        target.store(targetValue);
        timeInSeconds = durationInSeconds;
        timeInFrames  = static_cast<int> (timeInSeconds * frameRate);
        computeDelta();
    }

    /// @brief Set the rate at which the `ValueTransition`'s `get` function will be called.
    /// @param framesPerSecond The number of times per second that the
    /// `ValueTransition`'s `get` function will be called. This may be a frame rate (e.g. 60fps)
    /// in an animation context or a sample rate (e.g. 48,000Hz) in an audio context.

    void setFrameRate(unsigned int framesPerSecond) noexcept(false)
    {
        if (!(framesPerSecond > 0))
            throw std::invalid_argument(FRAME_ERROR);
        
        frameRate = framesPerSecond;
    }

private:
    /// @author ROLI Ltd.
    /// =================================================================================
    /// @brief Compute the value that `value` should be multiplied by in order to reach `target` in `time` frames.
    /// =================================================================================
    /// ISC licence and source: "juce_SmoothedValue.h" in: <https://github.com/juce-framework/JUCE/>
    /// =================================================================================

    void computeDelta()
    {
        delta = std::exp((std::log(target) - std::log(value)) / timeInFrames);
    }

private:
    std::atomic<T> delta  = {1.0f};
    std::atomic<T> value  = {1.0f};
    std::atomic<T> target = {1.0f};

private:
    int frameRate     = 60;
    int timeInFrames  = 30;
    T   timeInSeconds = 0.50;
};

#endif
