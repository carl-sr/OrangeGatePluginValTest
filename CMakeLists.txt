cmake_minimum_required(VERSION 3.24.1)

set(PROJECT_NAME "OrangeGate")

set(FORMATS VST3 Standalone)

file(STRINGS VERSION CURRENT_VERSION)

project(${PROJECT_NAME} VERSION ${CURRENT_VERSION})

set_property(GLOBAL PROPERTY USE_FOLDERS YES)

option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Show all module sources in IDE projects" ON)

add_subdirectory(JUCE)

juce_add_plugin("${PROJECT_NAME}"
    COMPANY_NAME VainAudio
    BUNDLE_ID com.VainAudio.VainAudio
    COPY_PLUGIN_AFTER_BUILD FALSE # TODO permissions issues here
    PLUGIN_MANUFACTURER_CODE Vain
    PLUGIN_CODE Vain
    FORMATS "${FORMATS}"
    PRODUCT_NAME "${PROJECT_NAME}")
	

target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_20)

# target_compile_options("${PROJECT_NAME}" PRIVATE
#   $<$<CXX_COMPILER_ID:MSVC>:/WX /W2>
#   $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
# )

if(MSVC)
  target_compile_options("${PROJECT_NAME}" PUBLIC /W4 /WX)
else()
  target_compile_options("${PROJECT_NAME}" PUBLIC -Wall -Wextra -Wpedantic -Werror)
endif()

set(SourceFiles
	Source/OrangeGate.h
	Source/AboutPage.cpp
	Source/AboutPage.h
	Source/AnimatedToggleButton.cpp
	Source/AnimatedToggleButton.h
	Source/Apvts.cpp
	Source/Apvts.h
	Source/BackgroundImage.cpp
	Source/BackgroundImage.h
	Source/BoxShadow.cpp
	Source/BoxShadow.h
	Source/ControlPanel.cpp
	Source/ControlPanel.h
	Source/Filter.cpp
	Source/Filter.h
	Source/FilterView.cpp
	Source/FilterView.h
	Source/FilterVisualizer.cpp
	Source/FilterVisualizer.h
	Source/GateMultiplier.cpp
	Source/GateMultiplier.h
	Source/GateProcessor.cpp
	Source/GateProcessor.h
	Source/Header.cpp
	Source/Header.h
	Source/LookAndFeels.cpp
	Source/LookAndFeels.h
	Source/PluginEditor.cpp
	Source/PluginEditor.h
	Source/PluginProcessor.cpp
	Source/PluginProcessor.h
	Source/ProcessorInterface.h
	Source/RmsLevelCalculator.cpp
	Source/RmsLevelCalculator.h
	Source/SpectrumComponent.cpp
	Source/SpectrumComponent.h
	Source/SpectrumComponentBase.cpp
	Source/SpectrumComponentBase.h
	Source/SpectrumLevelValue.cpp
	Source/SpectrumLevelValue.h
	Source/SpectrumProcessor.cpp
	Source/SpectrumProcessor.h
	Source/Visualizer.cpp
	Source/Visualizer.h
	Source/VisualizerMonitor.cpp
	Source/VisualizerMonitor.h
	Source/VuMeter.cpp
	Source/VuMeter.h
	)
	
target_sources("${PROJECT_NAME}" PRIVATE ${SourceFiles})

set_target_properties("${PROJECT_NAME}" PROPERTIES FOLDER "")

juce_add_binary_data(Assets SOURCES
	Images/background.png
	Images/header.png
	Images/Source/logo.svg
	Images/Source/logo_large.svg
	Assets/RosaSans-Regular.ttf
	)

foreach(target ${FORMATS} "All")
    if(TARGET ${PROJECT_NAME}_${target})
        set_target_properties(${PROJECT_NAME}_${target} PROPERTIES FOLDER "Targets")
    endif()
endforeach()
set_target_properties(Assets PROPERTIES FOLDER "Targets")

target_compile_definitions("${PROJECT_NAME}"
    PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries("${PROJECT_NAME}"
    PRIVATE
    Assets
    PUBLIC
    juce::juce_audio_utils
    juce::juce_audio_processors
	juce::juce_dsp
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

