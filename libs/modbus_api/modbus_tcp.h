#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <mutex>
#include <future>
#include <functional>
#include <future>
#include <thread>
#include <atomic>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <iostream>
#include <algorithm>
#include "imgui.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SOCKET int
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#endif

class ModbusTcp {
public:
    struct Packet {
        std::string timestamp;
        std::string direction;
        std::string function;
        std::string details;
        std::string dataHex;
        std::string dataAscii;
        bool isError = false;
    };

    ModbusTcp(const std::string& serverIp, uint16_t serverPort, uint32_t timeoutMs);
    ModbusTcp();
    ~ModbusTcp();

    bool connect();
    void disconnect();
    bool isConnected();

    std::vector<bool> readCoils(uint16_t startAddr, uint16_t quantity, uint8_t unitId);
    std::vector<uint16_t> readHoldingRegisters(uint16_t startAddr, uint16_t quantity, uint8_t unitId);
    std::vector<uint16_t> readInputRegisters(uint16_t startAddr, uint16_t quantity, uint8_t unitId);
    bool writeSingleRegister(uint16_t addr, uint16_t value, uint8_t unitId);
    bool writeMultipleRegisters(uint16_t startAddr, const std::vector<uint16_t>& values, uint8_t unitId);
    std::string processRegisters(const std::vector<uint16_t>& registers,
        size_t startPos, size_t length, bool reverseBytes);

    void startPolling(uint16_t startAddr, uint16_t quantity,
        std::function<void(const std::vector<uint16_t>&)> callback,
        uint16_t intervalMs = 1000, uint8_t unitId = 1);
    void stopPolling();

    std::vector<Packet> getPacketLog() const;
    void clearPacketLog();

private:
    std::vector<uint16_t> readRegisters(uint8_t functionCode, 
        uint16_t startAddr, uint16_t quantity, uint8_t unitId);
    bool sendRequest(const std::vector<uint8_t>& request);
    bool receiveResponse(uint8_t* buffer, size_t size);
    std::string getModbusErrorString(uint8_t code);
    std::string getFunctionName(uint8_t code) const;
    void addToPacketLog(const std::string& direction, const std::string& function,
        const std::vector<uint8_t>& data, bool isError = false);

    SOCKET socket_ = INVALID_SOCKET;
    std::string server_ip;
    uint16_t server_port;
    uint32_t timeout_ms;

    std::atomic<bool> polling_active_{ false };
    std::thread polling_thread_;

    mutable std::mutex log_mutex_;
    std::vector<Packet> packet_log_;
};