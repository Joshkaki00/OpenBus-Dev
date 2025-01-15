#include "ZeroMQServer.h"
#include <iostream>

ZeroMQServer::ZeroMQServer()
    : socket(context, ZMQ_REP) {
    socket.bind("tcp://*:5555");
}

ZeroMQServer::~ZeroMQServer() {
    socket.close();
    context.close();
}

void ZeroMQServer::listen() {
    AudioEngine audioEngine;

    while (true) {
        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);

        try {
            std::string msg(static_cast<char*>(request.data()), request.size());
            nlohmann::json command = nlohmann::json::parse(msg);
            nlohmann::json response = processCommand(command, audioEngine);

            zmq::message_t reply(response.dump());
            socket.send(reply, zmq::send_flags::none);
        } catch (const std::exception& e) {
            std::cerr << "JSON parse error: " << e.what() << std::endl;

            nlohmann::json errorResponse = {
                {"status", "error"},
                {"message", "Invalid JSON"}
            };

            zmq::message_t reply(errorResponse.dump());
            socket.send(reply, zmq::send_flags::none);
        }
    }
}

nlohmann::json ZeroMQServer::processCommand(const nlohmann::json& command, AudioEngine& audioEngine) {
    if (command.contains("action")) {
        std::string action = command["action"];
        if (action == "get_devices") {
            return audioEngine.getDeviceList();
        } else if (action == "set_input" && command.contains("device_name")) {
            return audioEngine.setInputDevice(command["device_name"]);
        } else if (action == "set_output" && command.contains("device_name")) {
            return audioEngine.setOutputDevice(command["device_name"]);
        }
    }

    return {{"status", "error"}, {"message", "Unknown or invalid command"}};
}
