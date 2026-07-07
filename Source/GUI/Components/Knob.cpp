#include "Knob.h"
#include "GUI/Theme.h"

KnobLookAndFeel::KnobLookAndFeel()
{
    setColour (juce::Slider::textBoxTextColourId, Theme::textBright);
    setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xff090909));
    setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0xff2d2d2d));
}

void KnobLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int w, int h,
                                        float pos, float start, float end, juce::Slider& s)
{
    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) w, (float) h).reduced (5.0f);
    auto dial = bounds.withSizeKeepingCentre (juce::jmin (bounds.getWidth(), bounds.getHeight()),
                                              juce::jmin (bounds.getWidth(), bounds.getHeight()));
    auto centre = dial.getCentre();
    auto radius = dial.getWidth() * 0.5f;
    auto angle = start + pos * (end - start);

    g.setColour (juce::Colour (0xaa000000));
    g.fillEllipse (dial.translated (0.0f, 4.0f));

    auto outer = dial.reduced (1.0f);
    juce::ColourGradient outerGrad (juce::Colour (0xff565656), centre.x, outer.getY(),
                                    juce::Colour (0xff111111), centre.x, outer.getBottom(), false);
    g.setGradientFill (outerGrad);
    g.fillEllipse (outer);

    g.setColour (juce::Colour (0xff050505));
    g.fillEllipse (outer.reduced (6.0f));

    juce::ColourGradient innerGrad (juce::Colour (0xff3c3d3d), centre.x - radius, centre.y - radius,
                                    juce::Colour (0xff121212), centre.x + radius, centre.y + radius, false);
    g.setGradientFill (innerGrad);
    g.fillEllipse (outer.reduced (8.0f));

    for (int i = 0; i <= 10; ++i)
    {
        const float a = start + (end - start) * (float) i / 10.0f;
        const auto p1 = centre + juce::Point<float> (std::sin (a), -std::cos (a)) * (radius - 2.0f);
        const auto p2 = centre + juce::Point<float> (std::sin (a), -std::cos (a)) * (radius - (i % 5 == 0 ? 10.0f : 7.0f));
        g.setColour (i % 5 == 0 ? Theme::mutedText.withAlpha (0.72f) : Theme::mutedText.withAlpha (0.35f));
        g.drawLine (p1.x, p1.y, p2.x, p2.y, i % 5 == 0 ? 1.4f : 1.0f);
    }

    juce::Path backArc;
    backArc.addCentredArc (centre.x, centre.y, radius - 5.0f, radius - 5.0f,
                           0.0f, start, end, true);
    g.setColour (juce::Colour (0xff151515));
    g.strokePath (backArc, juce::PathStrokeType (4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Path valueArc;
    valueArc.addCentredArc (centre.x, centre.y, radius - 5.0f, radius - 5.0f,
                            0.0f, start, angle, true);
    g.setColour (s.findColour (juce::Slider::rotarySliderFillColourId));
    g.strokePath (valueArc, juce::PathStrokeType (4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Path pointer;
    pointer.addRoundedRectangle (-1.5f, -radius + 13.0f, 3.0f, radius * 0.42f, 1.5f);
    pointer.applyTransform (juce::AffineTransform::rotation (angle).translated (centre.x, centre.y));
    g.setColour (Theme::textBright);
    g.fillPath (pointer);

    auto cap = outer.reduced (25.0f);
    juce::ColourGradient capGrad (juce::Colour (0xffe9e1c8), centre.x, cap.getY(),
                                  juce::Colour (0xff6f6f6f), centre.x, cap.getBottom(), false);
    g.setGradientFill (capGrad);
    g.fillEllipse (cap);
    g.setColour (juce::Colour (0x66000000));
    g.drawEllipse (cap, 1.0f);

    g.setColour (juce::Colour (0x22ffffff));
    g.fillEllipse (outer.reduced (14.0f).withTrimmedBottom (outer.getHeight() * 0.48f));
}

Knob::Knob (juce::AudioProcessorValueTreeState& state,
            const juce::String& paramId,
            const juce::String& labelText,
            juce::Colour fill)
{
    label.setText (labelText, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setColour (juce::Label::textColourId, Theme::text);
    label.setFont (Theme::bold (12.5f));

    slider.setLookAndFeel (&laf);
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 21);
    slider.setRotaryParameters (juce::MathConstants<float>::pi * 1.24f,
                                juce::MathConstants<float>::pi * 2.76f, true);
    slider.setColour (juce::Slider::rotarySliderFillColourId, fill);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (state, paramId, slider);

    addAndMakeVisible (label);
    addAndMakeVisible (slider);
}

Knob::~Knob()
{
    slider.setLookAndFeel (nullptr);
}

void Knob::resized()
{
    auto r = getLocalBounds();
    label.setBounds (r.removeFromTop (20));
    slider.setBounds (r.reduced (0, 0));
}
