#pragma once

#include <string>
#include <string_view>
#include <vector>

struct StyleProfile
{
    std::string id;
    std::string label;
    std::string toneHint;
};

const std::vector<StyleProfile>& getDefaultStyleProfiles();
const StyleProfile* findStyleProfile(std::string_view id);
