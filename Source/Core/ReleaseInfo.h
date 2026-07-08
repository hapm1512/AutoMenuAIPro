#pragma once
#include <JuceHeader.h>

namespace ReleaseInfo
{
    inline constexpr const char* productName = "Vocal Suite Ultra Pro";
    inline constexpr const char* companyName = "Hai Audio";
    inline constexpr const char* version = "4.5.0-rc";
    inline constexpr const char* milestone = "Epic 4F Release Candidate";
    inline constexpr const char* tag = "v4.5-epic4f-release-candidate";

    inline juce::String getFullVersionString()
    {
        return juce::String(productName) + " " + version + " - " + milestone;
    }
}
