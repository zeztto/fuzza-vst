#pragma once

#include "PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>

class FuzzaAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
  FuzzaAudioProcessorEditor(FuzzaAudioProcessor &);
  ~FuzzaAudioProcessorEditor() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  FuzzaAudioProcessor &audioProcessor;

  juce::Slider gainSlider;
  juce::Slider toneSlider;
  juce::Slider volumeSlider;

  juce::Label gainLabel;
  juce::Label toneLabel;
  juce::Label volumeLabel;

  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      gainAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      toneAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      volumeAttachment;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FuzzaAudioProcessorEditor)
};
