#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

#include "PluginProcessor.h"
#include "engine/SuggestionEngine.h"
#include "ui/RationalePanel.h"

class MixCopilotAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit MixCopilotAudioProcessorEditor(MixCopilotAudioProcessor&);
    ~MixCopilotAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void runAnalysis();

    MixCopilotAudioProcessor& audioProcessor;
    SuggestionEngine suggestionEngine;

    juce::TextEditor promptEditor;
    juce::TextButton analyzeButton { "Analyze" };
    juce::TextButton applyButton { "Apply" };
    juce::TextButton rollbackButton { "Rollback" };
    juce::ToggleButton abToggleButton { "A/B" };
    juce::Label statusLabel;
    RationalePanel rationalePanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MixCopilotAudioProcessorEditor)
};
