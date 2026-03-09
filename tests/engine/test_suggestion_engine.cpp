#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <algorithm>

#include "engine/SuggestionEngine.h"

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
        const auto high = 0.1f * std::sin(2.0f * pi * 4500.0f * t);
        writePtr[sample] = lowMid + high;
    }

    return buffer;
}
} // namespace

TEST_CASE("SuggestionEngine returns low-mid rationale and at least one action")
{
    SuggestionEngine engine;
    const auto suggestion = engine.suggest(makeLowMidHeavyBuffer(), "소리가 너무 먹먹해");
    REQUIRE(suggestion.reason.find("low-mid") != std::string::npos);
    REQUIRE(suggestion.actions.size() > 0);
}

TEST_CASE("SuggestionEngine applies style presets when generating actions")
{
    SuggestionEngine engine;
    const auto warmPop = engine.suggest(makeLowMidHeavyBuffer(), "소리가 너무 먹먹해", 48000.0, "warm-pop");
    const auto modernPop = engine.suggest(makeLowMidHeavyBuffer(), "소리가 너무 먹먹해", 48000.0, "modern-pop");

    REQUIRE(modernPop.actions.size() == warmPop.actions.size() + 1);
    REQUIRE(std::find_if(warmPop.actions.begin(), warmPop.actions.end(), [](const SuggestedAction& action) {
                return action.description.find("Gentle top air") != std::string::npos;
            }) != warmPop.actions.end());
    REQUIRE(std::find_if(modernPop.actions.begin(), modernPop.actions.end(), [](const SuggestedAction& action) {
                return action.description.find("Attack transient") != std::string::npos;
            }) != modernPop.actions.end());
}
