#pragma once
#include <JuceHeader.h>

namespace ReleaseInfo
{
    inline constexpr const char* productName = "Vocal Suite Ultra Pro";
    inline constexpr const char* companyName = "Hai Pham Audio";
    inline constexpr const char* version = "5.5.0";
    inline constexpr const char* milestone = "Epic 5F Commercial Release v1.0";
    inline constexpr const char* tag = "v1.0-commercial-release";

    inline juce::String getFullVersionString()
    {
        return juce::String(productName) + " " + version + " - " + milestone;
    }
}
