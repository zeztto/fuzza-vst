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

  // Knobs (3x instead of 4)
  juce::Slider gainSlider;
  juce::Slider mixSlider;
  juce::Slider gateSlider;

  // Labels
  juce::Label gainLabel;
  juce::Label mixLabel;
  juce::Label gateLabel;

  // Bypass footswitch button (pedal style)
  juce::TextButton bypassButton;

  // Tone preset buttons (3-way toggle)
  juce::TextButton toneWarmButton;
  juce::TextButton toneBalancedButton;
  juce::TextButton toneBrightButton;
  juce::Label toneLabel;

  // Clipping mode buttons (3-way toggle)
  juce::TextButton clipHardButton;
  juce::TextButton clipSoftButton;
  juce::TextButton clipAsymButton;
  juce::Label clipLabel;

  // Attachments
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
      bypassAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      gainAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      mixAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      gateAttachment;

  // Helper methods
  void updateToneButtons(int selectedPreset);
  void updateClipButtons(int selectedMode);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FuzzaAudioProcessorEditor)
};
