#include "AudioEngine.h"

json AudioEngine::getDeviceList()
{
    json devices;
    json inputs, outputs;

    // Retrieve available device types
    auto deviceTypes = deviceManager.getAvailableDeviceTypes();

    for (const auto* type : *deviceTypes) // Iterate through device types
    {
        type->scanForDevices(); // Scan for devices

        auto inputNames = type->getDeviceNames(true);  // True for input devices
        auto outputNames = type->getDeviceNames(false); // False for output devices

        for (const auto& input : inputNames)
            inputs.push_back(input.toStdString());

        for (const auto& output : outputNames)
            outputs.push_back(output.toStdString());
    }

    devices["inputs"] = inputs;
    devices["outputs"] = outputs;
    return devices;
}

json AudioEngine::setInputDevice(const std::string& deviceName)
{
    json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    setup.inputDeviceName = deviceName;

    auto setupSuccess = deviceManager.setAudioDeviceSetup(setup, true); // Configure input device
    if (setupSuccess) // Explicit boolean check
    {
        response["status"] = "success";
        response["message"] = "Input device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = "Failed to set input device"; // Error message
    }

    return response;
}

json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    setup.outputDeviceName = deviceName;

    auto setupSuccess = deviceManager.setAudioDeviceSetup(setup, true); // Configure output device
    if (setupSuccess) // Explicit boolean check
    {
        response["status"] = "success";
        response["message"] = "Output device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = "Failed to set output device"; // Error message
    }

    return response;
}
