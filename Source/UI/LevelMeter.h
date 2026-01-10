#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

/**
 * Level Meter Component
 * Displays audio level with peak hold and clip indication
 */
class LevelMeter : public juce::Component,
                   private juce::Timer
{
public:
    LevelMeter()
    {
        startTimerHz (30);
    }
    
    ~LevelMeter() override
    {
        stopTimer();
    }
    
    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();
        
        // Background
        g.setColour (juce::Colour (0xff1a1a1a));
        g.fillRect (bounds);
        
        // Border
        g.setColour (juce::Colour (0xff3a3a3a));
        g.drawRect (bounds, 1.0f);
        
        // Level indicator
        float levelHeight = bounds.getHeight() * currentLevel.load();
        
        auto levelBounds = bounds;
        levelBounds.removeFromTop (bounds.getHeight() - levelHeight);
        
        // Color gradient based on level
        if (isClipping.load())
        {
            g.setColour (juce::Colours::red);
        }
        else if (currentLevel.load() > 0.9f)
        {
            g.setColour (juce::Colours::orange);
        }
        else if (currentLevel.load() > 0.7f)
        {
            g.setColour (juce::Colours::yellow);
        }
        else
        {
            g.setColour (juce::Colours::green);
        }
        
        g.fillRect (levelBounds);
        
        // Peak hold
        if (peakLevel > 0.0f)
        {
            float peakY = bounds.getHeight() * (1.0f - peakLevel);
            g.setColour (juce::Colours::white);
            g.drawLine (bounds.getX(), peakY, bounds.getRight(), peakY, 2.0f);
        }
        
        // Clip indicator
        if (isClipping.load())
        {
            g.setColour (juce::Colours::red);
            g.fillRect (bounds.getX(), bounds.getY(), bounds.getWidth(), 10.0f);
        }
    }
    
    void setLevel (float level)
    {
        currentLevel.store (juce::jlimit (0.0f, 1.0f, level));
        
        // Update peak hold
        if (level > peakLevel)
        {
            peakLevel = level;
            peakHoldCounter = 0;
        }
        
        repaint();
    }
    
    void setClipping (bool clipping)
    {
        isClipping.store (clipping);
        repaint();
    }
    
private:
    void timerCallback() override
    {
        // Decay peak hold
        if (peakHoldCounter++ > 30) // Hold for ~1 second at 30 Hz
        {
            peakLevel *= 0.95f;
            if (peakLevel < 0.01f)
                peakLevel = 0.0f;
            repaint();
        }
    }
    
    std::atomic<float> currentLevel { 0.0f };
    std::atomic<bool> isClipping { false };
    float peakLevel = 0.0f;
    int peakHoldCounter = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
