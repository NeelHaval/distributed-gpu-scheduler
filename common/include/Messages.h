#pragma once
#include <string>

// Namespace for specific protocol messages
namespace Messages {

    constexpr const char* REGISTER = "Register";
    constexpr const char* HEARTBEAT = "HEARTBEAT";
    constexpr const char* COMPLETE = "COMPLETE";
    constexpr const char* JOB = "JOB";
    constexpr const char* ACK = "ACK";

}