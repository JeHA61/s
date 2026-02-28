#include "engine/SessionState.h"

#include <utility>

void SessionState::setOriginalChain(std::vector<std::string> chain)
{
    originalProcessingChain = std::move(chain);
    activeProcessingChain = originalProcessingChain;
    hasAppliedChanges = false;
}

void SessionState::applyChain(std::vector<std::string> chain)
{
    activeProcessingChain = std::move(chain);
    hasAppliedChanges = true;
}

void SessionState::rollback()
{
    activeProcessingChain = originalProcessingChain;
    hasAppliedChanges = false;
}

bool SessionState::canRollback() const
{
    return hasAppliedChanges;
}

bool SessionState::shouldUseFallback(std::int64_t elapsedMs) const
{
    return elapsedMs > kTargetMs;
}

bool SessionState::exceededHardCap(std::int64_t elapsedMs) const
{
    return elapsedMs > kHardCapMs;
}

const std::vector<std::string>& SessionState::originalChain() const
{
    return originalProcessingChain;
}

const std::vector<std::string>& SessionState::currentChain() const
{
    return activeProcessingChain;
}
