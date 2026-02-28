#include <catch2/catch_test_macros.hpp>

#include <vector>

#include "engine/SessionState.h"

TEST_CASE("SessionState supports rollback to original chain")
{
    SessionState state;
    state.setOriginalChain({ "EQ", "Comp" });
    state.applyChain({ "EQ", "DynamicEQ", "Comp" });
    REQUIRE(state.canRollback());
    state.rollback();
    REQUIRE(state.currentChain() == state.originalChain());
}
