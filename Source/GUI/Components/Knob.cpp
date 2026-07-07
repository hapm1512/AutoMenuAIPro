#include "Knob.h"
#include "GUI/Theme.h"

KnobLookAndFeel::KnobLookAndFeel()
{
    setColour (juce::Slider::textBoxTextColourId, Theme::textBright);
    setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xff050606));
    setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0xff3c3c3c));
}

void KnobLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int w, int h,
                                        float pos, float start, float end, juce::Slider& s)
{
    auto area = juce::Rectangle<float> ((float) x, (float) y, (float) w, (float) h).reduced (2.0f);
    auto size = juce::jmin (area.getWidth(), area.getHeight());
    auto dial = area.withSizeKeepingCentre (size, size).reduced (2.0f);

    auto centre = dial.getCentre();
    auto radius = dial.getWidth() * 0.5f;
    auto angle = start + pos * (end - start);
    auto fill = s.findColour (juce::Slider::rotarySliderFillColourId);
    const bool hot = s.isMouseOverOrDragging();

    g.setColour (juce::Colour (0xd5000000));
    g.fillEllipse (dial.translated (0.0f, 5.8f));

    g.setColour (juce::Colour (0x64000000));
    g.fillEllipse (dial.expanded (4.0f).translated (0.0f, 2.0f));

    if (hot)
    {
        g.setColour (fill.withAlpha (0.15f));
        g.fillEllipse (dial.expanded (13.0f));
        g.setColour (fill.withAlpha (0.075f));
        g.fillEllipse (dial.expanded (23.0f));
    }

    for (int i = 0; i <= 96; ++i)
    {
        const float t = (float) i / 96.0f;
        const float a = start + (end - start) * t;
        const bool major = (i % 16) == 0;
        const bool mid = (i % 8) == 0;

        const float len = major ? 11.0f : mid ? 7.4f : 4.8f;
        auto dir = juce::Point<float> (std::sin (a), -std::cos (a));
        auto p1 = centre + dir * (radius + 1.0f);
        auto p2 = centre + dir * (radius - len);

        const bool active = t <= pos;
        g.setColour (active ? fill.withAlpha (major ? 1.0f : mid ? 0.70f : 0.38f)
                            : Theme::mutedText.withAlpha (major ? 0.42f : mid ? 0.24f : 0.12f));

        g.drawLine (p1.x, p1.y, p2.x, p2.y, major ? 1.50f : mid ? 1.05f : 0.70f);
    }

    auto outer = dial.reduced (8.0f);

    juce::ColourGradient outerGrad (juce::Colour (0xff929292), centre.x, outer.getY(),
                                    juce::Colour (0xff0e0e0e), centre.x, outer.getBottom(), false);
    outerGrad.addColour (0.16, juce::Colour (0xff5a5a5a));
    outerGrad.addColour (0.48, juce::Colour (0xff2d2f30));
    outerGrad.addColour (0.76, juce::Colour (0xff151515));
    g.setGradientFill (outerGrad);
    g.fillEllipse (outer);

    g.setColour (juce::Colour (0xff050505));
    g.fillEllipse (outer.reduced (5.0f));

    juce::ColourGradient bodyGrad (juce::Colour (0xff5a5c5d), centre.x - radius, centre.y - radius,
                                   juce::Colour (0xff090a0a), centre.x + radius, centre.y + radius, false);
    bodyGrad.addColour (0.25, juce::Colour (0xff3f4040));
    bodyGrad.addColour (0.50, juce::Colour (0xff202222));
    bodyGrad.addColour (0.80, juce::Colour (0xff080808));
    g.setGradientFill (bodyGrad);
    g.fillEllipse (outer.reduced (7.5f));

    for (int i = 0; i < 12; ++i)
    {
        const float yy = outer.getY() + 13.0f + (float) i * 3.1f;
        g.setColour (juce::Colour (0x0fffffff));
        g.drawLine (outer.getX() + 18.0f, yy, outer.getRight() - 18.0f, yy + 0.8f, 0.6f);
    }

    juce::Path backArc;
    backArc.addCentredArc (centre.x, centre.y, radius - 13.5f, radius - 13.5f,
                           0.0f, start, end, true);

    g.setColour (juce::Colour (0xff070707));
    g.strokePath (backArc, juce::PathStrokeType (6.1f,
                                                 juce::PathStrokeType::curved,
                                                 juce::PathStrokeType::rounded));

    juce::Path valueArc;
    valueArc.addCentredArc (centre.x, centre.y, radius - 13.5f, radius - 13.5f,
                            0.0f, start, angle, true);

    g.setColour (fill.withAlpha (hot ? 0.30f : 0.17f));
    g.strokePath (valueArc, juce::PathStrokeType (10.0f,
                                                  juce::PathStrokeType::curved,
                                                  juce::PathStrokeType::rounded));

    g.setColour (fill.withAlpha (hot ? 1.0f : 0.88f));
    g.strokePath (valueArc, juce::PathStrokeType (hot ? 6.4f : 5.4f,
                                                  juce::PathStrokeType::curved,
                                                  juce::PathStrokeType::rounded));

    juce::Path pointer;
    pointer.addRoundedRectangle (-1.8f, -radius + 18.0f, 3.6f, radius * 0.46f, 1.8f);
    pointer.applyTransform (juce::AffineTransform::rotation (angle).translated (centre.x, centre.y));

    g.setColour (juce::Colour (0xa9000000));
    g.fillPath (pointer, juce::AffineTransform::translation (1.5f, 1.7f));

    g.setColour (hot ? Theme::textBright : juce::Colour (0xffe4dbc2));
    g.fillPath (pointer);

    auto cap = outer.reduced (26.0f);

    juce::ColourGradient capGrad (juce::Colour (0xfffff7dd), centre.x, cap.getY(),
                                  juce::Colour (0xff424242), centre.x, cap.getBottom(), false);
    capGrad.addColour (0.22, juce::Colour (0xffd8d8d8));
    capGrad.addColour (0.48, juce::Colour (0xff999999));
    capGrad.addColour (0.78, juce::Colour (0xff5b5b5b));
    g.setGradientFill (capGrad);
    g.fillEllipse (cap);

    g.setColour (juce::Colour (0x82000000));
    g.drawEllipse (cap, 1.0f);

    g.setColour (juce::Colour (0x36ffffff));
    g.fillEllipse (outer.reduced (13.0f).withTrimmedBottom (outer.getHeight() * 0.52f));

    auto dot = juce::Rectangle<float> (centre.x - 2.4f, centre.y - radius + 21.0f, 4.8f, 4.8f);
    dot = dot.transformedBy (juce::AffineTransform::rotation (angle, centre.x, centre.y));
    g.setColour (fill.brighter (0.48f).withAlpha (hot ? 1.0f : 0.88f));
    g.fillEllipse (dot);

    g.setColour (juce::Colour (0x22ffffff));
    g.drawEllipse (outer.reduced (10.0f), 0.6f);

    if (hot)
    {
        g.setColour (fill.withAlpha (0.45f));
        g.drawEllipse (outer.reduced (3.0f), 1.5f);
    }
}

Knob::Knob (juce::AudioProcessorValueTreeState& state,
            const juce::String& paramId,
            const juce::String& labelText,
            juce::Colour fill)
    : titleText (labelText)
{
    label.setText (labelText, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setColour (juce::Label::textColourId, Theme::text);
    label.setFont (Theme::bold (12.5f));

    unitLabel.setJustificationType (juce::Justification::centred);
    unitLabel.setColour (juce::Label::textColourId, Theme::mutedText.withAlpha (0.78f));
    unitLabel.setFont (Theme::regular (9.4f));

    if (auto* param = state.getParameter (paramId))
        unitLabel.setText (param->getLabel(), juce::dontSendNotification);

    slider.setLookAndFeel (&laf);
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 82, 21);
    slider.setRotaryParameters (juce::MathConstants<float>::pi * 1.25f,
                                juce::MathConstants<float>::pi * 2.75f, true);
    slider.setColour (juce::Slider::rotarySliderFillColourId, fill);
    slider.setPopupDisplayEnabled (true, true, nullptr);
    slider.setTooltip (labelText);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (state, paramId, slider);

    addAndMakeVisible (label);
    addAndMakeVisible (slider);
    addAndMakeVisible (unitLabel);
}

Knob::~Knob()
{
    slider.setLookAndFeel (nullptr);
}

void Knob::resized()
{
    auto r = getLocalBounds();
    label.setBounds (r.removeFromTop (18));
    unitLabel.setBounds (r.removeFromBottom (12));
    slider.setBounds (r.reduced (0, 0));
}

void Knob::mouseEnter (const juce::MouseEvent&)
{
    label.setColour (juce::Label::textColourId, Theme::textBright);
}

void Knob::mouseExit (const juce::MouseEvent&)
{
    label.setColour (juce::Label::textColourId, Theme::text);
}
