#include "BottomMeterBar.h"
#include "GUI/Theme.h"

BottomMeterBar::BottomMeterBar()
{
    auto setup = [&] (juce::Label& l, const juce::String& text)
    {
        l.setText (text, juce::dontSendNotification);
        l.setJustificationType (juce::Justification::centred);
        l.setColour (juce::Label::textColourId, Theme::text);
        l.setFont (Theme::bold (14.0f));
        addAndMakeVisible (l);
    };

    setup (inputLabel, "INPUT");
    setup (outputLabel, "OUTPUT");
    setup (grLabel, "REDUCTION");
    setup (lufsLabel, "LUFS\n-14.2");
    setup (lufsShortLabel, "SHORT\n-13.8");
    setup (momentaryLabel, "MOM\n-1.2");
    setup (peakLabel, "TRUE PEAK\n-1.0");
    setup (corrLabel, "CORRELATION\n0.62");
    setup (clipInLabel, "CLIP");
    setup (clipOutLabel, "CLIP");

    clipInLabel.setColour (juce::Label::textColourId, Theme::red);
    clipOutLabel.setColour (juce::Label::textColourId, Theme::red);
    lufsLabel.setColour (juce::Label::textColourId, Theme::blue.brighter (0.35f));
    momentaryLabel.setColour (juce::Label::textColourId, juce::Colour (0xff4fe3e9));
    peakLabel.setColour (juce::Label::textColourId, Theme::red.brighter (0.35f));

    for (auto* b : { &monoButton, &stereoButton, &midButton, &sideButton })
    {
        b->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff121314));
        b->setColour (juce::TextButton::buttonOnColourId, Theme::blue.darker (0.25f));
        b->setColour (juce::TextButton::textColourOffId, Theme::text);
        b->setColour (juce::TextButton::textColourOnId, Theme::textBright);
        b->setClickingTogglesState (true);
        addAndMakeVisible (*b);
    }

    stereoButton.setToggleState (true, juce::dontSendNotification);

    addAndMakeVisible (inputMeter);
    addAndMakeVisible (outputMeter);
    addAndMakeVisible (grMeter);
}

juce::String BottomMeterBar::dbText (float amp)
{
    const float db = juce::Decibels::gainToDecibels (juce::jmax (amp, 1.0e-9f), -90.0f);
    return juce::String (db, 1);
}

juce::String BottomMeterBar::lufsText (float value)
{
    if (value <= -69.0f)
        return "-inf";
    return juce::String (value, 1);
}

void BottomMeterBar::setLevels (float input, float output, float gr)
{
    setProfessionalMetering (input, output, input * 0.72f, output * 0.72f,
                             output, gr, -70.0f, -70.0f, -70.0f, 1.0f);
}

void BottomMeterBar::setProfessionalMetering (float inputPeak, float outputPeak,
                                              float inputRms, float outputRms,
                                              float truePeak,
                                              float gainReductionDb,
                                              float lufsIntegrated,
                                              float lufsShortTerm,
                                              float lufsMomentary,
                                              float correlation)
{
    inputLevel = juce::jlimit (0.0f, 1.0f, inputPeak);
    outputLevel = juce::jlimit (0.0f, 1.0f, outputPeak);
    inputRmsLevel = juce::jlimit (0.0f, 1.0f, inputRms);
    outputRmsLevel = juce::jlimit (0.0f, 1.0f, outputRms);
    truePeakLevel = juce::jlimit (0.0f, 1.0f, truePeak);
    grLevel = juce::jlimit (0.0f, 1.0f, gainReductionDb / 24.0f);
    correlationValue = juce::jlimit (-1.0f, 1.0f, correlation);
    lufsI = lufsIntegrated;
    lufsS = lufsShortTerm;
    lufsM = lufsMomentary;

    inputMeter.setLevels (inputLevel, inputRmsLevel);
    outputMeter.setLevels (outputLevel, outputRmsLevel);
    grMeter.setLevel (grLevel);

    lufsLabel.setText ("LUFS-I\n" + lufsText (lufsI), juce::dontSendNotification);
    lufsShortLabel.setText ("LUFS-S\n" + lufsText (lufsS), juce::dontSendNotification);
    momentaryLabel.setText ("MOM\n" + lufsText (lufsM), juce::dontSendNotification);
    peakLabel.setText ("TRUE PEAK\n" + dbText (truePeakLevel), juce::dontSendNotification);
    corrLabel.setText ("CORRELATION\n" + juce::String (correlationValue, 2), juce::dontSendNotification);

    clipInLabel.setAlpha (inputLevel > 0.96f ? 1.0f : 0.18f);
    clipOutLabel.setAlpha (outputLevel > 0.96f ? 1.0f : 0.18f);
}

void BottomMeterBar::paint (juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat().reduced (2.0f);
    Theme::drawInsetPanel (g, r, 9.0f);

    g.setColour (Theme::gold.withAlpha (0.16f));
    g.drawLine (18.0f, 8.0f, getWidth() - 18.0f, 8.0f, 1.0f);

    auto inputBlock = juce::Rectangle<float> (18.0f, 17.0f, 315.0f, getHeight() - 32.0f);
    auto grBlock = juce::Rectangle<float> (348.0f, 17.0f, 330.0f, getHeight() - 32.0f);
    auto lufsBlock = juce::Rectangle<float> (690.0f, 17.0f, 425.0f, getHeight() - 32.0f);
    auto corrBlock = juce::Rectangle<float> (1130.0f, 17.0f, 195.0f, getHeight() - 32.0f);
    auto modeBlock = juce::Rectangle<float> (1342.0f, 17.0f, 165.0f, getHeight() - 32.0f);

    for (auto block : { inputBlock, grBlock, lufsBlock, corrBlock, modeBlock })
    {
        juce::ColourGradient grad (juce::Colour (0xff111315), block.getX(), block.getY(),
                                   juce::Colour (0xff060707), block.getX(), block.getBottom(), false);
        g.setGradientFill (grad);
        g.fillRoundedRectangle (block, 6.0f);

        g.setColour (Theme::panelLine.withAlpha (0.46f));
        g.drawRoundedRectangle (block, 6.0f, 1.0f);
    }

    Theme::drawSmallLed (g, juce::Rectangle<float> (28.0f, 25.0f, 9.0f, 9.0f),
                         inputLevel > 0.02f, Theme::green);
    Theme::drawSmallLed (g, juce::Rectangle<float> (360.0f, 25.0f, 9.0f, 9.0f),
                         grLevel > 0.02f, Theme::gold);

    auto corr = juce::Rectangle<float> (1150.0f, 82.0f, 155.0f, 9.0f);
    g.setColour (juce::Colour (0xff071012));
    g.fillRoundedRectangle (corr, 2.0f);

    auto corrBg = corr;
    const float zeroX = corrBg.getCentreX();
    const float valueX = corrBg.getX() + juce::jmap (correlationValue, -1.0f, 1.0f, 0.0f, corrBg.getWidth());

    g.setColour (correlationValue < 0.0f ? Theme::red.withAlpha (0.70f) : Theme::blue.withAlpha (0.80f));
    if (valueX >= zeroX)
        g.fillRoundedRectangle (juce::Rectangle<float> (zeroX, corrBg.getY(), valueX - zeroX, corrBg.getHeight()), 2.0f);
    else
        g.fillRoundedRectangle (juce::Rectangle<float> (valueX, corrBg.getY(), zeroX - valueX, corrBg.getHeight()), 2.0f);

    g.setColour (Theme::textBright.withAlpha (0.72f));
    g.drawVerticalLine (1150 + 77, 78.0f, 96.0f);

    g.setColour (Theme::mutedText.withAlpha (0.50f));
    g.setFont (Theme::regular (8.0f));
    g.drawText ("L     C     R", juce::Rectangle<int> (1146, 94, 160, 16), juce::Justification::centred);
}

void BottomMeterBar::resized()
{
    auto r = getLocalBounds().reduced (12, 10);

    auto input = r.removeFromLeft (330);
    auto gr = r.removeFromLeft (342);
    auto lufs = r.removeFromLeft (440);
    auto corr = r.removeFromLeft (210);
    auto mode = r;

    auto inputHeader = input.removeFromTop (28);
    inputLabel.setBounds (inputHeader);
    clipInLabel.setBounds (inputHeader.removeFromRight (55));
    inputMeter.setBounds (input.reduced (14, 22));

    auto grHeader = gr.removeFromTop (28);
    grLabel.setBounds (grHeader);
    clipOutLabel.setBounds (grHeader.removeFromRight (55));
    grMeter.setBounds (gr.reduced (16, 22));

    auto lufsRow = lufs.reduced (8);
    lufsLabel.setBounds (lufsRow.removeFromLeft (100).reduced (4));
    lufsShortLabel.setBounds (lufsRow.removeFromLeft (100).reduced (4));
    momentaryLabel.setBounds (lufsRow.removeFromLeft (100).reduced (4));
    peakLabel.setBounds (lufsRow.removeFromLeft (100).reduced (4));

    corrLabel.setBounds (corr.removeFromTop (62).reduced (6));
    outputMeter.setBounds (corr.reduced (20, 18));

    auto top = mode.removeFromTop (48).reduced (8, 6);
    monoButton.setBounds (top.removeFromLeft (78).reduced (2));
    stereoButton.setBounds (top.removeFromLeft (78).reduced (2));

    auto bottom = mode.removeFromTop (48).reduced (8, 6);
    midButton.setBounds (bottom.removeFromLeft (78).reduced (2));
    sideButton.setBounds (bottom.removeFromLeft (78).reduced (2));
}
