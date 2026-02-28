#include "nlp/IntentParser.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <string_view>

namespace
{
std::string toLowerAscii(std::string text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return text;
}

template <std::size_t N>
bool containsAny(std::string_view input, const std::array<std::string_view, N>& keywords)
{
    for (const auto keyword : keywords)
    {
        if (input.find(keyword) != std::string_view::npos)
        {
            return true;
        }
    }
    return false;
}
} // namespace

IntentResult IntentParser::parse(const std::string& text) const
{
    const auto lower = toLowerAscii(text);

    static constexpr std::array<std::string_view, 5> muddinessKeywords {
        "먹먹",
        "답답",
        "muddy",
        "boxy",
        "muffled",
    };

    static constexpr std::array<std::string_view, 6> clarityKeywords {
        "선명",
        "알맹이",
        "clear",
        "clarity",
        "punchy",
        "punch",
    };

    if (containsAny(lower, muddinessKeywords))
    {
        return { Intent::ReduceMuddiness, 0.9f };
    }

    if (containsAny(lower, clarityKeywords))
    {
        return { Intent::IncreaseClarity, 0.85f };
    }

    return { Intent::Unknown, 0.2f };
}
