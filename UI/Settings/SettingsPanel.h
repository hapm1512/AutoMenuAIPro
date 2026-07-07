#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <functional>

class SettingsPanel final : public juce::Component
{
public:
    SettingsPanel();

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseUp (const juce::MouseEvent& event) override;

    std::function<void()> onClose;
    std::function<void()> onSave;

private:
    juce::Rectangle<int> closeButtonArea;
    juce::Rectangle<int> saveButtonArea;
    juce::Rectangle<int> resetButtonArea;
    juce::Rectangle<int> cancelButtonArea;

    void drawHeader (juce::Graphics& g, juce::Rectangle<int> area);
    void drawGroup (juce::Graphics& g,
                    juce::Rectangle<int> area,
                    const juce::String& title,
                    const std::vector<std::pair<juce::String, juce::String>>& rows,
                    juce::Colour accent);

    void drawRow (juce::Graphics& g,
                  juce::Rectangle<int> area,
                  const juce::String& label,
                  const juce::String& value,
                  bool hasButton = false);

    void drawCheckRow (juce::Graphics& g,
                       juce::Rectangle<int> area,
                       const juce::String& label,
                       bool checked);

    void drawBottomButton (juce::Graphics& g,
                           juce::Rectangle<int> area,
                           const juce::String& text,
                           juce::Colour colour);
};
