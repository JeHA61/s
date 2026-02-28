#include <catch2/catch_test_macros.hpp>

#include "ui/RationalePanel.h"

TEST_CASE("Rationale formatter includes Korean why prefix")
{
    const auto formatted = formatReasonText("low-mid crowded");
    REQUIRE(formatted.find("왜") != std::string::npos);
}
