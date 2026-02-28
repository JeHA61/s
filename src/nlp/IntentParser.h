#pragma once

#include <string>

enum class Intent
{
    Unknown,
    ReduceMuddiness,
    IncreaseClarity
};

struct IntentResult
{
    Intent intent { Intent::Unknown };
    float confidence { 0.0f };
};

class IntentParser
{
public:
    IntentResult parse(const std::string& text) const;
};
