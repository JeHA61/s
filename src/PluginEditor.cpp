#include "PluginEditor.h"

#include <cmath>

MixCopilotAudioProcessorEditor::MixCopilotAudioProcessorEditor(MixCopilotAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    juce::ignoreUnused(audioProcessor);

    promptEditor.setText("소리가 너무 먹먹해");
    statusLabel.setText("Ready", juce::dontSendNotification);
    statusLabel.setJustificationType(juce::Justification::centredLeft);

    analyzeButton.onClick = [this] { runAnalysis(); };
    applyButton.onClick = [this] { statusLabel.setText("Applied suggestion.", juce::dontSendNotification); };
    rollbackButton.onClick = [this] { statusLabel.setText("Rolled back to previous state.", juce::dontSendNotification); };
    abToggleButton.onClick = [this] {
        statusLabel.setText(abToggleButton.getToggleState() ? "A/B: B" : "A/B: A", juce::dontSendNotification);
    };

    addAndMakeVisible(promptEditor);
    addAndMakeVisible(analyzeButton);
    addAndMakeVisible(applyButton);
    addAndMakeVisible(rollbackButton);
    addAndMakeVisible(abToggleButton);
    addAndMakeVisible(statusLabel);
    addAndMakeVisible(rationalePanel);

    setSize(760, 420);
}

MixCopilotAudioProcessorEditor::~MixCopilotAudioProcessorEditor() = default;

void MixCopilotAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff12151a));
    g.setColour(juce::Colours::white.withAlpha(0.85f));
    g.setFont(juce::FontOptions(19.0f));
    g.drawText("MixCopilot AU MVP", 16, 10, getWidth() - 32, 24, juce::Justification::left);
}

void MixCopilotAudioProcessorEditor::resized()
{
    juce::Rectangle<int> bounds(getLocalBounds().reduced(16));
    auto top = bounds.removeFromTop(32);
    juce::ignoreUnused(top);

    auto inputRow = bounds.removeFromTop(34);
    promptEditor.setBounds(inputRow.removeFromLeft(static_cast<int>(inputRow.getWidth() * 0.65f)));
    analyzeButton.setBounds(inputRow.removeFromLeft(10).removeFromRight(100));
    abToggleButton.setBounds(inputRow.removeFromRight(70));

    bounds.removeFromTop(10);
    auto statusRow = bounds.removeFromTop(24);
    statusLabel.setBounds(statusRow);

    bounds.removeFromTop(10);
    rationalePanel.setBounds(bounds.removeFromTop(static_cast<int>(bounds.getHeight() * 0.72f)));

    bounds.removeFromTop(10);
    auto actionsRow = bounds.removeFromTop(30);
    applyButton.setBounds(actionsRow.removeFromLeft(130));
    actionsRow.removeFromLeft(8);
    rollbackButton.setBounds(actionsRow.removeFromLeft(130));
}

void MixCopilotAudioProcessorEditor::runAnalysis()
{
    juce::AudioBuffer<float> mockBuffer(1, 4096);
    auto* writePtr = mockBuffer.getWritePointer(0);

    constexpr float pi = 3.14159265358979323846f;
    constexpr float sampleRate = 48000.0f;
    for (int sample = 0; sample < mockBuffer.getNumSamples(); ++sample)
    {
        const auto t = static_cast<float>(sample) / sampleRate;
        const auto lowMid = 0.9f * std::sin(2.0f * pi * 260.0f * t);
        const auto high = 0.1f * std::sin(2.0f * pi * 4200.0f * t);
        writePtr[sample] = lowMid + high;
    }

    const auto suggestion = suggestionEngine.suggest(mockBuffer, promptEditor.getText().toStdString(), sampleRate);
    rationalePanel.setSuggestion(suggestion);
    statusLabel.setText("Suggestion generated. Review and A/B check.", juce::dontSendNotification);
}
