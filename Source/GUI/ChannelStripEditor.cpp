#include "ChannelStripEditor.h"
#include "PluginProcessor.h"
#include "GUI/Theme.h"
#include "GUI/Layout/ChannelStripLayout.h"

ChannelStripEditor::ChannelStripEditor (VocalSuiteUltraProAudioProcessor& p)
    : processor (p)
{
    setLookAndFeel (&mainLookAndFeel);

    addAndMakeVisible (topBar);
    addAndMakeVisible (bottomMeters);
    addAndMakeVisible (popupValue);
    addAndMakeVisible (tooltipManager);

    footer.setText ("VOCAL SUITE ULTRA PRO  |  EPIC 3G OUTPUT STAGE  |  DSP ACTIVE", juce::dontSendNotification);
    footer.setJustificationType (juce::Justification::centred);
    footer.setColour (juce::Label::textColourId, Theme::mutedText);
    footer.setFont (Theme::bold (13.5f));
    addAndMakeVisible (footer);

    auto& pre = createModule ("ANALOG FRONT", "preampOn");
    pre.addKnob ("TRIM", "preampInputTrim", Theme::textBright);
    pre.addKnob ("DRIVE", "preampDrive", Theme::red);
    pre.addKnob ("IRON", "preampTransformer", Theme::gold);
    pre.addKnob ("TUBE", "preampTube", Theme::purple);
    pre.addKnob ("OUT", "preampOutput", Theme::blue);

    auto& gate = createModule ("PRO GATE", "gateOn");
    gate.addKnob ("THRESH", "gateThreshold", Theme::purple);
    gate.addKnob ("RANGE", "gateRange", Theme::purple);
    gate.addKnob ("LOOK", "gateLookAhead", Theme::blue);
    gate.addKnob ("KNEE", "gateKnee", Theme::gold);
    gate.addKnob ("REL", "gateRelease", Theme::purple);

    auto& eq = createModule ("COMM EQ", "eqOn");
    eq.addKnob ("HPF", "eqHpfFreq", Theme::textBright);
    eq.addKnob ("LOW", "eqLowGain", Theme::gold);
    eq.addKnob ("MID", "eqMidGain", Theme::green);
    eq.addKnob ("HIGH", "eqHighGain", Theme::blue);
    eq.addKnob ("AIR", "eqAirGain", Theme::purple);

    auto& de = createModule ("PRO DE-ESS", "deesserOn");
    de.addKnob ("FREQ", "deessFreq", Theme::blue);
    de.addKnob ("THRESH", "deessThreshold", Theme::gold);
    de.addKnob ("RANGE", "deessRange", Theme::textBright);
    de.addKnob ("FOCUS", "deessFocus", Theme::purple);
    de.addKnob ("REL", "deessRelease", Theme::green);

    auto& comp76 = createModule ("1176 FET", "compressorOn");
    comp76.addKnob ("INPUT", "comp1176Amount", Theme::blue);
    comp76.addKnob ("RATIO", "comp1176Ratio", Theme::purple);
    comp76.addKnob ("ATTACK", "compAttack", Theme::gold);
    comp76.addKnob ("RELEASE", "compRelease", Theme::gold);
    comp76.addKnob ("SC HPF", "compSideHp", Theme::textBright);

    auto& comp2a = createModule ("LA-2A OPTO", "compressorOn");
    comp2a.addKnob ("PEAK", "compLA2APeak", Theme::gold);
    comp2a.addKnob ("GAIN", "compLA2AGain", Theme::blue);
    comp2a.addKnob ("SPEED", "compLA2ASpeed", Theme::purple);
    comp2a.addKnob ("MIX", "compParallel", Theme::green);
    comp2a.addKnob ("ANALOG", "compAnalog", Theme::gold);

    auto& sat = createModule ("ANALOG COLOR", "saturationOn");
    sat.addKnob ("DRIVE", "satDrive", Theme::gold);
    sat.addKnob ("TAPE", "satTape", Theme::gold);
    sat.addKnob ("TUBE", "satTube", Theme::purple);
    sat.addKnob ("AIR", "satAir", Theme::blue);
    sat.addKnob ("MIX", "satMix", Theme::green);

    auto& hi = createModule ("HI-END", "hiendOn");
    hi.addKnob ("AIR", "hiWarm", Theme::blue);
    hi.addKnob ("PRESENCE", "hiTame", Theme::textBright);
    hi.addKnob ("BRIGHT", "hiGlue", Theme::blue);
    hi.addKnob ("WIDTH", "hiWidth", Theme::blue);

    auto& lim = createModule ("TRUE PEAK", "outputOn");
    lim.addKnob ("THRESH", "outThreshold", Theme::textBright);
    lim.addKnob ("CEIL", "outCeiling", Theme::gold);
    lim.addKnob ("SOFT", "outSoftClip", Theme::purple);
    lim.addKnob ("ISP", "outISP", Theme::blue);

    auto& out = createModule ("OUTPUT", "outputOn");
    out.addKnob ("TRIM", "outTrim", Theme::blue);
    out.addKnob ("LUFS", "outLufsComp", Theme::green);
    out.addKnob ("DITH", "outDither", Theme::textBright);
    out.addKnob ("REL", "outRelease", Theme::gold);
    out.addKnob ("MIX", "outMix", Theme::purple);

    startTimerHz (30);
}

ModulePanel& ChannelStripEditor::createModule (const juce::String& name, const juce::String& onParam)
{
    auto* m = new ModulePanel (processor.apvts, name, onParam);
    modules.add (m);
    addAndMakeVisible (m);
    return *m;
}

void ChannelStripEditor::paint (juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat();

    juce::ColourGradient bg (Theme::bgTop, 0, 0,
                             Theme::bgBottom, 0, (float) getHeight(), false);
    bg.addColour (0.28, Theme::bgMid);
    bg.addColour (0.70, juce::Colour (0xff07090a));
    g.setGradientFill (bg);
    g.fillRoundedRectangle (r.reduced (5.0f), 10.0f);

    g.setColour (juce::Colour (0xff343434));
    g.drawRoundedRectangle (r.reduced (5.0f), 10.0f, 1.4f);

    auto layout = ChannelStripLayout::calculate (getLocalBounds());

    g.setColour (juce::Colour (0x18000000));
    for (int i = 0; i < getWidth(); i += 22)
        g.drawVerticalLine (i, (float) layout.moduleArea.getY() - 8.0f, (float) layout.meterBar.getY() - 8.0f);

    g.setColour (juce::Colour (0x09000000));
    for (int y = layout.moduleArea.getY(); y < layout.meterBar.getY() - 8; y += 18)
        g.drawHorizontalLine (y, 16.0f, (float) getWidth() - 16.0f);

    auto tabs = layout.modeTabs.reduced (8, 12);
    auto tab1 = tabs.removeFromLeft (150);
    tabs.removeFromLeft (20);
    auto tab2 = tabs.removeFromLeft (130);

    g.setColour (juce::Colour (0xff241832));
    g.fillRoundedRectangle (tab1.toFloat(), 5.0f);
    g.setColour (Theme::purple.withAlpha (0.74f));
    g.drawRoundedRectangle (tab1.toFloat(), 5.0f, 1.0f);
    g.setColour (Theme::textBright);
    g.setFont (Theme::bold (15.0f));
    g.drawText ("CHANNEL STRIP", tab1, juce::Justification::centred);

    g.setColour (juce::Colour (0xff151718));
    g.fillRoundedRectangle (tab2.toFloat(), 5.0f);
    g.setColour (Theme::panelLine);
    g.drawRoundedRectangle (tab2.toFloat(), 5.0f, 1.0f);
    g.setColour (Theme::text);
    g.drawText ("ADVANCED", tab2, juce::Justification::centred);

    auto side = juce::Rectangle<int> (getWidth() - 470, layout.modeTabs.getY() + 19, 340, 38);
    g.setColour (juce::Colour (0xff0b0c0d));
    g.fillRoundedRectangle (side.toFloat(), 5.0f);
    g.setColour (Theme::panelLine);
    g.drawRoundedRectangle (side.toFloat(), 5.0f, 1.0f);

    g.setColour (Theme::text);
    g.setFont (Theme::regular (13.0f));
    g.drawText ("SIDE CHAIN     Int. Sidechain", side.reduced (12, 0), juce::Justification::centredLeft);

    Theme::drawSmallLed (g, juce::Rectangle<float> ((float) side.getRight() - 22.0f, (float) side.getY() + 14.0f, 10.0f, 10.0f),
                         true, Theme::green);

    g.setColour (Theme::gold.withAlpha (0.08f));
    g.drawLine (16.0f, (float) layout.moduleArea.getY() - 6.0f,
                (float) getWidth() - 16.0f, (float) layout.moduleArea.getY() - 6.0f, 1.0f);
    g.drawLine (16.0f, (float) getHeight() - 170.0f,
                (float) getWidth() - 16.0f, (float) getHeight() - 170.0f, 1.0f);
}

void ChannelStripEditor::resized()
{
    auto layout = ChannelStripLayout::calculate (getLocalBounds());

    topBar.setBounds (layout.topBar);
    footer.setBounds (layout.footer);
    bottomMeters.setBounds (layout.meterBar);

    auto moduleArea = layout.moduleArea;
    const int gap = 5;
    auto widths = ChannelStripLayout::moduleWidths();

    for (int i = 0; i < modules.size(); ++i)
    {
        modules[i]->setBounds (moduleArea.removeFromLeft (widths[(size_t) i]));
        moduleArea.removeFromLeft (gap);
    }

    popupValue.toFront (false);
    tooltipManager.toFront (false);
}

void ChannelStripEditor::timerCallback()
{
    bottomMeters.setLevels (processor.getInputPeak() * 2.2f,
                            processor.getOutputPeak() * 2.2f,
                            processor.getGainReduction());
}
