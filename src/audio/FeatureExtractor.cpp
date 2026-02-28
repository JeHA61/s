#include "audio/FeatureExtractor.h"

#include <algorithm>
#include <cmath>

namespace
{
constexpr float kPi = 3.14159265358979323846f;
}

MixFeatures FeatureExtractor::analyze(const juce::AudioBuffer<float>& buffer, double sampleRate) const
{
    MixFeatures features {};

    if (buffer.getNumChannels() == 0 || buffer.getNumSamples() == 0 || sampleRate <= 0.0)
    {
        return features;
    }

    const auto fftSize = std::min(buffer.getNumSamples(), 2048);
    const auto* readPtr = buffer.getReadPointer(0);
    const auto maxBin = fftSize / 2;

    double totalEnergy = 0.0;
    double weightedFrequencySum = 0.0;
    double lowMidEnergy = 0.0;

    for (int bin = 0; bin < maxBin; ++bin)
    {
        double real = 0.0;
        double imag = 0.0;

        for (int n = 0; n < fftSize; ++n)
        {
            const auto angle = static_cast<float>(2.0 * kPi * static_cast<float>(bin) * static_cast<float>(n) / static_cast<float>(fftSize));
            const auto sample = static_cast<double>(readPtr[n]);
            real += sample * std::cos(angle);
            imag -= sample * std::sin(angle);
        }

        const auto energy = real * real + imag * imag;
        const auto frequency = static_cast<double>(bin) * sampleRate / static_cast<double>(fftSize);

        totalEnergy += energy;
        weightedFrequencySum += frequency * energy;
        if (frequency >= 150.0 && frequency <= 500.0)
        {
            lowMidEnergy += energy;
        }
    }

    if (totalEnergy <= 0.0)
    {
        return features;
    }

    features.spectralCentroidHz = static_cast<float>(weightedFrequencySum / totalEnergy);
    features.lowMidRatio = static_cast<float>(lowMidEnergy / totalEnergy);
    return features;
}
