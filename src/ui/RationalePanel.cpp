#include "ui/RationalePanel.h"

#include <sstream>

std::string formatReasonText(const std::string& reason)
{
    return "왜: " + reason;
}

RationalePanel::RationalePanel()
{
    addAndMakeVisible(rationaleLabel);
    rationaleLabel.setJustificationType(juce::Justification::topLeft);
    rationaleLabel.setText("왜: 분석 결과가 여기에 표시됩니다.", juce::dontSendNotification);
}

void RationalePanel::setSuggestion(const Suggestion& suggestion)
{
    std::ostringstream builder;
    builder << formatReasonText(suggestion.reason) << "\n";
    builder << "기대 효과: " << suggestion.impact << "\n";
    builder << "신뢰도: " << static_cast<int>(suggestion.confidence * 100.0f) << "%\n";
    builder << "제안:\n";

    for (const auto& action : suggestion.actions)
    {
        builder << "- " << action.description << "\n";
    }

    rationaleLabel.setText(builder.str(), juce::dontSendNotification);
}

void RationalePanel::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::darkslategrey);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 8.0f);
}

void RationalePanel::resized()
{
    rationaleLabel.setBounds(getLocalBounds().reduced(10));
}
