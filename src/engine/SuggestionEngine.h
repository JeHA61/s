#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

#include <string>
#include <vector>

struct SuggestedAction
{
    std::string description;
};

struct Suggestion
{
    std::string reason;
    std::string impact;
    float confidence {};
    std::vector<SuggestedAction> actions;
};

class SuggestionEngine
{
public:
    Suggestion suggest(const juce::AudioBuffer<float>& buffer, const std::string& prompt, double sampleRate = 48000.0) const;
};
