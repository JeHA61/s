#include "engine/SuggestionEngine.h"

#include "analysis/MuddyDetector.h"
#include "nlp/IntentParser.h"

#include <algorithm>

Suggestion SuggestionEngine::suggest(const juce::AudioBuffer<float>& buffer, const std::string& prompt, double sampleRate) const
{
    IntentParser parser;
    MuddyDetector detector;

    const auto intent = parser.parse(prompt);
    const auto muddyScore = detector.score(buffer, sampleRate);

    Suggestion suggestion;
    suggestion.confidence = std::clamp((intent.confidence + muddyScore) * 0.5f, 0.0f, 1.0f);

    if (intent.intent == Intent::ReduceMuddiness || muddyScore > 0.65f)
    {
        suggestion.reason = "Detected low-mid crowding and masking risk.";
        suggestion.impact = "Expected result: tighter low end and clearer presence.";
        suggestion.actions.push_back({ "Bell EQ: -2.5 dB @ 280 Hz (Q 1.2)" });
        suggestion.actions.push_back({ "Dynamic EQ: 250-450 Hz, -1.5 dB on peaks" });
        suggestion.actions.push_back({ "Transient emphasis: +5%" });
        return suggestion;
    }

    if (intent.intent == Intent::IncreaseClarity)
    {
        suggestion.reason = "Detected clarity intent with moderate low-mid pressure.";
        suggestion.impact = "Expected result: improved articulation and definition.";
        suggestion.actions.push_back({ "Presence shelf: +1.5 dB @ 4.5 kHz" });
        suggestion.actions.push_back({ "Low-mid cleanup: -1.0 dB @ 320 Hz" });
        return suggestion;
    }

    suggestion.reason = "No strong muddy signature detected from prompt/audio.";
    suggestion.impact = "Expected result: keep current tone with minimal intervention.";
    suggestion.actions.push_back({ "Monitor-only mode: no automatic processing" });
    return suggestion;
}
