#include "PluginEditor.h"

//==============================================================================
SuperBassmanAudioProcessorEditor::SuperBassmanAudioProcessorEditor (SuperBassmanAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel (&customLookAndFeel);
    
    // Input section
    setupSlider (inputGainSlider, inputGainLabel, "INPUT", "inputGain");
    inputGainSlider.setTextValueSuffix (" dB");
    
    setupToggleButton (inputPadButton, "PAD", "inputPad");
    setupToggleButton (hpfEnableButton, "HPF", "hpfEnable");
    
    setupSlider (hpfFreqSlider, juce::Label(), "", "hpfFreq");
    hpfFreqSlider.setTextValueSuffix (" Hz");
    
    // Channel selection
    channelLabel.setText ("CHANNEL", juce::dontSendNotification);
    channelLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (channelLabel);
    
    setupComboBox (channelSelector, "channel");
    channelSelector.addItem ("Normal", 1);
    channelSelector.addItem ("Bright", 2);
    channelSelector.addItem ("Overdrive", 3);
    
    // Preamp
    setupSlider (preampGainSlider, preampGainLabel, "GAIN", "preampGain");
    setupSlider (preampBiasSlider, preampBiasLabel, "BIAS", "preampBias");
    
    // Tone stack
    setupSlider (bassSlider, bassLabel, "BASS", "bass");
    setupSlider (midSlider, midLabel, "MID", "mid");
    setupSlider (trebleSlider, trebleLabel, "TREBLE", "treble");
    setupSlider (presenceSlider, presenceLabel, "PRESENCE", "presence");
    
    // Overdrive
    setupSlider (driveSlider, driveLabel, "DRIVE", "drive");
    setupSlider (driveBlendSlider, driveBlendLabel, "BLEND", "driveBlend");
    driveBlendSlider.setTextValueSuffix (" %");
    setupSlider (driveToneSlider, driveToneLabel, "TONE", "driveTone");
    
    // Power amp
    setupSlider (masterVolumeSlider, masterVolumeLabel, "MASTER", "masterVolume");
    setupSlider (powerSagSlider, powerSagLabel, "SAG", "powerSag");
    
    // Cabinet
    setupToggleButton (cabEnableButton, "CAB ON", "cabEnable");
    setupComboBox (cabTypeSelector, "cabType");
    cabTypeSelector.addItem ("8x10 Sealed", 1);
    cabTypeSelector.addItem ("4x10 Ported", 2);
    cabTypeSelector.addItem ("1x15 Sealed", 3);
    setupSlider (cabMixSlider, cabMixLabel, "CAB MIX", "cabMix");
    cabMixSlider.setTextValueSuffix (" %");
    
    // Output
    setupSlider (outputGainSlider, outputGainLabel, "OUTPUT", "outputGain");
    outputGainSlider.setTextValueSuffix (" dB");
    setupToggleButton (limiterEnableButton, "LIMITER", "limiterEnable");
    setupToggleButton (bypassButton, "BYPASS", "bypass");
    
    // Meters
    addAndMakeVisible (inputMeter);
    addAndMakeVisible (outputMeter);
    
    // Start timer for meter updates
    startTimerHz (30);
    
    setSize (900, 600);
    setResizable (true, true);
    setResizeLimits (600, 400, 1800, 1200);
}

SuperBassmanAudioProcessorEditor::~SuperBassmanAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

//==============================================================================
void SuperBassmanAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background
    g.fillAll (juce::Colour (0xff2a2a2a));
    
    // Title area
    g.setColour (juce::Colour (0xff1a1a1a));
    g.fillRect (0, 0, getWidth(), 60);
    
    g.setColour (juce::Colours::orange);
    g.setFont (juce::Font (32.0f, juce::Font::bold));
    g.drawText ("SUPER BASSMAN", 20, 10, getWidth() - 40, 40, juce::Justification::centred);
    
    // Section dividers
    g.setColour (juce::Colour (0xff3a3a3a));
    int y = 80;
    g.drawLine (20.0f, static_cast<float> (y), static_cast<float> (getWidth() - 20), static_cast<float> (y), 2.0f);
    
    // Section labels
    g.setColour (juce::Colours::lightgrey);
    g.setFont (juce::Font (14.0f, juce::Font::bold));
    
    g.drawText ("INPUT", 20, 90, 100, 20, juce::Justification::centredLeft);
    g.drawText ("PREAMP", 20, 200, 100, 20, juce::Justification::centredLeft);
    g.drawText ("TONE", 20, 310, 100, 20, juce::Justification::centredLeft);
    g.drawText ("DRIVE", 500, 310, 100, 20, juce::Justification::centredLeft);
    g.drawText ("POWER", 500, 90, 100, 20, juce::Justification::centredLeft);
    g.drawText ("CAB & OUTPUT", 500, 200, 150, 20, juce::Justification::centredLeft);
}

void SuperBassmanAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop (60); // Title area
    bounds.reduce (20, 20);
    
    int knobSize = 80;
    int spacing = 10;
    
    // Input section (top left)
    int x = 20;
    int y = 120;
    
    inputGainSlider.setBounds (x, y, knobSize, knobSize);
    inputGainLabel.setBounds (x, y + knobSize, knobSize, 20);
    x += knobSize + spacing;
    
    inputPadButton.setBounds (x, y + 20, 60, 30);
    hpfEnableButton.setBounds (x, y + 55, 60, 30);
    hpfFreqSlider.setBounds (x + 70, y + 20, 80, 65);
    
    // Channel selector
    x = 20;
    y = 230;
    channelLabel.setBounds (x, y, 120, 20);
    channelSelector.setBounds (x, y + 25, 150, 30);
    
    // Preamp
    y = 280;
    preampGainSlider.setBounds (x, y, knobSize, knobSize);
    preampGainLabel.setBounds (x, y + knobSize, knobSize, 20);
    x += knobSize + spacing;
    
    preampBiasSlider.setBounds (x, y, knobSize, knobSize);
    preampBiasLabel.setBounds (x, y + knobSize, knobSize, 20);
    
    // Tone stack
    x = 20;
    y = 340;
    
    bassSlider.setBounds (x, y, knobSize, knobSize);
    bassLabel.setBounds (x, y + knobSize, knobSize, 20);
    x += knobSize + spacing;
    
    midSlider.setBounds (x, y, knobSize, knobSize);
    midLabel.setBounds (x, y + knobSize, knobSize, 20);
    x += knobSize + spacing;
    
    trebleSlider.setBounds (x, y, knobSize, knobSize);
    trebleLabel.setBounds (x, y + knobSize, knobSize, 20);
    x += knobSize + spacing;
    
    presenceSlider.setBounds (x, y, knobSize, knobSize);
    presenceLabel.setBounds (x, y + knobSize, knobSize, 20);
    
    // Overdrive section
    x = 500;
    y = 340;
    
    driveSlider.setBounds (x, y, knobSize, knobSize);
    driveLabel.setBounds (x, y + knobSize, knobSize, 20);
    x += knobSize + spacing;
    
    driveBlendSlider.setBounds (x, y, knobSize, knobSize);
    driveBlendLabel.setBounds (x, y + knobSize, knobSize, 20);
    x += knobSize + spacing;
    
    driveToneSlider.setBounds (x, y, knobSize, knobSize);
    driveToneLabel.setBounds (x, y + knobSize, knobSize, 20);
    
    // Power amp
    x = 500;
    y = 120;
    
    masterVolumeSlider.setBounds (x, y, knobSize, knobSize);
    masterVolumeLabel.setBounds (x, y + knobSize, knobSize, 20);
    x += knobSize + spacing;
    
    powerSagSlider.setBounds (x, y, knobSize, knobSize);
    powerSagLabel.setBounds (x, y + knobSize, knobSize, 20);
    
    // Cabinet and Output
    x = 500;
    y = 230;
    
    cabEnableButton.setBounds (x, y, 80, 30);
    cabTypeSelector.setBounds (x, y + 35, 150, 30);
    cabMixSlider.setBounds (x + 160, y, knobSize, knobSize);
    cabMixLabel.setBounds (x + 160, y + knobSize, knobSize, 20);
    
    x = 700;
    outputGainSlider.setBounds (x, y, knobSize, knobSize);
    outputGainLabel.setBounds (x, y + knobSize, knobSize, 20);
    
    limiterEnableButton.setBounds (x, y - 35, 80, 30);
    bypassButton.setBounds (x + 90, y - 35, 80, 30);
    
    // Meters
    inputMeter.setBounds (getWidth() - 50, 80, 30, getHeight() - 100);
    outputMeter.setBounds (getWidth() - 80, 80, 30, getHeight() - 100);
}

void SuperBassmanAudioProcessorEditor::timerCallback()
{
    // Update meters
    inputMeter.setLevel (audioProcessor.getInputLevel());
    inputMeter.setClipping (audioProcessor.getInputClipping());
    
    outputMeter.setLevel (audioProcessor.getOutputLevel());
    outputMeter.setClipping (audioProcessor.getOutputClipping());
}

//==============================================================================
void SuperBassmanAudioProcessorEditor::setupSlider (juce::Slider& slider, juce::Label& label,
                                                     const juce::String& labelText, const juce::String& paramId)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible (slider);
    
    if (labelText.isNotEmpty())
    {
        label.setText (labelText, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (label);
    }
    
    sliderAttachments.push_back (std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.getAPVTS(), paramId, slider));
}

void SuperBassmanAudioProcessorEditor::setupToggleButton (juce::ToggleButton& button,
                                                           const juce::String& buttonText,
                                                           const juce::String& paramId)
{
    button.setButtonText (buttonText);
    addAndMakeVisible (button);
    
    buttonAttachments.push_back (std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (
        audioProcessor.getAPVTS(), paramId, button));
}

void SuperBassmanAudioProcessorEditor::setupComboBox (juce::ComboBox& comboBox, const juce::String& paramId)
{
    addAndMakeVisible (comboBox);
    
    comboBoxAttachments.push_back (std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        audioProcessor.getAPVTS(), paramId, comboBox));
}
