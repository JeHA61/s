#include "analysis/MuddyDetector.h"

#include "audio/FeatureExtractor.h"

#include <algorithm>

float MuddyDetector::score(const juce::AudioBuffer<float>& buffer, double sampleRate) const
{
    FeatureExtractor extractor;
    const auto features = extractor.analyze(buffer, sampleRate);

    const auto maskingIndex = std::clamp((1500.0f - features.spectralCentroidHz) / 1500.0f, 0.0f, 1.0f);
    const auto muddynessScore = features.lowMidRatio * 0.7f + maskingIndex * 0.3f;
    return std::clamp(muddynessScore, 0.0f, 1.0f);
}
