#include "profile/StyleProfile.h"

#include <algorithm>

const std::vector<StyleProfile>& getDefaultStyleProfiles()
{
    static const std::vector<StyleProfile> profiles = {
        { "default", "스타일 없음", "균형" },
        { "warm-pop", "따뜻한 팝", "따뜻한 저역 정리 + 보컬 존재감" },
        { "modern-pop", "모던 팝", "선명한 하이선/어택" },
        { "indie-acoustic", "어쿠스틱 인디", "부드러운 에어리 톤" },
    };

    return profiles;
}

const StyleProfile* findStyleProfile(std::string_view id)
{
    const auto& profiles = getDefaultStyleProfiles();
    const auto it = std::find_if(profiles.begin(), profiles.end(), [id](const auto& profile) {
        return profile.id == id;
    });

    if (it == profiles.end())
    {
        return nullptr;
    }

    return &(*it);
}
