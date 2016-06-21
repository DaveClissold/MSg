/*
  ==============================================================================

    PluginSlotComponent.cpp
    Created: 25 Aug 2015 7:20:43pm
    Author:  Gary Jones

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginSlotComponent.h"
#include "MainViewComponent.h"
#include "PluginWindow.h"

#define REMOVEPLUGIN  10000
#define LOADPRESET    10001
#define CLEARSETTINGS 10002
#define SCANPLUGINS   10003

//==============================================================================
PluginSlotComponent::PluginSlotComponent()
{
    menu_pluginList = new PopupMenu();
    
    button_pluginNamePopupMenu = new TextButton();
    button_pluginNamePopupMenu->setButtonText("No Plugin Assigned");
    button_pluginNamePopupMenu->addListener (this);
    button_pluginNamePopupMenu->setColour(TextButton::buttonColourId, Colours::transparentWhite);
    button_pluginNamePopupMenu->setColour(TextButton::buttonOnColourId, Colours::transparentWhite);
    button_pluginNamePopupMenu->setColour(TextButton::textColourOnId, Colours::white);
    button_pluginNamePopupMenu->setColour(TextButton::textColourOffId, Colours::white);
    addAndMakeVisible(button_pluginNamePopupMenu);
    
    button_openGUIMid = new TextButton();
    button_openGUIMid->setButtonText("Mid");
    button_openGUIMid->addListener (this);
    button_openGUIMid->setColour(TextButton::buttonColourId, Colours::transparentWhite);
    button_openGUIMid->setColour(TextButton::buttonOnColourId, Colours::transparentWhite);
    button_openGUIMid->setColour(TextButton::textColourOnId, Colours::white);
    button_openGUIMid->setColour(TextButton::textColourOffId, Colours::white);
    addAndMakeVisible(button_openGUIMid);
    
    button_openGUISide = new TextButton();
    button_openGUISide->setButtonText("Side");
    button_openGUISide->addListener (this);
    button_openGUISide->setColour(TextButton::buttonColourId, Colours::transparentWhite);
    button_openGUISide->setColour(TextButton::buttonOnColourId, Colours::transparentWhite);
    button_openGUISide->setColour(TextButton::textColourOnId, Colours::white);
    button_openGUISide->setColour(TextButton::textColourOffId, Colours::white);
    addAndMakeVisible(button_openGUISide);
}

PluginSlotComponent::~PluginSlotComponent()
{
}



void PluginSlotComponent::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == button_openGUIMid)
    {
        MainViewComponent * mv = (MainViewComponent*)getParentComponent();
        PluginAssignProcessor * f = mv->processor->pluginAssignProcessor[0];
        if (f->hasPlugin)
        {
            AudioProcessor * plugin = f->instance;
            PluginWindow* const w = PluginWindow::openWindowFor (plugin, false, true);
            w->toFront (true);
        }
        else
        {
            // could do popup alert msg
        }
    }
    else if (buttonThatWasClicked == button_openGUISide)
    {
        MainViewComponent * mv = (MainViewComponent*)getParentComponent();
        PluginAssignProcessor * f = mv->processor->pluginAssignProcessor[1];
        if (f->hasPlugin)
        {
            AudioProcessor * plugin = f->instance;
            PluginWindow* const w = PluginWindow::openWindowFor (plugin, false, false);
            w->toFront (true);
        }
        else
        {
            // could do popup alert msg
        }
    }
    else if (buttonThatWasClicked == button_pluginNamePopupMenu)
    {
        MainViewComponent * mv = (MainViewComponent*)getParentComponent();

        PopupMenu auMenu; // remove AU menu link for win version... // comment for compile
        PopupMenu vstMenu;
        PopupMenu vst3Menu;
        
        // do search for plugins and add to au/vst menu
        for (int i = 0; i < mv->processor->knownPluginList->getNumTypes(); i++)
        {
            PluginDescription * desc = mv->processor->knownPluginList->getType(i);
            
            if (!desc->isInstrument)
            {
                if (desc->pluginFormatName == "AudioUnit")
                {
                    auMenu.addItem(i+1, desc->name); // remove AU menu link for win version... // comment for compile
                }
                else if (desc->pluginFormatName =="VST")
                {
                    vstMenu.addItem(i+1, desc->name);
                }
                else if (desc->pluginFormatName =="VST3")
                {
                    vst3Menu.addItem(i+1, desc->name);
                }
                // might need extra for VST3..
            }
        }
        
    
        PopupMenu loadMenu;
        
        
        loadMenu.addSubMenu("AU", auMenu); // remove AU menu link for win version... // comment for compile
        loadMenu.addSubMenu("VST", vstMenu);
        loadMenu.addSubMenu("VST3", vst3Menu);
        
        // add vst/au menu to loadmenu
        PopupMenu menu; // main menu
        menu.addSubMenu("Load Plugin", loadMenu);
        
        // remove plugin need code defined..
        menu.addItem(REMOVEPLUGIN, "Remove Plugin");
      //  menu.addItem(CLEARSETTINGS, "Clear Settings");
        menu.addItem(SCANPLUGINS, "Scan for new plugins");

        const int result = menu.show();
        if (result)
        {
            if (result == REMOVEPLUGIN)
            {
                
                
                //*** to setParameter()
                int paramIndex = (0*TOTAL_SLOT_PARAMS)+SLOT_PLUGINREF;
                int paramIndex2 = (1*TOTAL_SLOT_PARAMS)+SLOT_PLUGINREF;
                mv->processor->setParameter(paramIndex, -1.0);
                mv->processor->setParameter(paramIndex2, -1.0);
                
                PluginWindow::closeAllCurrentlyOpenWindows();
                
                PluginAssignProcessor * f = mv->processor->pluginAssignProcessor[0];
                int result = f->clearPlugin();
                if (result) {
                    button_pluginNamePopupMenu->setButtonText("No Plugin Assigned");
                }
                
                PluginAssignProcessor * f2 = mv->processor->pluginAssignProcessor[1];
                result = f2->clearPlugin();
                if (result) {
                    button_pluginNamePopupMenu->setButtonText("No Plugin Assigned");
                }
                
            }
            else if (result == CLEARSETTINGS)
            {
                
            }
            else if (result == SCANPLUGINS)
            {
                mv->rescanPlugins();
            }
            else
            {
                // load plugin to slot
                
                PluginWindow::closeAllCurrentlyOpenWindows();
                
                // load selected plugin into both slots
                
                int paramIndex = (0 * TOTAL_SLOT_PARAMS) + SLOT_PLUGINREF;
                int paramIndex2 = (1 * TOTAL_SLOT_PARAMS) + SLOT_PLUGINREF;
                mv->processor->setParameter(paramIndex, result-1);
                mv->processor->setParameter(paramIndex2, result-1);
            }

        }
    }
}

void PluginSlotComponent::openParameterListBox()
{
    // creat listbox, then add items to list..
    printf("stage 2");
    MainViewComponent * mv = (MainViewComponent*)getParentComponent();
    int numParams = mv->processor->pluginAssignProcessor[slot_index]->getNumParameters();
    for (int i = 0; i < numParams; i++)
    {
        printf("stage 3, num params = %i", numParams);
        String paramName = mv->processor->pluginAssignProcessor[slot_index]->getParameterName(i);
        String valString(mv->processor->pluginAssignProcessor[slot_index]->getParameter(i));
    }
}
void PluginSlotComponent::paint (Graphics& g)
{
}

void PluginSlotComponent::resized()
{
    button_pluginNamePopupMenu->setBounds(16, 6, 170, 23);
    button_openGUIMid->setBounds(186, 6, 37, 23);
    button_openGUISide->setBounds(186+37, 6, 37, 23);
}
