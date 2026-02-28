#include <catch2/catch_test_macros.hpp>

#include "nlp/IntentParser.h"

TEST_CASE("IntentParser maps Korean muddy phrase to ReduceMuddiness")
{
    IntentParser parser;
    const auto result = parser.parse("소리가 너무 먹먹해");
    REQUIRE(result.intent == Intent::ReduceMuddiness);
}

TEST_CASE("IntentParser maps English clarity phrase to IncreaseClarity")
{
    IntentParser parser;
    const auto result = parser.parse("make it more clear and punchy");
    REQUIRE(result.intent == Intent::IncreaseClarity);
}
