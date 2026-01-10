#include "LevelMeter.h"

LevelMeter::LevelMeter()
{
    startTimerHz(30);
}

void LevelMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(2.0f);
    
    // Draw background
    g.setColour(juce::Colour(0xff1a1a1a));
    g.fillRect(bounds);
    
    // Draw level
    float levelHeight = displayLevel * bounds.getHeight();
    
    g.setColour(displayLevel > 0.9f ? juce::Colours::red : juce::Colours::green);
    g.fillRect(bounds.getX(), bounds.getBottom() - levelHeight, bounds.getWidth(), levelHeight);
    
    // Draw border
    g.setColour(juce::Colour(0xff5a5a5a));
    g.drawRect(bounds, 1.0f);
}

void LevelMeter::timerCallback()
{
    float targetLevel = level.load();
    
    // Smooth decay
    if (targetLevel > displayLevel)
        displayLevel = targetLevel;
    else
        displayLevel *= 0.95f;
    
    repaint();
}

void LevelMeter::setLevel(float newLevel)
{
    level.store(juce::jlimit(0.0f, 1.0f, newLevel));
}
