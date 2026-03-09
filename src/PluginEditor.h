#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

#include "PluginProcessor.h"
#include "engine/SuggestionEngine.h"
#include "ui/RationalePanel.h"

#include <vector>

class MixCopilotAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit MixCopilotAudioProcessorEditor(MixCopilotAudioProcessor&);
    ~MixCopilotAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void runAnalysis();
    std::string getSelectedStyleId() const;

    MixCopilotAudioProcessor& audioProcessor;
    SuggestionEngine suggestionEngine;

    juce::TextEditor promptEditor;
    juce::Label styleLabel;
    juce::ComboBox styleCombo;
    juce::TextButton analyzeButton { "Analyze" };
    juce::TextButton applyButton { "Apply" };
    juce::TextButton rollbackButton { "Rollback" };
    juce::ToggleButton bypassButton { "Bypass" };
    juce::Label statusLabel;
    RationalePanel rationalePanel;

    std::vector<std::string> presetIds;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MixCopilotAudioProcessorEditor)
};
