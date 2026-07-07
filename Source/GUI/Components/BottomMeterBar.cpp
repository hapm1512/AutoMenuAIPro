#include "BottomMeterBar.h"
#include "GUI/Theme.h"

BottomMeterBar::BottomMeterBar()
{
    auto setup = [&] (juce::Label& l, const juce::String& text)
    {
        l.setText (text, juce::dontSendNotification);
        l.setJustificationType (juce::Justification::centred);
        l.setColour (juce::Label::textColourId, Theme::text);
        l.setFont (Theme::bold (15.0f));
        addAndMakeVisible (l);
    };

    setup (inputLabel, "INPUT");
    setup (outputLabel, "OUTPUT");
    setup (grLabel, "GAIN REDUCTION");
    setup (lufsLabel, "LUFS\n-14.0");
    setup (peakLabel, "PEAK\n-1.2 dBTP");
    setup (clipInLabel, "CLIP");
    setup (clipOutLabel, "CLIP");

    clipInLabel.setColour (juce::Label::textColourId, Theme::red);
    clipOutLabel.setColour (juce::Label::textColourId, Theme::red);

    addAndMakeVisible (inputMeter);
    addAndMakeVisible (outputMeter);
    addAndMakeVisible (grMeter);
}

void BottomMeterBar::setLevels (float input, float output, float gr)
{
    inputLevel = juce::jlimit (0.0f, 1.0f, input);
    outputLevel = juce::jlimit (0.0f, 1.0f, output);
    grLevel = juce::jlimit (0.0f, 1.0f, gr);

    inputMeter.setLevel (inputLevel);
    outputMeter.setLevel (outputLevel);
    grMeter.setLevel (grLevel);

    clipInLabel.setAlpha (inputLevel > 0.96f ? 1.0f : 0.22f);
    clipOutLabel.setAlpha (outputLevel > 0.96f ? 1.0f : 0.22f);
}

void BottomMeterBar::paint (juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat().reduced (2.0f);
    Theme::drawInsetPanel (g, r, 8.0f);

    g.setColour (Theme::gold.withAlpha (0.18f));
    g.drawLine (18.0f, 8.0f, getWidth() - 18.0f, 8.0f, 1.0f);

    Theme::drawSmallLed (g, juce::Rectangle<float> (24.0f, 22.0f, 9.0f, 9.0f), inputLevel > 0.02f, Theme::green);
    Theme::drawSmallLed (g, juce::Rectangle<float> ((float) getWidth() - 416.0f, 22.0f, 9.0f, 9.0f), outputLevel > 0.02f, Theme::green);
}

void BottomMeterBar::resized()
{
    auto r = getLocalBounds().reduced (12, 10);

    auto left = r.removeFromLeft (430);
    auto mid = r.removeFromLeft (300);
    auto right = r.removeFromLeft (450);
    auto small = r;

    auto inputHeader = left.removeFromTop (28);
    inputLabel.setBounds (inputHeader);
    clipInLabel.setBounds (inputHeader.removeFromRight (58));
    inputMeter.setBounds (left.reduced (45, 22));

    grLabel.setBounds (mid.removeFromTop (28));
    grMeter.setBounds (mid.reduced (48, 26));

    auto outputHeader = right.removeFromTop (28);
    outputLabel.setBounds (outputHeader);
    clipOutLabel.setBounds (outputHeader.removeFromRight (58));
    outputMeter.setBounds (right.reduced (50, 22));

    lufsLabel.setBounds (small.removeFromLeft (130).reduced (8));
    peakLabel.setBounds (small.reduced (8));
}
