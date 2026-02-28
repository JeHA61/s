#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include <string>

#include "engine/SuggestionEngine.h"

std::string formatReasonText(const std::string& reason);

class RationalePanel final : public juce::Component
{
public:
    RationalePanel();

    void setSuggestion(const Suggestion& suggestion);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::Label rationaleLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RationalePanel)
};
