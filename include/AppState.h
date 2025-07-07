#pragma once
#include "imgui.h"
#include "modbus_tcp.h"

struct AppState {

    // Slave windows main params [used in Master]
    bool show_commutator_window = true;

    // Commutator's params and states
    bool comm_connection = false;
    bool comm_polling = false;
    bool mux_slot_id_1 = false;
    bool mux_slot_id_2 = false;
    bool mux_slot_id_3 = false;
    bool mux_slot_id_4 = false;

    bool show_progress_popup = false;
    float progress = 0.0f;
    ModbusTcp comm_modbus;
    std::future<void> connection_future;
    std::string error_message;
    std::mutex state_mutex;
    std::mutex modbus_mutex;

    // Window's style settings
    ImGuiWindowFlags window_flags = 0;
};