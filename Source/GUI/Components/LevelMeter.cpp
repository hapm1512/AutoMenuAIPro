#include "LevelMeter.h"
#include "GUI/Theme.h"

void LevelMeter::setLevel (float value)
{
    level = juce::jlimit (0.0f, 1.0f, value);
    peakHold = juce::jmax (level, peakHold * 0.965f);
    repaint();
}

void LevelMeter::paint (juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat();

    g.setColour (juce::Colour (0xff050505));
    g.fillRoundedRectangle (r, 4.0f);

    g.setColour (juce::Colour (0xff242424));
    g.drawRoundedRectangle (r.reduced (0.5f), 4.0f, 1.0f);

    constexpr int bars = 52;
    const float gap = 2.0f;
    const float bw = (r.getWidth() - gap * (bars - 1)) / bars;
    const int active = juce::roundToInt (level * bars);
    const int peak = juce::roundToInt (peakHold * bars);

    for (int i = 0; i < bars; ++i)
    {
        auto br = juce::Rectangle<float> (r.getX() + i * (bw + gap),
                                          r.getY() + 6.0f,
                                          bw,
                                          r.getHeight() - 12.0f);

        if (i < active)
        {
            if (i > bars * 0.88f)      g.setColour (Theme::red);
            else if (i > bars * 0.72f) g.setColour (Theme::gold);
            else                       g.setColour (Theme::green);
        }
        else
        {
            g.setColour (juce::Colour (0xff121812));
        }

        g.fillRoundedRectangle (br, 1.5f);
    }

    if (peak > 0)
    {
        const float x = r.getX() + juce::jlimit (0, bars - 1, peak - 1) * (bw + gap);
        auto hold = juce::Rectangle<float> (x, r.getY() + 4.0f, bw + 1.0f, r.getHeight() - 8.0f);
        g.setColour (peakHold > 0.88f ? Theme::red.brighter (0.3f) : Theme::textBright);
        g.fillRoundedRectangle (hold, 1.0f);
    }

    g.setColour (juce::Colour (0x18ffffff));
    g.fillRoundedRectangle (r.withHeight (r.getHeight() * 0.35f).reduced (2.0f), 3.0f);

    g.setColour (Theme::mutedText.withAlpha (0.5f));
    g.setFont (Theme::regular (9.0f));
    g.drawText ("-60        -24        -12        -6        0", r.reduced (6, 0),
                juce::Justification::centredBottom);
}
