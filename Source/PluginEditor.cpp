#include "PluginProcessor.h"
#include "PluginEditor.h"

AmpegSVTAudioProcessorEditor::AmpegSVTAudioProcessorEditor(AmpegSVTAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(800, 600);
    startTimerHz(30);
}

AmpegSVTAudioProcessorEditor::~AmpegSVTAudioProcessorEditor()
{
}

void AmpegSVTAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Ampeg-style background
    g.fillAll(juce::Colour(0xff1a1a1a));
    
    // Header
    g.setColour(juce::Colour(0xff2a4a6a));
    g.fillRect(0, 0, getWidth(), 80);
    
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(48.0f, juce::Font::bold));
    g.drawText("AMPEG SVT", 20, 15, getWidth() - 40, 50, juce::Justification::centred);
    
    g.setFont(juce::Font(16.0f));
    g.drawText("Heritage Bass Amplifier", 20, 55, getWidth() - 40, 20, juce::Justification::centred);
    
    // Section labels
    g.setColour(juce::Colours::lightgrey);
    g.setFont(juce::Font(14.0f, juce::Font::bold));
    g.drawText("INPUT", 30, 100, 100, 20, juce::Justification::left);
    g.drawText("PREAMP", 180, 100, 100, 20, juce::Justification::left);
    g.drawText("TONE", 330, 100, 200, 20, juce::Justification::left);
    g.drawText("POWER", 580, 100, 100, 20, juce::Justification::left);
    
    // Meters
    float inputLvl = audioProcessor.inputLevel.load();
    float outputLvl = audioProcessor.outputLevel.load();
    bool isClipping = audioProcessor.clipping.load();
    
    // Input meter
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(30, 500, 20, 80);
    
    float inputMeterHeight = juce::jlimit(0.0f, 80.0f, juce::Decibels::gainToDecibels(inputLvl, -60.0f) / 60.0f * 80.0f + 80.0f);
    g.setColour(inputMeterHeight > 70.0f ? juce::Colours::red : juce::Colours::green);
    g.fillRect(30, 500 + (80 - static_cast<int>(inputMeterHeight)), 20, static_cast<int>(inputMeterHeight));
    
    g.setColour(juce::Colours::white);
    g.drawText("IN", 28, 480, 24, 15, juce::Justification::centred);
    
    // Output meter
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(750, 500, 20, 80);
    
    float outputMeterHeight = juce::jlimit(0.0f, 80.0f, juce::Decibels::gainToDecibels(outputLvl, -60.0f) / 60.0f * 80.0f + 80.0f);
    g.setColour(outputMeterHeight > 70.0f ? juce::Colours::red : juce::Colours::green);
    g.fillRect(750, 500 + (80 - static_cast<int>(outputMeterHeight)), 20, static_cast<int>(outputMeterHeight));
    
    g.setColour(juce::Colours::white);
    g.drawText("OUT", 748, 480, 24, 15, juce::Justification::centred);
    
    // Clip indicator
    if (isClipping)
    {
        g.setColour(juce::Colours::red);
        g.fillEllipse(getWidth() / 2 - 10, 490, 20, 20);
        g.setColour(juce::Colours::white);
        g.drawText("CLIP", getWidth() / 2 - 20, 515, 40, 15, juce::Justification::centred);
    }
}

void AmpegSVTAudioProcessorEditor::resized()
{
}

void AmpegSVTAudioProcessorEditor::timerCallback()
{
    repaint();
}
