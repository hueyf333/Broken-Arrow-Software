#include "PluginProcessor.h"
#include "PluginEditor.h"

SVTCLAudioProcessorEditor::SVTCLAudioProcessorEditor(SVTCLAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
      inputMeter(p, true), outputMeter(p, false)
{
    setResizable(true, true);
    setResizeLimits(600, 400, 1200, 800);
    setSize(800, 600);
    
    // Configure Gain slider
    gainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(gainSlider);
    gainLabel.setText("GAIN", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainLabel);
    gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), "gain", gainSlider));
    
    // Configure Bass slider
    bassSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    bassSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(bassSlider);
    bassLabel.setText("BASS", juce::dontSendNotification);
    bassLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(bassLabel);
    bassAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), "bass", bassSlider));
    
    // Configure Mid slider
    midSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    midSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(midSlider);
    midLabel.setText("MID", juce::dontSendNotification);
    midLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(midLabel);
    midAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), "mid", midSlider));
    
    // Configure Treble slider
    trebleSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    trebleSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(trebleSlider);
    trebleLabel.setText("TREBLE", juce::dontSendNotification);
    trebleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(trebleLabel);
    trebleAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), "treble", trebleSlider));
    
    // Configure Ultra Lo slider
    ultraLoSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    ultraLoSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(ultraLoSlider);
    ultraLoLabel.setText("ULTRA LO", juce::dontSendNotification);
    ultraLoLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(ultraLoLabel);
    ultraLoAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), "ultraLo", ultraLoSlider));
    
    // Configure Ultra Hi slider
    ultraHiSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    ultraHiSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(ultraHiSlider);
    ultraHiLabel.setText("ULTRA HI", juce::dontSendNotification);
    ultraHiLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(ultraHiLabel);
    ultraHiAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), "ultraHi", ultraHiSlider));
    
    // Configure Master slider
    masterSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    masterSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(masterSlider);
    masterLabel.setText("MASTER", juce::dontSendNotification);
    masterLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(masterLabel);
    masterAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), "master", masterSlider));
    
    // Configure Mid Freq selector
    midFreqCombo.addItem("220 Hz", 1);
    midFreqCombo.addItem("450 Hz", 2);
    midFreqCombo.addItem("800 Hz", 3);
    midFreqCombo.addItem("1.6 kHz", 4);
    midFreqCombo.addItem("3 kHz", 5);
    addAndMakeVisible(midFreqCombo);
    midFreqLabel.setText("MID FREQ", juce::dontSendNotification);
    midFreqLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(midFreqLabel);
    midFreqAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(
        audioProcessor.getAPVTS(), "midFreq", midFreqCombo));
    
    // Configure buttons
    addAndMakeVisible(padButton);
    padAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(
        audioProcessor.getAPVTS(), "pad", padButton));
    
    addAndMakeVisible(cabSimButton);
    cabSimAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(
        audioProcessor.getAPVTS(), "cabSim", cabSimButton));
    
    addAndMakeVisible(standbyButton);
    standbyAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(
        audioProcessor.getAPVTS(), "standby", standbyButton));
    
    // Add meters
    addAndMakeVisible(inputMeter);
    addAndMakeVisible(outputMeter);
}

SVTCLAudioProcessorEditor::~SVTCLAudioProcessorEditor()
{
}

void SVTCLAudioProcessorEditor::paint(juce::Graphics& g)
{
    // SVT-CL inspired background
    g.fillAll(juce::Colour(0xff2a2a2a));
    
    // Title
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font("Arial", 32.0f, juce::Font::bold));
    g.drawText("SVT-CL", getLocalBounds().removeFromTop(60), juce::Justification::centred);
    
    // Draw panel sections
    g.setColour(juce::Colour(0xff3a3a3a));
    auto bounds = getLocalBounds().reduced(20);
    bounds.removeFromTop(60);
    g.fillRoundedRectangle(bounds.toFloat(), 10.0f);
    
    // Divider lines
    g.setColour(juce::Colour(0xff4a4a4a));
    auto divider1 = bounds.getX() + bounds.getWidth() / 3;
    auto divider2 = bounds.getX() + 2 * bounds.getWidth() / 3;
    g.drawLine(divider1, bounds.getY(), divider1, bounds.getBottom(), 2.0f);
    g.drawLine(divider2, bounds.getY(), divider2, bounds.getBottom(), 2.0f);
}

void SVTCLAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto titleArea = bounds.removeFromTop(60);
    bounds.reduce(20, 20);
    
    auto meterWidth = 30;
    auto meterArea = bounds.removeFromRight(meterWidth * 2 + 30);
    
    // Input meter on left, output on right
    auto inputMeterBounds = meterArea.removeFromLeft(meterWidth);
    meterArea.removeFromLeft(30);
    auto outputMeterBounds = meterArea;
    
    inputMeter.setBounds(inputMeterBounds);
    outputMeter.setBounds(outputMeterBounds);
    
    bounds.removeFromRight(20);
    
    // Layout controls in sections
    auto topSection = bounds.removeFromTop(bounds.getHeight() / 2);
    auto bottomSection = bounds;
    
    // Top row: Gain, Tone Stack
    auto gainArea = topSection.removeFromLeft(topSection.getWidth() / 7);
    gainLabel.setBounds(gainArea.removeFromTop(20));
    gainSlider.setBounds(gainArea.reduced(5));
    
    auto bassArea = topSection.removeFromLeft(topSection.getWidth() / 6);
    bassLabel.setBounds(bassArea.removeFromTop(20));
    bassSlider.setBounds(bassArea.reduced(5));
    
    auto midArea = topSection.removeFromLeft(topSection.getWidth() / 5);
    midLabel.setBounds(midArea.removeFromTop(20));
    midSlider.setBounds(midArea.reduced(5));
    
    auto trebleArea = topSection.removeFromLeft(topSection.getWidth() / 4);
    trebleLabel.setBounds(trebleArea.removeFromTop(20));
    trebleSlider.setBounds(trebleArea.reduced(5));
    
    auto ultraLoArea = topSection.removeFromLeft(topSection.getWidth() / 3);
    ultraLoLabel.setBounds(ultraLoArea.removeFromTop(20));
    ultraLoSlider.setBounds(ultraLoArea.reduced(5));
    
    auto ultraHiArea = topSection.removeFromLeft(topSection.getWidth() / 2);
    ultraHiLabel.setBounds(ultraHiArea.removeFromTop(20));
    ultraHiSlider.setBounds(ultraHiArea.reduced(5));
    
    auto masterArea = topSection;
    masterLabel.setBounds(masterArea.removeFromTop(20));
    masterSlider.setBounds(masterArea.reduced(5));
    
    // Bottom section: Mid freq selector and buttons
    auto midFreqArea = bottomSection.removeFromTop(60).reduced(10);
    midFreqLabel.setBounds(midFreqArea.removeFromTop(20));
    midFreqCombo.setBounds(midFreqArea);
    
    bottomSection.removeFromTop(20);
    auto buttonArea = bottomSection.removeFromTop(40).reduced(10);
    
    auto padArea = buttonArea.removeFromLeft(buttonArea.getWidth() / 3);
    padButton.setBounds(padArea.reduced(5));
    
    auto cabArea = buttonArea.removeFromLeft(buttonArea.getWidth() / 2);
    cabSimButton.setBounds(cabArea.reduced(5));
    
    standbyButton.setBounds(buttonArea.reduced(5));
}
