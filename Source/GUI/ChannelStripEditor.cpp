#include "ChannelStripEditor.h"
#include "PluginProcessor.h"
#include "GUI/Theme.h"

ChannelStripEditor::ChannelStripEditor (VocalSuiteUltraProAudioProcessor& p)
    : processor (p)
{
    addAndMakeVisible (topBar);
    addAndMakeVisible (bottomMeters);

    footer.setText ("READY  |  GUI PRO MODE  |  PEAK HOLD METERS  |  DSP BYPASS SHELL  |  SPRINT 03 GUI PRO", juce::dontSendNotification);
    footer.setJustificationType (juce::Justification::centred);
    footer.setColour (juce::Label::textColourId, Theme::mutedText);
    footer.setFont (Theme::bold (14.0f));
    addAndMakeVisible (footer);

    auto& pre = createModule ("PREAMP", "preampOn");
    pre.addKnob ("DRIVE", "preampDrive", Theme::gold);
    pre.addKnob ("HARMONIC", "preampHarmonic", Theme::gold);
    pre.addKnob ("OUTPUT", "preampOutput", Theme::textBright);

    auto& eq = createModule ("EQ", "eqOn");
    eq.addKnob ("LOW", "eqLow", Theme::blue);
    eq.addKnob ("MID", "eqMid", Theme::green);
    eq.addKnob ("HIGH", "eqHigh", Theme::gold);
    eq.addKnob ("AIR", "eqAir", Theme::purple);

    auto& gate = createModule ("GATE", "gateOn");
    gate.addKnob ("THRESHOLD", "gateThreshold", Theme::gold);
    gate.addKnob ("RANGE", "gateRange", Theme::textBright);
    gate.addKnob ("HYST", "gateHyst", Theme::green);
    gate.addKnob ("HOLD", "gateHold", Theme::blue);
    gate.addKnob ("RELEASE", "gateRelease", Theme::blue);

    auto& de = createModule ("DE-ESSER", "deesserOn");
    de.addKnob ("S FREQ", "deessFreq", Theme::purple);
    de.addKnob ("THRESHOLD", "deessThreshold", Theme::gold);
    de.addKnob ("RANGE", "deessRange", Theme::textBright);
    de.addKnob ("LOOKAHEAD", "deessLookahead", Theme::blue);
    de.addKnob ("RELEASE", "deessRelease", Theme::green);

    auto& comp = createModule ("COMPRESSOR", "compressorOn");
    comp.addKnob ("INPUT", "compInput", Theme::textBright);
    comp.addKnob ("OUTPUT", "compOutput", Theme::textBright);
    comp.addKnob ("ATTACK", "compAttack", Theme::blue);
    comp.addKnob ("RELEASE", "compRelease", Theme::green);
    comp.addKnob ("PEAK REDUCTION", "compPeakReduction", Theme::gold);
    comp.addKnob ("GAIN", "compGain", Theme::textBright);

    auto& sat = createModule ("SATURATION", "saturationOn");
    sat.addKnob ("DRIVE", "satDrive", Theme::gold);
    sat.addKnob ("TONE", "satTone", Theme::red);
    sat.addKnob ("MIX", "satMix", Theme::blue);

    auto& hi = createModule ("HI-END", "hiendOn");
    hi.addKnob ("WARM", "hiWarm", Theme::red);
    hi.addKnob ("WIDTH", "hiWidth", Theme::textBright);
    hi.addKnob ("TAME", "hiTame", Theme::blue);
    hi.addKnob ("GLUE", "hiGlue", Theme::purple);

    auto& out = createModule ("OUTPUT", "outputOn");
    out.addKnob ("CEILING", "outCeiling", Theme::gold);
    out.addKnob ("THRESHOLD", "outThreshold", Theme::textBright);

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

    juce::ColourGradient bg (Theme::bgTop, 0, 0, Theme::bgBottom, 0, (float) getHeight(), false);
    bg.addColour (0.42, Theme::bgMid);
    g.setGradientFill (bg);
    g.fillRoundedRectangle (r.reduced (5.0f), 10.0f);

    g.setColour (juce::Colour (0xff343434));
    g.drawRoundedRectangle (r.reduced (5.0f), 10.0f, 1.4f);

    g.setColour (juce::Colour (0x18000000));
    for (int i = 0; i < getWidth(); i += 24)
        g.drawVerticalLine (i, 104.0f, (float) getHeight() - 152.0f);

    g.setColour (Theme::gold.withAlpha (0.09f));
    g.drawLine (16.0f, 103.0f, (float) getWidth() - 16.0f, 103.0f, 1.0f);
    g.drawLine (16.0f, (float) getHeight() - 150.0f, (float) getWidth() - 16.0f, (float) getHeight() - 150.0f, 1.0f);
}

void ChannelStripEditor::resized()
{
    auto r = getLocalBounds().reduced (10);

    topBar.setBounds (r.removeFromTop (96));

    auto meterArea = r.removeFromBottom (145);
    footer.setBounds (meterArea.removeFromBottom (32));
    bottomMeters.setBounds (meterArea.reduced (0, 4));

    auto moduleArea = r.reduced (0, 7);
    const int gap = 6;
    int widths[] = { 184, 169, 171, 176, 248, 169, 160, 169 };

    for (int i = 0; i < modules.size(); ++i)
    {
        const int w = widths[i];
        modules[i]->setBounds (moduleArea.removeFromLeft (w));
        moduleArea.removeFromLeft (gap);
    }
}

void ChannelStripEditor::timerCallback()
{
    bottomMeters.setLevels (processor.getInputPeak() * 2.2f,
                            processor.getOutputPeak() * 2.2f,
                            processor.getGainReduction());
}
