#include "PluginProcessor.h"
#include "PluginEditor.h"

SVTCLAudioProcessor::SVTCLAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

SVTCLAudioProcessor::~SVTCLAudioProcessor()
{
}

const juce::String SVTCLAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SVTCLAudioProcessor::acceptsMidi() const
{
    return false;
}

bool SVTCLAudioProcessor::producesMidi() const
{
    return false;
}

bool SVTCLAudioProcessor::isMidiEffect() const
{
    return false;
}

double SVTCLAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SVTCLAudioProcessor::getNumPrograms()
{
    return 5; // Factory presets
}

int SVTCLAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SVTCLAudioProcessor::setCurrentProgram(int index)
{
    // Load preset based on index
    switch (index)
    {
        case 0: // Default
            apvts.getParameter("gain")->setValueNotifyingHost(0.3f);
            apvts.getParameter("bass")->setValueNotifyingHost(0.5f);
            apvts.getParameter("mid")->setValueNotifyingHost(0.5f);
            apvts.getParameter("treble")->setValueNotifyingHost(0.5f);
            apvts.getParameter("master")->setValueNotifyingHost(0.5f);
            break;
        case 1: // Bright
            apvts.getParameter("gain")->setValueNotifyingHost(0.4f);
            apvts.getParameter("bass")->setValueNotifyingHost(0.4f);
            apvts.getParameter("mid")->setValueNotifyingHost(0.6f);
            apvts.getParameter("treble")->setValueNotifyingHost(0.7f);
            apvts.getParameter("ultraHi")->setValueNotifyingHost(0.3f);
            apvts.getParameter("master")->setValueNotifyingHost(0.5f);
            break;
        case 2: // Deep
            apvts.getParameter("gain")->setValueNotifyingHost(0.3f);
            apvts.getParameter("bass")->setValueNotifyingHost(0.7f);
            apvts.getParameter("mid")->setValueNotifyingHost(0.4f);
            apvts.getParameter("treble")->setValueNotifyingHost(0.4f);
            apvts.getParameter("ultraLo")->setValueNotifyingHost(0.2f);
            apvts.getParameter("master")->setValueNotifyingHost(0.5f);
            break;
        case 3: // Growl
            apvts.getParameter("gain")->setValueNotifyingHost(0.6f);
            apvts.getParameter("bass")->setValueNotifyingHost(0.6f);
            apvts.getParameter("mid")->setValueNotifyingHost(0.7f);
            apvts.getParameter("treble")->setValueNotifyingHost(0.5f);
            apvts.getParameter("midFreq")->setValueNotifyingHost(0.5f);
            apvts.getParameter("master")->setValueNotifyingHost(0.6f);
            break;
        case 4: // Clean
            apvts.getParameter("gain")->setValueNotifyingHost(0.2f);
            apvts.getParameter("bass")->setValueNotifyingHost(0.5f);
            apvts.getParameter("mid")->setValueNotifyingHost(0.5f);
            apvts.getParameter("treble")->setValueNotifyingHost(0.6f);
            apvts.getParameter("master")->setValueNotifyingHost(0.4f);
            break;
    }
}

const juce::String SVTCLAudioProcessor::getProgramName(int index)
{
    switch (index)
    {
        case 0: return "Default";
        case 1: return "Bright";
        case 2: return "Deep";
        case 3: return "Growl";
        case 4: return "Clean";
        default: return "Unknown";
    }
}

void SVTCLAudioProcessor::changeProgramName(int, const juce::String&)
{
    // Not implemented
}

void SVTCLAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32)samplesPerBlock;
    spec.numChannels = (juce::uint32)getTotalNumOutputChannels();
    
    svtProcessor.prepare(spec);
    updateProcessorFromParameters();
}

void SVTCLAudioProcessor::releaseResources()
{
    svtProcessor.reset();
}

bool SVTCLAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void SVTCLAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    updateProcessorFromParameters();
    svtProcessor.process(buffer);
}

bool SVTCLAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SVTCLAudioProcessor::createEditor()
{
    return new SVTCLAudioProcessorEditor(*this);
}

void SVTCLAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SVTCLAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorValueTreeState::ParameterLayout SVTCLAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "gain", "Gain", juce::NormalisableRange<float>(0.0f, 60.0f, 0.1f), 20.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "bass", "Bass", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "mid", "Mid", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "treble", "Treble", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "midFreq", "Mid Frequency", 
        juce::StringArray{"220 Hz", "450 Hz", "800 Hz", "1.6 kHz", "3 kHz"}, 2));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "ultraLo", "Ultra Lo", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "ultraHi", "Ultra Hi", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "master", "Master", juce::NormalisableRange<float>(-40.0f, 10.0f, 0.1f), 0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterBool>(
        "pad", "Pad", false));
    
    layout.add(std::make_unique<juce::AudioParameterBool>(
        "cabSim", "Cab Sim", false));
    
    layout.add(std::make_unique<juce::AudioParameterBool>(
        "standby", "Standby", false));

    return layout;
}

void SVTCLAudioProcessor::updateProcessorFromParameters()
{
    auto gain = apvts.getRawParameterValue("gain")->load();
    auto bass = apvts.getRawParameterValue("bass")->load();
    auto mid = apvts.getRawParameterValue("mid")->load();
    auto treble = apvts.getRawParameterValue("treble")->load();
    auto midFreq = (int)apvts.getRawParameterValue("midFreq")->load();
    auto ultraLo = apvts.getRawParameterValue("ultraLo")->load();
    auto ultraHi = apvts.getRawParameterValue("ultraHi")->load();
    auto master = apvts.getRawParameterValue("master")->load();
    auto pad = apvts.getRawParameterValue("pad")->load() > 0.5f;
    auto cabSim = apvts.getRawParameterValue("cabSim")->load() > 0.5f;
    auto standby = apvts.getRawParameterValue("standby")->load() > 0.5f;
    
    svtProcessor.setGain(gain);
    svtProcessor.setBass(bass);
    svtProcessor.setMid(mid);
    svtProcessor.setTreble(treble);
    svtProcessor.setMidFrequency(static_cast<ToneStack::MidFrequency>(midFreq));
    svtProcessor.setUltraLo(ultraLo);
    svtProcessor.setUltraHi(ultraHi);
    svtProcessor.setMaster(master);
    svtProcessor.setPad(pad);
    svtProcessor.setCabEnabled(cabSim);
    svtProcessor.setStandby(standby);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SVTCLAudioProcessor();
}
