/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "MainViewComponent.h"


//==============================================================================
/**
*/
class AudioVitaminsAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    AudioVitaminsAudioProcessorEditor (AudioVitaminsAudioProcessor&);
    ~AudioVitaminsAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioVitaminsAudioProcessor& processor;

    ScopedPointer<MainViewComponent> mainViewComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioVitaminsAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
