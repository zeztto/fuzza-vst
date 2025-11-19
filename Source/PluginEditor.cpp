#include "PluginEditor.h"
#include "PluginProcessor.h"

FuzzaAudioProcessorEditor::FuzzaAudioProcessorEditor(FuzzaAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
  // Bypass Button
  bypassButton.setButtonText("Bypass");
  bypassButton.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
  bypassButton.setColour(juce::ToggleButton::tickColourId, juce::Colours::red);
  bypassButton.setColour(juce::ToggleButton::tickDisabledColourId, juce::Colours::grey);
  addAndMakeVisible(bypassButton);

  bypassAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
          audioProcessor.apvts, "BYPASS", bypassButton);

  // Gain Slider
  gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
  addAndMakeVisible(gainSlider);

  gainLabel.setText("Gain", juce::dontSendNotification);
  gainLabel.setJustificationType(juce::Justification::centred);
  gainLabel.attachToComponent(&gainSlider, false);
  addAndMakeVisible(gainLabel);

  gainAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.apvts, "GAIN", gainSlider);

  // Tone Slider
  toneSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  toneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
  addAndMakeVisible(toneSlider);

  toneLabel.setText("Tone", juce::dontSendNotification);
  toneLabel.setJustificationType(juce::Justification::centred);
  toneLabel.attachToComponent(&toneSlider, false);
  addAndMakeVisible(toneLabel);

  toneAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.apvts, "TONE", toneSlider);

  setSize(400, 300);
}

FuzzaAudioProcessorEditor::~FuzzaAudioProcessorEditor() {}

void FuzzaAudioProcessorEditor::paint(juce::Graphics &g) {
  g.fillAll(juce::Colours::black);

  g.setColour(juce::Colours::white);
  g.setFont(30.0f);
  g.drawFittedText("Fuzza", getLocalBounds().removeFromTop(50),
                   juce::Justification::centred, 1);
}

void FuzzaAudioProcessorEditor::resized() {
  auto bounds = getLocalBounds();

  // Bypass button at the top right
  auto topArea = bounds.removeFromTop(50);
  bypassButton.setBounds(topArea.removeFromRight(100).reduced(10));

  // Two sliders at the bottom (Gain and Tone only)
  auto area = bounds.removeFromBottom(200);
  int dialWidth = area.getWidth() / 2;

  gainSlider.setBounds(area.removeFromLeft(dialWidth).reduced(10));
  toneSlider.setBounds(area.removeFromLeft(dialWidth).reduced(10));
}
