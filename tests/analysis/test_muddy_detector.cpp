#include <catch2/catch_test_macros.hpp>

#include <cmath>

#include "analysis/MuddyDetector.h"

namespace
{
juce::AudioBuffer<float> makeLowMidHeavyBuffer(double sampleRate = 48000.0, int numSamples = 4096)
{
    juce::AudioBuffer<float> buffer(1, numSamples);
    auto* writePtr = buffer.getWritePointer(0);
    constexpr float pi = 3.14159265358979323846f;

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const auto t = static_cast<float>(sample) / static_cast<float>(sampleRate);
        const auto lowMid = 0.9f * std::sin(2.0f * pi * 250.0f * t);
        const auto high = 0.1f * std::sin(2.0f * pi * 5000.0f * t);
        writePtr[sample] = lowMid + high;
    }

    return buffer;
}
} // namespace

TEST_CASE("MuddyDetector flags crowded low-mid content")
{
    MuddyDetector detector;
    const auto score = detector.score(makeLowMidHeavyBuffer());
    REQUIRE(score > 0.65f);
}
