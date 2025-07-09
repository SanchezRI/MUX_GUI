#include "modbus_tcp.h"

/// Constructors and destructors

ModbusTcp::ModbusTcp(const std::string& serverIp, uint16_t serverPort, uint32_t timeoutMs)
    : server_ip(serverIp), server_port(serverPort), timeout_ms(timeoutMs) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
    }
#endif
}

ModbusTcp::ModbusTcp() {
#ifdef _WIN32
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed" << std::endl;
	}
#endif
}
ModbusTcp::~ModbusTcp() {
    stopPolling();
    disconnect();
#ifdef _WIN32
    WSACleanup();
#endif
}

/// Base ModbusTCP methods

bool ModbusTcp::connect() {
    if (isConnected()) return true;

    socket_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        return false;
    }

#ifdef _WIN32
    setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout_ms, sizeof(timeout_ms));
    setsockopt(socket_, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout_ms, sizeof(timeout_ms));
#else
    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;
    setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
    setsockopt(socket_, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv));
#endif

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip.c_str(), &serverAddr.sin_addr);

    if (::connect(socket_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed" << std::endl;
        closesocket(socket_);
        socket_ = INVALID_SOCKET;
        return false;
    }

    return true;
}

void ModbusTcp::disconnect() {
    if (socket_ != INVALID_SOCKET) {
        closesocket(socket_);
        socket_ = INVALID_SOCKET;
    }
}

bool ModbusTcp::isConnected() {
	if (socket_ == INVALID_SOCKET) {
		return false;
	}

	fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(socket_, &read_fds);

	fd_set error_fds;
	FD_ZERO(&error_fds);
	FD_SET(socket_, &error_fds);

	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	int select_result = select(
		static_cast<int>(socket_) + 1,
		&read_fds,
		nullptr,
		&error_fds,
		&timeout
	);

	if (select_result == SOCKET_ERROR) {
#ifdef _WIN32
		std::cerr << "select() failed: " << WSAGetLastError() << std::endl;
#else
		std::cerr << "select() failed: " << strerror(errno) << std::endl;
#endif
		return false;
	}

	if (FD_ISSET(socket_, &error_fds)) {
		return false;
	}

	return true;
}

bool ModbusTcp::sendRequest(const std::vector<uint8_t>& request) {
	if (send(socket_, reinterpret_cast<const char*>(request.data()), request.size(), 0) == SOCKET_ERROR) {
		std::cerr << "Send failed" << std::endl;
		disconnect();
		return false;
	}

	// Log sent packet
	std::string funcName = "Unknown";
	if (request.size() >= 7) {
		funcName = getFunctionName(request[7]);
	}
	addToPacketLog("Sent", funcName, request);

	return true;
}

bool ModbusTcp::receiveResponse(uint8_t* buffer, size_t size) {
	int bytesReceived = recv(socket_, reinterpret_cast<char*>(buffer), size, 0);
	if (bytesReceived <= 0) {
		std::cerr << "Receive failed" << std::endl;
		disconnect();
		return false;
	}

	// Log received packet
	std::vector<uint8_t> response(buffer, buffer + bytesReceived);
	std::string funcName = "Unknown";
	bool isError = false;

	if (response.size() >= 7) {
		isError = (response[7] & 0x80) != 0;
		funcName = isError ? "Error Response" : getFunctionName(response[7]);
	}

	addToPacketLog("Received", funcName, response, isError);

	return true;
}

/// Packet tracer methods

void ModbusTcp::addToPacketLog(const std::string & direction, const std::string & function,
    const std::vector<uint8_t>&data, bool isError) {
    std::lock_guard<std::mutex> lock(log_mutex_);

    Packet packet;

    // Get current time
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    char timestamp_buf[20];
    strftime(timestamp_buf, sizeof(timestamp_buf), "%H:%M:%S", localtime(&now_time));
    packet.timestamp = timestamp_buf;

    packet.direction = direction;
    packet.function = function;
    packet.isError = isError;

    // HEX representation
    std::ostringstream hexStream;
    hexStream << std::hex << std::setfill('0');
    for (uint8_t byte : data) {
        hexStream << std::setw(2) << static_cast<int>(byte) << " ";
    }
    packet.dataHex = hexStream.str();

    // ASCII representation
    std::ostringstream asciiStream;
    for (uint8_t byte : data) {
        if (byte >= 32 && byte <= 126) {
            asciiStream << static_cast<char>(byte);
        }
        else {
            asciiStream << ".";
        }
    }
    packet.dataAscii = asciiStream.str();

    // Packet details
    if (data.size() >= 7) {
        std::ostringstream detailsStream;
        detailsStream << "UnitID: " << static_cast<int>(data[6]) << ", ";
        detailsStream << "Func: 0x" << std::hex << static_cast<int>(data[7]);

        if (data[7] & 0x80) { // Error response
            detailsStream << " (Error: " << getModbusErrorString(data[8]) << ")";
        }
        packet.details = detailsStream.str();
    }

    packet_log_.push_back(packet);

    // Limit log size
    if (packet_log_.size() > 1000) {
        packet_log_.erase(packet_log_.begin());
    }
}

std::string ModbusTcp::getFunctionName(uint8_t code) const {
    switch (code) {
    case 0x01: return "Read Coils";
    case 0x02: return "Read Discrete Inputs";
    case 0x03: return "Read Holding Registers";
    case 0x04: return "Read Input Registers";
    case 0x05: return "Write Single Coil";
    case 0x06: return "Write Single Register";
    case 0x10: return "Write Multiple Registers";
    default: return "Unknown Function";
    }
}

std::vector<ModbusTcp::Packet> ModbusTcp::getPacketLog() const {

	for (const auto& packet : packet_log_) {
		std::cout << "[DEBUG] " << "Time: " << packet.timestamp
			<< ", Direction: " << packet.direction
			<< ", Function: " << packet.function
			<< ", Details: " << packet.details
			<< ", Data: " << packet.dataHex << std::endl;
	}

	return packet_log_;
}

void ModbusTcp::clearPacketLog() {
    std::lock_guard<std::mutex> lock(log_mutex_);
    packet_log_.clear();
}

std::string ModbusTcp::getModbusErrorString(uint8_t code) {
	switch (code) {
	case 0x01: return "Illegal Function";
	case 0x02: return "Illegal Data Address";
	case 0x03: return "Illegal Data Value";
	case 0x04: return "Slave Device Failure";
	case 0x05: return "Acknowledge";
	case 0x06: return "Slave Device Busy";
	case 0x08: return "Memory Parity Error";
	case 0x0A: return "Gateway Path Unavailable";
	case 0x0B: return "Gateway Target Device Failed to Respond";
	default: return "Unknown error";
	}
}

/// Polling methods

void ModbusTcp::startPolling(uint16_t startAddr, uint16_t quantity, std::function<void(const std::vector<uint16_t>&)> callback,
	uint16_t interval_ms, uint8_t unitId) {
	stopPolling();
	polling_active_ = true;
	polling_thread_ = std::thread([this, startAddr, quantity, callback, interval_ms, unitId]() {
		while (polling_active_) {
			auto registers = readHoldingRegisters(startAddr, quantity, unitId);
			if (!registers.empty()) {
				callback(registers);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
		}
		});
}

void ModbusTcp::stopPolling() {
	polling_active_ = false;
	if (polling_thread_.joinable()) {
		polling_thread_.join();
	}
}

/// Registrers methods

std::vector<uint16_t> ModbusTcp::readRegisters(uint8_t functionCode, uint16_t startAddr, uint16_t quantity, uint8_t unitId) {
	std::vector<uint16_t> result;

	if (!isConnected() && !connect()) {
		std::cerr << "Not connected to server" << std::endl;
		return result;
	}

	if (quantity > 125) {
		std::cerr << "Quantity too large (max 125)" << std::endl;
		return result;
	}

	std::vector<uint8_t> request = {
		0x00, 0x01, // Transaction ID
		0x00, 0x00, // Protocol ID
		0x00, 0x06, // Length
		unitId,     // Unit ID
		functionCode, // Function Code
		static_cast<uint8_t>(startAddr >> 8),
		static_cast<uint8_t>(startAddr & 0xFF),
		static_cast<uint8_t>(quantity >> 8),
		static_cast<uint8_t>(quantity & 0xFF)
	};

	if (sendRequest(request)) {
		uint8_t header[8];
		if (receiveResponse(header, sizeof(header))) {
			if (header[7] & 0x80) {
				std::cerr << "Modbus error: " << getModbusErrorString(header[8]) << std::endl;
				return result;
			}

			if (header[7] != functionCode) {
				std::cerr << "Invalid function code in response" << std::endl;
				return result;
			}

			uint8_t byteCount;
			if (recv(socket_, reinterpret_cast<char*>(&byteCount), 1, 0) != 1) {
				std::cerr << "Byte count receive failed" << std::endl;
				disconnect();
				return result;
			}

			std::vector<uint8_t> data(byteCount);
			if (recv(socket_, reinterpret_cast<char*>(data.data()), byteCount, 0) != byteCount) {
				std::cerr << "Data receive failed" << std::endl;
				disconnect();
				return result;
			}

			for (size_t i = 0; i < data.size(); i += 2) {
				uint16_t value = (data[i] << 8) | data[i + 1];
				result.push_back(value);
			}
		}
	}

	return result;
}

std::vector<bool> ModbusTcp::readCoils(uint16_t startAddr, uint16_t quantity, uint8_t unitId = 1) {
	std::vector<bool> result;

	if (!isConnected() && !connect()) {
		std::cerr << "Not connected to server" << std::endl;
		return result;
	}

	if (quantity > 2000) {
		std::cerr << "Quantity too large (max 2000)" << std::endl;
		return result;
	}

	std::vector<uint8_t> request = {
		0x00, 0x01, // Transaction ID
		0x00, 0x00, // Protocol ID
		0x00, 0x06, // Length
		unitId,     // Unit ID
		0x01,       // Function Code
		static_cast<uint8_t>(startAddr >> 8),
		static_cast<uint8_t>(startAddr & 0xFF),
		static_cast<uint8_t>(quantity >> 8),
		static_cast<uint8_t>(quantity & 0xFF)
	};

	if (sendRequest(request)) {
		uint8_t header[8];
		if (receiveResponse(header, sizeof(header))) {
			if (header[7] & 0x80) {
				std::cerr << "Modbus error: " << getModbusErrorString(header[8]) << std::endl;
				return result;
			}

			if (header[7] != 0x01) {
				std::cerr << "Invalid function code in response" << std::endl;
				return result;
			}

			uint8_t byteCount;
			if (recv(socket_, reinterpret_cast<char*>(&byteCount), 1, 0) != 1) {
				std::cerr << "Byte count receive failed" << std::endl;
				disconnect();
				return result;
			}

			std::vector<uint8_t> data(byteCount);
			if (recv(socket_, reinterpret_cast<char*>(data.data()), byteCount, 0) != byteCount) {
				std::cerr << "Data receive failed" << std::endl;
				disconnect();
				return result;
			}

			for (int i = 0; i < quantity; ++i) {
				int byteIndex = i / 8;
				int bitIndex = i % 8;
				bool value = (data[byteIndex] & (1 << bitIndex)) != 0;
				result.push_back(value);
			}
		}
	}

	return result;
}

bool ModbusTcp::writeMultipleRegisters(uint16_t startAddr, const std::vector<uint16_t>& values, uint8_t unitId = 1) {
	if (!isConnected() && !connect()) {
		std::cerr << "Not connected to server" << std::endl;
		return false;
	}

	uint16_t quantity = static_cast<uint16_t>(values.size());
	uint8_t byteCount = quantity * 2;

	std::vector<uint8_t> request = {
		0x00, 0x01, // Transaction ID
		0x00, 0x00, // Protocol ID
		0x00, static_cast<uint8_t>(7 + byteCount), // Length
		unitId,     // Unit ID
		0x10,       // Function Code
		static_cast<uint8_t>(startAddr >> 8),
		static_cast<uint8_t>(startAddr & 0xFF),
		static_cast<uint8_t>(quantity >> 8),
		static_cast<uint8_t>(quantity & 0xFF),
		byteCount
	};

	for (auto value : values) {
		request.push_back(static_cast<uint8_t>(value >> 8));
		request.push_back(static_cast<uint8_t>(value & 0xFF));
	}

	if (sendRequest(request)) {
		uint8_t response[12];
		if (receiveResponse(response, sizeof(response))) {
			return response[7] == 0x10;
		}
	}
	return false;
}

bool ModbusTcp::writeSingleRegister(uint16_t addr, uint16_t value, uint8_t unitId = 1) {
	if (!isConnected() && !connect()) {
		std::cerr << "Not connected to server" << std::endl;
		return false;
	}

	std::vector<uint8_t> request = {
		0x00, 0x01, // Transaction ID
		0x00, 0x00, // Protocol ID
		0x00, 0x06, // Length
		unitId,     // Unit ID
		0x06,       // Function Code
		static_cast<uint8_t>(addr >> 8),
		static_cast<uint8_t>(addr & 0xFF),
		static_cast<uint8_t>(value >> 8),
		static_cast<uint8_t>(value & 0xFF)
	};

	if (sendRequest(request)) {
		uint8_t response[12];
		if (receiveResponse(response, sizeof(response))) {
			return response[7] == 0x06;
		}
	}
	return false;
}

std::vector<uint16_t> ModbusTcp::readHoldingRegisters(uint16_t startAddr, uint16_t quantity, uint8_t unitId = 1) {
	return readRegisters(0x03, startAddr, quantity, unitId);
}

std::vector<uint16_t> ModbusTcp::readInputRegisters(uint16_t startAddr, uint16_t quantity, uint8_t unitId = 1) {
	return readRegisters(0x04, startAddr, quantity, unitId);
}

std::string ModbusTcp::processRegisters(const std::vector<uint16_t>& registers,
	size_t startPos, size_t length, bool reverseBytes) {
	std::stringstream hexStream;

	for (uint16_t reg : registers) {
		if (reverseBytes) {
			reg = (reg >> 8) | (reg << 8);
		}
		hexStream << std::hex << std::setw(4) << std::setfill('0') << reg;
	}

	std::string hexString = hexStream.str();

	if (startPos >= hexString.length()) {
		return "";
	}

	if (startPos + length > hexString.length()) {
		length = hexString.length() - startPos;
	}

	return hexString.substr(startPos, length);
}