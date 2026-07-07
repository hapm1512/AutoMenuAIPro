#pragma once
#include <JuceHeader.h>

namespace Theme
{
    const juce::Colour bgTop       { 0xff252526 };
    const juce::Colour bgMid       { 0xff121314 };
    const juce::Colour bgBottom    { 0xff060707 };

    const juce::Colour panelTop    { 0xff303132 };
    const juce::Colour panel       { 0xff111213 };
    const juce::Colour panelDark   { 0xff080909 };
    const juce::Colour panelLine   { 0xff484848 };
    const juce::Colour panelGlow   { 0xff6a5222 };

    const juce::Colour text        { 0xffdddddd };
    const juce::Colour textBright  { 0xfff0e7cf };
    const juce::Colour mutedText   { 0xff8d8d8d };

    const juce::Colour gold        { 0xffd6a62c };
    const juce::Colour goldDark    { 0xff8f6818 };
    const juce::Colour blue        { 0xff5da6c8 };
    const juce::Colour green       { 0xff66b86a };
    const juce::Colour purple      { 0xffa47de2 };
    const juce::Colour red         { 0xffbb2b22 };
    const juce::Colour amber       { 0xffe0b14a };

    juce::FontOptions bold (float size);
    juce::FontOptions regular (float size);

    void drawInsetPanel (juce::Graphics& g, juce::Rectangle<float> r, float radius = 6.0f);
    void drawSectionHeader (juce::Graphics& g, juce::Rectangle<int> r, const juce::String& title);
    void drawSmallLed (juce::Graphics& g, juce::Rectangle<float> r, bool active, juce::Colour colour);
}
