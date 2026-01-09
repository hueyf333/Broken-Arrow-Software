/*
  ==============================================================================

    Broken Arrow M900 - Marshall JCM900-Style Amp Plugin
    Copyright (c) 2026 Broken Arrow Software

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// AmpKnob Implementation
//==============================================================================
AmpKnob::AmpKnob(const juce::String& labelText)
    : label(labelText)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    slider.setColour(juce::Slider::thumbColourId, juce::Colours::darkgrey);
    addAndMakeVisible(slider);
}

void AmpKnob::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    g.setFont(12.0f);
    g.drawText(label, getLocalBounds().removeFromTop(15), juce::Justification::centred);
}

void AmpKnob::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(15); // Space for label
    slider.setBounds(bounds);
}

//==============================================================================
// BrokenArrowM900AudioProcessorEditor Implementation
//==============================================================================
BrokenArrowM900AudioProcessorEditor::BrokenArrowM900AudioProcessorEditor(BrokenArrowM900AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set up knobs
    addAndMakeVisible(inputTrimKnob);
    addAndMakeVisible(preampGainKnob);
    addAndMakeVisible(masterKnob);
    addAndMakeVisible(bassKnob);
    addAndMakeVisible(midKnob);
    addAndMakeVisible(trebleKnob);
    addAndMakeVisible(presenceKnob);
    addAndMakeVisible(outputKnob);

    // Set up toggles
    addAndMakeVisible(gateToggle);
    addAndMakeVisible(cabToggle);
    addAndMakeVisible(noiseToggle);

    // Set up combo boxes
    addAndMakeVisible(modeSelector);
    modeSelector.addItem("Clean", 1);
    modeSelector.addItem("Crunch", 2);
    modeSelector.addItem("High Gain", 3);
    modeLabel.setText("MODE", juce::dontSendNotification);
    modeLabel.attachToComponent(&modeSelector, true);
    addAndMakeVisible(modeLabel);

    addAndMakeVisible(oversamplingSelector);
    oversamplingSelector.addItem("1x", 1);
    oversamplingSelector.addItem("2x", 2);
    oversamplingSelector.addItem("4x", 3);
    oversamplingLabel.setText("OS", juce::dontSendNotification);
    oversamplingLabel.attachToComponent(&oversamplingSelector, true);
    addAndMakeVisible(oversamplingLabel);

    addAndMakeVisible(irSelector);
    irSelector.addItem("4x12 Vintage", 1);
    irSelector.addItem("4x12 Modern", 2);
    irSelector.addItem("2x12 Combo", 3);
    irLabel.setText("IR", juce::dontSendNotification);
    irLabel.attachToComponent(&irSelector, true);
    addAndMakeVisible(irLabel);

    // Create parameter attachments
    inputTrimAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "inputTrim", inputTrimKnob.getSlider());
    preampGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "preampGain", preampGainKnob.getSlider());
    masterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "master", masterKnob.getSlider());
    bassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "bass", bassKnob.getSlider());
    midAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "mid", midKnob.getSlider());
    trebleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "treble", trebleKnob.getSlider());
    presenceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "presence", presenceKnob.getSlider());
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "output", outputKnob.getSlider());

    gateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "gateEnabled", gateToggle);
    cabAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "cabEnabled", cabToggle);
    noiseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "noiseEnabled", noiseToggle);

    modeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.apvts, "mode", modeSelector);
    oversamplingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.apvts, "oversampling", oversamplingSelector);
    irAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.apvts, "irSelection", irSelector);

    // Start timer for meter updates
    startTimerHz(30);

    setSize(800, 500);
}

BrokenArrowM900AudioProcessorEditor::~BrokenArrowM900AudioProcessorEditor()
{
}

//==============================================================================
void BrokenArrowM900AudioProcessorEditor::paint(juce::Graphics& g)
{
    // Background - JCM900 style dark panel
    g.fillAll(juce::Colour(0xff1a1a1a));

    // Header
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font("Arial", 28.0f, juce::Font::bold));
    g.drawText("BROKEN ARROW M900", 20, 10, 400, 40, juce::Justification::centredLeft);

    g.setFont(juce::Font("Arial", 14.0f, juce::Font::italic));
    g.setColour(juce::Colours::orange);
    g.drawText("Marshall JCM900 Style", 20, 45, 400, 20, juce::Justification::centredLeft);

    // Draw sections
    g.setColour(juce::Colour(0xff2a2a2a));
    g.fillRect(10, 80, getWidth() - 20, 320);

    // Draw meters background
    g.setColour(juce::Colour(0xff3a3a3a));
    g.fillRect(getWidth() - 100, 80, 90, 320);

    // Input meter
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(getWidth() - 90, 100, 30, 200);
    
    float inputMeterHeight = juce::jmap(inputMeterLevel, -60.0f, 0.0f, 0.0f, 200.0f);
    g.setColour(juce::Colours::green);
    g.fillRect(getWidth() - 90, static_cast<int>(100 + 200 - inputMeterHeight), 30, static_cast<int>(inputMeterHeight));

    g.setColour(juce::Colours::white);
    g.setFont(10.0f);
    g.drawText("IN", getWidth() - 90, 305, 30, 15, juce::Justification::centred);

    // Output meter
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(getWidth() - 50, 100, 30, 200);
    
    float outputMeterHeight = juce::jmap(outputMeterLevel, -60.0f, 0.0f, 0.0f, 200.0f);
    g.setColour(clipIndicator ? juce::Colours::red : juce::Colours::green);
    g.fillRect(getWidth() - 50, static_cast<int>(100 + 200 - outputMeterHeight), 30, static_cast<int>(outputMeterHeight));

    g.setColour(juce::Colours::white);
    g.drawText("OUT", getWidth() - 50, 305, 30, 15, juce::Justification::centred);

    // Clip indicator
    if (clipIndicator)
    {
        g.setColour(juce::Colours::red);
        g.fillEllipse(getWidth() - 40.0f, 330.0f, 10.0f, 10.0f);
        g.setColour(juce::Colours::white);
        g.drawText("CLIP", getWidth() - 60, 345, 50, 15, juce::Justification::centred);
    }
}

void BrokenArrowM900AudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(80); // Header space
    bounds.removeFromRight(110); // Meter space
    bounds.removeFromBottom(80); // Bottom space

    auto knobBounds = bounds.reduced(10);
    int knobWidth = 90;
    int knobHeight = 110;

    // Top row - Input, Preamp, Tone Stack
    int x = 20;
    int y = 90;

    inputTrimKnob.setBounds(x, y, knobWidth, knobHeight);
    x += knobWidth + 10;

    preampGainKnob.setBounds(x, y, knobWidth, knobHeight);
    x += knobWidth + 20;

    bassKnob.setBounds(x, y, knobWidth, knobHeight);
    x += knobWidth + 10;

    midKnob.setBounds(x, y, knobWidth, knobHeight);
    x += knobWidth + 10;

    trebleKnob.setBounds(x, y, knobWidth, knobHeight);
    x += knobWidth + 10;

    presenceKnob.setBounds(x, y, knobWidth, knobHeight);

    // Bottom row - Master, Output
    x = 20;
    y = 210;

    masterKnob.setBounds(x, y, knobWidth, knobHeight);
    x += knobWidth + 10;

    outputKnob.setBounds(x, y, knobWidth, knobHeight);
    x += knobWidth + 30;

    // Toggles and selectors
    gateToggle.setBounds(x, y, 80, 30);
    cabToggle.setBounds(x + 100, y, 80, 30);
    noiseToggle.setBounds(x + 200, y, 80, 30);

    y += 40;
    modeSelector.setBounds(x + 50, y, 120, 25);
    
    y += 35;
    oversamplingSelector.setBounds(x + 50, y, 120, 25);
    
    y += 35;
    irSelector.setBounds(x + 50, y, 120, 25);
}

void BrokenArrowM900AudioProcessorEditor::timerCallback()
{
    // Update meter levels from processor
    inputMeterLevel = audioProcessor.inputLevel.load(std::memory_order_relaxed);
    outputMeterLevel = audioProcessor.outputLevel.load(std::memory_order_relaxed);
    clipIndicator = audioProcessor.isClipping.load(std::memory_order_relaxed);

    repaint();
}
