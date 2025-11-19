cmake_minimum_required(VERSION 3.15)

project(Fuzza VERSION 0.3.2)

# Add JUCE
include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.3 # Using a specific tag for stability
)
FetchContent_MakeAvailable(juce)

# Create the plugin target
juce_add_plugin(Fuzza
    COMPANY_NAME "h4ppy Labs"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE AntG
    PLUGIN_CODE Fuzz
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "Fuzza"
    ICON_BIG "${CMAKE_CURRENT_SOURCE_DIR}/Resources/icon.png"
    ICON_SMALL "${CMAKE_CURRENT_SOURCE_DIR}/Resources/icon.png"
)

# Add source files
target_sources(Fuzza
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h
)

# Link JUCE modules
target_link_libraries(Fuzza
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
