#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

#include "PluginProcessor.h"

class MixCopilotAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit MixCopilotAudioProcessorEditor(MixCopilotAudioProcessor&);
    ~MixCopilotAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    MixCopilotAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MixCopilotAudioProcessorEditor)
};
