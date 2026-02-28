cmake_minimum_required(VERSION 3.24)
project(MixCopilotAU VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.6
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(juce Catch2)

juce_add_plugin(MixCopilotAU
    COMPANY_NAME "MixCopilot"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE MXCP
    PLUGIN_CODE MXC1
    FORMATS AU
    PRODUCT_NAME "MixCopilot"
)

target_sources(MixCopilotAU
    PRIVATE
        src/PluginProcessor.cpp
        src/PluginProcessor.h
        src/PluginEditor.cpp
        src/PluginEditor.h
)

target_compile_definitions(MixCopilotAU
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
)

target_link_libraries(MixCopilotAU
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

add_library(mixcopilot_core STATIC
    src/audio/FeatureExtractor.cpp
    src/audio/FeatureExtractor.h
    src/analysis/MuddyDetector.cpp
    src/analysis/MuddyDetector.h
    src/nlp/IntentParser.cpp
    src/nlp/IntentParser.h
    src/engine/SuggestionEngine.cpp
    src/engine/SuggestionEngine.h
    src/engine/SessionState.cpp
    src/engine/SessionState.h
    src/ui/RationalePanel.cpp
    src/ui/RationalePanel.h
)
target_include_directories(mixcopilot_core PUBLIC src)
target_link_libraries(mixcopilot_core PUBLIC juce::juce_audio_basics juce::juce_gui_basics)

target_link_libraries(MixCopilotAU PRIVATE mixcopilot_core)

include(CTest)
if(BUILD_TESTING)
    list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")
    include(Catch)

    add_executable(mixcopilot_tests
        tests/audio/test_feature_extractor.cpp
        tests/analysis/test_muddy_detector.cpp
        tests/nlp/test_intent_parser.cpp
        tests/engine/test_suggestion_engine.cpp
        tests/engine/test_session_state.cpp
        tests/ui/test_rationale_format.cpp
    )
    target_link_libraries(mixcopilot_tests PRIVATE mixcopilot_core Catch2::Catch2WithMain)
    catch_discover_tests(mixcopilot_tests)
endif()
