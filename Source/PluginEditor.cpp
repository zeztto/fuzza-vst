#include "PluginEditor.h"
#include "PluginProcessor.h"

FuzzaAudioProcessorEditor::FuzzaAudioProcessorEditor(FuzzaAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p) {

  // === KNOBS (3x rotary) ===
  auto setupKnob = [this](juce::Slider& slider, juce::Label& label,
                          const juce::String& labelText) {
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffff9500)); // Orange
    slider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    slider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0x00000000)); // Transparent
    addAndMakeVisible(slider);

    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colours::white);
    label.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);
  };

  setupKnob(gainSlider, gainLabel, "GAIN");
  setupKnob(mixSlider, mixLabel, "MIX");
  setupKnob(gateSlider, gateLabel, "GATE");

  gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.apvts, "GAIN", gainSlider);
  mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.apvts, "MIX", mixSlider);
  gateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.apvts, "GATE", gateSlider);

  // === TONE PRESET BUTTONS ===
  auto setupToneButton = [this](juce::TextButton& button, const juce::String& text, int preset) {
    button.setButtonText(text);
    button.setClickingTogglesState(false);
    button.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff3a3a3a));
    button.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff9500)); // Orange when selected
    button.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    button.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    button.onClick = [this, preset]() {
      audioProcessor.apvts.getParameter("TONE_PRESET")->setValueNotifyingHost(preset / 2.0f);
      updateToneButtons(preset);
    };
    addAndMakeVisible(button);
  };

  setupToneButton(toneWarmButton, "WARM", 0);
  setupToneButton(toneBalancedButton, "BALANCED", 1);
  setupToneButton(toneBrightButton, "BRIGHT", 2);

  toneLabel.setText("TONE", juce::dontSendNotification);
  toneLabel.setJustificationType(juce::Justification::centred);
  toneLabel.setColour(juce::Label::textColourId, juce::Colours::white);
  toneLabel.setFont(juce::FontOptions(12.0f, juce::Font::bold));
  addAndMakeVisible(toneLabel);

  // === CLIPPING MODE BUTTONS ===
  auto setupClipButton = [this](juce::TextButton& button, const juce::String& text, int mode) {
    button.setButtonText(text);
    button.setClickingTogglesState(false);
    button.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff3a3a3a));
    button.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff9500)); // Orange when selected
    button.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    button.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    button.onClick = [this, mode]() {
      audioProcessor.apvts.getParameter("CLIP_MODE")->setValueNotifyingHost(mode / 2.0f);
      updateClipButtons(mode);
    };
    addAndMakeVisible(button);
  };

  setupClipButton(clipHardButton, "HARD", 0);
  setupClipButton(clipSoftButton, "SOFT", 1);
  setupClipButton(clipAsymButton, "ASYM", 2);

  clipLabel.setText("CLIPPING", juce::dontSendNotification);
  clipLabel.setJustificationType(juce::Justification::centred);
  clipLabel.setColour(juce::Label::textColourId, juce::Colours::white);
  clipLabel.setFont(juce::FontOptions(12.0f, juce::Font::bold));
  addAndMakeVisible(clipLabel);

  // === BYPASS FOOTSWITCH ===
  bypassButton.setButtonText("BYPASS");
  bypassButton.setClickingTogglesState(true);
  bypassButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff333333));
  bypassButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffdd0000)); // Red when on
  bypassButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
  bypassButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
  addAndMakeVisible(bypassButton);

  bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
      audioProcessor.apvts, "BYPASS", bypassButton);

  // Initialize button states
  updateToneButtons(1); // Balanced default
  updateClipButtons(0); // Hard default

  setSize(500, 420);
}

FuzzaAudioProcessorEditor::~FuzzaAudioProcessorEditor() {}

void FuzzaAudioProcessorEditor::updateToneButtons(int selectedPreset) {
  toneWarmButton.setToggleState(selectedPreset == 0, juce::dontSendNotification);
  toneBalancedButton.setToggleState(selectedPreset == 1, juce::dontSendNotification);
  toneBrightButton.setToggleState(selectedPreset == 2, juce::dontSendNotification);
}

void FuzzaAudioProcessorEditor::updateClipButtons(int selectedMode) {
  clipHardButton.setToggleState(selectedMode == 0, juce::dontSendNotification);
  clipSoftButton.setToggleState(selectedMode == 1, juce::dontSendNotification);
  clipAsymButton.setToggleState(selectedMode == 2, juce::dontSendNotification);
}

void FuzzaAudioProcessorEditor::paint(juce::Graphics &g) {
  // Pedal enclosure background (dark grey with gradient)
  auto bounds = getLocalBounds();
  juce::ColourGradient gradient(
      juce::Colour(0xff1a1a1a), bounds.getX(), bounds.getY(),
      juce::Colour(0xff2d2d2d), bounds.getX(), bounds.getBottom(),
      false);
  g.setGradientFill(gradient);
  g.fillAll();

  // Border (metallic look)
  g.setColour(juce::Colour(0xff555555));
  g.drawRect(bounds, 3);

  // Top logo area
  auto logoArea = bounds.removeFromTop(60);
  g.setColour(juce::Colours::white);
  g.setFont(juce::FontOptions(36.0f, juce::Font::bold));
  g.drawFittedText("FUZZA", logoArea, juce::Justification::centred, 1);

  // Subtitle
  g.setFont(juce::FontOptions(12.0f));
  g.setColour(juce::Colour(0xffff9500)); // Orange
  g.drawFittedText("Modern Fuzz Pedal", logoArea.removeFromBottom(15),
                   juce::Justification::centred, 1);

  // Draw screws (pedal realism)
  auto drawScrew = [&g](int x, int y) {
    g.setColour(juce::Colour(0xff888888));
    g.fillEllipse(x - 4, y - 4, 8, 8);
    g.setColour(juce::Colour(0xff444444));
    g.drawLine(x - 3, y, x + 3, y, 1.0f);
  };

  drawScrew(15, 15);
  drawScrew(getWidth() - 15, 15);
  drawScrew(15, getHeight() - 15);
  drawScrew(getWidth() - 15, getHeight() - 15);
}

void FuzzaAudioProcessorEditor::resized() {
  auto bounds = getLocalBounds().reduced(20);

  // Top: Logo area
  bounds.removeFromTop(60);
  bounds.removeFromTop(10);

  // Tone preset buttons row
  auto toneRow = bounds.removeFromTop(50);
  toneRow.removeFromTop(15); // Space for label
  auto toneButtons = toneRow.removeFromTop(30);
  int toneButtonWidth = toneButtons.getWidth() / 3;
  toneWarmButton.setBounds(toneButtons.removeFromLeft(toneButtonWidth).reduced(5, 0));
  toneBalancedButton.setBounds(toneButtons.removeFromLeft(toneButtonWidth).reduced(5, 0));
  toneBrightButton.setBounds(toneButtons.removeFromLeft(toneButtonWidth).reduced(5, 0));

  bounds.removeFromTop(5);

  // Clipping mode buttons row
  auto clipRow = bounds.removeFromTop(50);
  clipRow.removeFromTop(15); // Space for label
  auto clipButtons = clipRow.removeFromTop(30);
  int clipButtonWidth = clipButtons.getWidth() / 3;
  clipHardButton.setBounds(clipButtons.removeFromLeft(clipButtonWidth).reduced(5, 0));
  clipSoftButton.setBounds(clipButtons.removeFromLeft(clipButtonWidth).reduced(5, 0));
  clipAsymButton.setBounds(clipButtons.removeFromLeft(clipButtonWidth).reduced(5, 0));

  bounds.removeFromTop(10);

  // Knobs area (3 knobs in a row)
  auto knobArea = bounds.removeFromTop(140);
  int knobWidth = knobArea.getWidth() / 3;

  gainSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(20, 0));
  mixSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(20, 0));
  gateSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(20, 0));

  bounds.removeFromTop(10);

  // Bypass footswitch (bottom center)
  auto bypassArea = bounds.removeFromBottom(60);
  bypassButton.setBounds(bypassArea.withSizeKeepingCentre(120, 50));
}
