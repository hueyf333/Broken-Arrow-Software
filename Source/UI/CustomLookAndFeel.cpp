#include "CustomLookAndFeel.h"

namespace KnobSmith
{

CustomLookAndFeel::CustomLookAndFeel()
{
    // Set dark colour scheme
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0xff1e1e1e));
    setColour(juce::TextButton::buttonColourId, juce::Colour(0xff3e3e42));
    setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff007acc));
    setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff3e3e42));
    setColour(juce::ComboBox::textColourId, juce::Colours::white);
    setColour(juce::Label::textColourId, juce::Colours::white);
    setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff2d2d2d));
    setColour(juce::TextEditor::textColourId, juce::Colours::white);
    setColour(juce::ListBox::backgroundColourId, juce::Colour(0xff2d2d2d));
    setColour(juce::ListBox::textColourId, juce::Colours::white);
    setColour(juce::TreeView::backgroundColourId, juce::Colour(0xff2d2d2d));
}

} // namespace KnobSmith
