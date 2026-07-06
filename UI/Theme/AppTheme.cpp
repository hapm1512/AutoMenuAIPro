#include "AppTheme.h"

namespace AppTheme
{
    juce::Colour background() { return juce::Colour::fromRGB (5, 8, 13); }
    juce::Colour panel()      { return juce::Colour::fromRGB (12, 17, 24); }
    juce::Colour panelDark()  { return juce::Colour::fromRGB (8, 12, 18); }
    juce::Colour border()     { return juce::Colour::fromRGB (32, 42, 55); }
    juce::Colour text()       { return juce::Colour::fromRGB (235, 240, 248); }
    juce::Colour subText()    { return juce::Colour::fromRGB (135, 145, 160); }
    juce::Colour purple()     { return juce::Colour::fromRGB (135, 72, 255); }
    juce::Colour blue()       { return juce::Colour::fromRGB (55, 155, 255); }
    juce::Colour green()      { return juce::Colour::fromRGB (35, 235, 95); }
    juce::Colour red()        { return juce::Colour::fromRGB (255, 72, 72); }
}
