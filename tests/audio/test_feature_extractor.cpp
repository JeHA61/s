#include <catch2/catch_test_macros.hpp>

#include <cmath>

#include "audio/FeatureExtractor.h"

namespace
{
juce::AudioBuffer<float> makeSineBuffer(float frequencyHz, double sampleRate = 48000.0, int numSamples = 4096)
{
    juce::AudioBuffer<float> buffer(1, numSamples);
    auto* writePtr = buffer.getWritePointer(0);
    constexpr float pi = 3.14159265358979323846f;
    for (int sample = 0; sample < numSamples; ++sample)
    {
        const auto phase = static_cast<float>(2.0 * pi * static_cast<float>(frequencyHz) * static_cast<float>(sample) / static_cast<float>(sampleRate));
        writePtr[sample] = std::sin(phase);
    }
    return buffer;
}
} // namespace

TEST_CASE("FeatureExtractor returns spectral centroid and low-mid energy ratio")
{
    FeatureExtractor extractor;
    const auto features = extractor.analyze(makeSineBuffer(220.0f));
    REQUIRE(features.spectralCentroidHz > 0.0f);
    REQUIRE(features.lowMidRatio >= 0.0f);
}
