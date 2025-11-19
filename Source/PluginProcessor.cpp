#include "PluginProcessor.h"
#include "PluginEditor.h"

FuzzaAudioProcessor::FuzzaAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(
          BusesProperties()
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
#endif
      apvts(*this, nullptr, "Parameters", createParameterLayout()) {
}

FuzzaAudioProcessor::~FuzzaAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout
FuzzaAudioProcessor::createParameterLayout() {
  juce::AudioProcessorValueTreeState::ParameterLayout layout;

  layout.add(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"GAIN", 1}, "Gain",
      juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f, 0.5f), 50.0f));

  layout.add(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"TONE", 1}, "Tone",
      juce::NormalisableRange<float>(100.0f, 20000.0f, 1.0f, 0.3f), 5000.0f));

  layout.add(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"VOLUME", 1}, "Volume",
      juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));

  return layout;
}

const juce::String FuzzaAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool FuzzaAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool FuzzaAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool FuzzaAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double FuzzaAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int FuzzaAudioProcessor::getNumPrograms() {
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not really
            // implementing programs.
}

int FuzzaAudioProcessor::getCurrentProgram() { return 0; }

void FuzzaAudioProcessor::setCurrentProgram(int index) {}

const juce::String FuzzaAudioProcessor::getProgramName(int index) { return {}; }

void FuzzaAudioProcessor::changeProgramName(int index,
                                            const juce::String &newName) {}

void FuzzaAudioProcessor::prepareToPlay(double sampleRate,
                                        int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
}

void FuzzaAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FuzzaAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

  // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

void FuzzaAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                       juce::MidiBuffer &midiMessages) {
  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  // In case we have more outputs than inputs, this code clears any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  auto gain = apvts.getRawParameterValue("GAIN")->load();
  auto volume = apvts.getRawParameterValue("VOLUME")->load();
  // Tone is not implemented yet in DSP, just a placeholder parameter

  // Simple Fuzz Algorithm: Hard Clipping with Gain
  for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    auto *channelData = buffer.getWritePointer(channel);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
      float in = channelData[sample];

      // Apply input gain
      in *= gain;

      // Hard clipping (Fuzz)
      if (in > 1.0f)
        in = 1.0f;
      else if (in < -1.0f)
        in = -1.0f;

      // Apply output volume
      channelData[sample] = in * volume;
    }
  }
}

bool FuzzaAudioProcessor::hasEditor() const {
  return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *FuzzaAudioProcessor::createEditor() {
  return new FuzzaAudioProcessorEditor(*this);
}

void FuzzaAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries
  auto state = apvts.copyState();
  std::unique_ptr<juce::XmlElement> xml(state.createXml());
  copyXmlToBinary(*xml, destData);
}

void FuzzaAudioProcessor::setStateInformation(const void *data,
                                              int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
  std::unique_ptr<juce::XmlElement> xmlState(
      getXmlFromBinary(data, sizeInBytes));
  if (xmlState.get() != nullptr)
    if (xmlState->hasTagName(apvts.state.getType()))
      apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new FuzzaAudioProcessor();
}
