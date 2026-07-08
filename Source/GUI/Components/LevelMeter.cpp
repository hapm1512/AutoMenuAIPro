#include "LevelMeter.h"
#include "GUI/Theme.h"

void LevelMeter::setLevel (float value)
{
    setLevels (value, value * 0.72f);
}

void LevelMeter::setLevels (float peakValue, float rmsValue)
{
    level = juce::jlimit (0.0f, 1.0f, peakValue);
    rmsLevel = juce::jlimit (0.0f, 1.0f, rmsValue);
    peakHold = juce::jmax (level, peakHold * 0.974f);
    rmsGhost = rmsGhost * 0.90f + rmsLevel * 0.10f;

    if (level > 0.96f)
        clipHold = 1.0f;
    else
        clipHold *= 0.95f;

    repaint();
}

void LevelMeter::paint (juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat();

    g.setColour (juce::Colour (0xff030404));
    g.fillRoundedRectangle (r, 4.0f);

    g.setColour (juce::Colour (0xff292929));
    g.drawRoundedRectangle (r.reduced (0.5f), 4.0f, 1.0f);

    auto meter = r.reduced (7.0f, 10.0f);
    constexpr int bars = 64;
    const float gap = 2.0f;
    const float bw = (meter.getWidth() - gap * (bars - 1)) / bars;

    const int active = juce::roundToInt (level * bars);
    const int peak = juce::roundToInt (peakHold * bars);
    const int rms = juce::roundToInt (rmsGhost * bars);

    for (int i = 0; i < bars; ++i)
    {
        auto br = juce::Rectangle<float> (meter.getX() + i * (bw + gap),
                                          meter.getY(),
                                          bw,
                                          meter.getHeight() - 10.0f);

        if (i < active)
        {
            if (i > bars * 0.92f)      g.setColour (Theme::red.brighter (0.12f));
            else if (i > bars * 0.76f) g.setColour (Theme::gold);
            else if (i > bars * 0.58f) g.setColour (Theme::amber);
            else                       g.setColour (Theme::green);
        }
        else
        {
            g.setColour (juce::Colour (0xff0d140f));
        }

        g.fillRoundedRectangle (br, 1.3f);
    }

    if (rms > 0)
    {
        const int idx = juce::jlimit (0, bars - 1, rms - 1);
        const float x = meter.getX() + idx * (bw + gap);

        g.setColour (Theme::blue.withAlpha (0.78f));
        g.fillRoundedRectangle (juce::Rectangle<float> (x, meter.getY() + 2.0f,
                                                       bw + 1.2f, meter.getHeight() - 14.0f), 1.0f);
    }

    if (peak > 0)
    {
        const int idx = juce::jlimit (0, bars - 1, peak - 1);
        const float x = meter.getX() + idx * (bw + gap);

        g.setColour (peakHold > 0.92f ? Theme::red.brighter (0.55f) : Theme::textBright);
        g.fillRoundedRectangle (juce::Rectangle<float> (x, meter.getY() - 2.0f,
                                                       bw + 1.2f, meter.getHeight() - 6.0f), 1.0f);
    }

    if (clipHold > 0.02f)
    {
        auto clip = juce::Rectangle<float> (r.getRight() - 20.0f, r.getY() + 5.0f, 12.0f, 12.0f);
        Theme::drawSmallLed (g, clip, true, Theme::red.withAlpha (clipHold));
    }

    g.setColour (juce::Colour (0x18ffffff));
    g.fillRoundedRectangle (r.withHeight (r.getHeight() * 0.34f).reduced (3.0f), 3.0f);

    g.setColour (Theme::mutedText.withAlpha (0.54f));
    g.setFont (Theme::regular (8.6f));
    g.drawText ("-60       -48       -36       -24       -18       -12       -6    0",
                r.reduced (8, 0), juce::Justification::centredBottom);
}
