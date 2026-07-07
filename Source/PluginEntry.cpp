#include "PluginProcessor.h"

juce::AudioProcessor* createPluginFilter()
{
    return new VocalSuiteUltraProAudioProcessor();
}
