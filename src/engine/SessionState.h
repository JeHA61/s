#pragma once

#include <cstdint>
#include <string>
#include <vector>

class SessionState
{
public:
    static constexpr std::int64_t kTargetMs = 120000;
    static constexpr std::int64_t kHardCapMs = 300000;

    void setOriginalChain(std::vector<std::string> chain);
    void applyChain(std::vector<std::string> chain);
    void rollback();

    bool canRollback() const;
    bool shouldUseFallback(std::int64_t elapsedMs) const;
    bool exceededHardCap(std::int64_t elapsedMs) const;

    const std::vector<std::string>& originalChain() const;
    const std::vector<std::string>& currentChain() const;

private:
    std::vector<std::string> originalProcessingChain;
    std::vector<std::string> activeProcessingChain;
    bool hasAppliedChanges { false };
};
