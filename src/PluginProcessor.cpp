#include "PluginEditor.h"
#include "PluginProcessor.h"

MixCopilotAudioProcessor::MixCopilotAudioProcessor()
    : AudioProcessor(BusesProperties()
          .withInput("Input", juce::AudioChannelSet::stereo(), true)
          .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

MixCopilotAudioProcessor::~MixCopilotAudioProcessor() = default;

const juce::String MixCopilotAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MixCopilotAudioProcessor::acceptsMidi() const
{
    return false;
}

bool MixCopilotAudioProcessor::producesMidi() const
{
    return false;
}

bool MixCopilotAudioProcessor::isMidiEffect() const
{
    return false;
}

double MixCopilotAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MixCopilotAudioProcessor::getNumPrograms()
{
    return 1;
}

int MixCopilotAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MixCopilotAudioProcessor::setCurrentProgram(int)
{
}

const juce::String MixCopilotAudioProcessor::getProgramName(int)
{
    return {};
}

void MixCopilotAudioProcessor::changeProgramName(int, const juce::String&)
{
}

void MixCopilotAudioProcessor::prepareToPlay(double, int)
{
}

void MixCopilotAudioProcessor::releaseResources()
{
}

bool MixCopilotAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto mainIn = layouts.getMainInputChannelSet();
    const auto mainOut = layouts.getMainOutputChannelSet();
    return mainIn == mainOut && (mainOut == juce::AudioChannelSet::mono() || mainOut == juce::AudioChannelSet::stereo());
}

void MixCopilotAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    for (auto channel = getTotalNumInputChannels(); channel < getTotalNumOutputChannels(); ++channel)
    {
        buffer.clear(channel, 0, buffer.getNumSamples());
    }
}

bool MixCopilotAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* MixCopilotAudioProcessor::createEditor()
{
    return new MixCopilotAudioProcessorEditor(*this);
}

void MixCopilotAudioProcessor::getStateInformation(juce::MemoryBlock&)
{
}

void MixCopilotAudioProcessor::setStateInformation(const void*, int)
{
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MixCopilotAudioProcessor();
}
