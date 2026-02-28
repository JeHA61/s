#include "PluginEditor.h"

MixCopilotAudioProcessorEditor::MixCopilotAudioProcessorEditor(MixCopilotAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    juce::ignoreUnused(audioProcessor);
    setSize(480, 240);
}

MixCopilotAudioProcessorEditor::~MixCopilotAudioProcessorEditor() = default;

void MixCopilotAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(18.0f));
    g.drawText("MixCopilot AU MVP", getLocalBounds(), juce::Justification::centred);
}

void MixCopilotAudioProcessorEditor::resized()
{
}
