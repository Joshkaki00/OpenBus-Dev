#pragma once
#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <juce_core/juce_core.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <nlohmann/json.hpp>

class AudioEngine
{
public:
    static AudioEngine& getInstance()
    {
        static AudioEngine instance; // Thread-safe in C++11 and later
        return instance;
    }

    nlohmann::json getDeviceList(); // Ensure this matches the definition in the .cpp file
    nlohmann::json setInputDevice(const std::string& deviceName); // Match this
    nlohmann::json setOutputDevice(const std::string& deviceName); // Match this

    // Other public methods
    bool loadPlugin(const juce::File& file);
    bool savePreset(const juce::File& file);
    bool loadPreset(const juce::File& file);

private:
    AudioEngine() = default; // Private constructor
    ~AudioEngine() = default;

    // Delete copy constructor and assignment operator to prevent copies
    AudioEngine(const AudioEngine&) = delete;
    AudioEngine& operator=(const AudioEngine&) = delete;

    juce::AudioDeviceManager deviceManager; // Example member variable
};

#endif // AUDIOENGINE_H
