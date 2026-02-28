#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

struct MixFeatures
{
    float spectralCentroidHz {};
    float lowMidRatio {};
};

class FeatureExtractor
{
public:
    MixFeatures analyze(const juce::AudioBuffer<float>& buffer, double sampleRate = 48000.0) const;
};
