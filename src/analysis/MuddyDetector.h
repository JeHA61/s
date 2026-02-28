#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class MuddyDetector
{
public:
    float score(const juce::AudioBuffer<float>& buffer, double sampleRate = 48000.0f) const;
};
