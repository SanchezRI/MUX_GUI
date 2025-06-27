#pragma once
#include "imgui.h"
#include "modbus_tcp.h"

struct AppState {

    // Slave windows main params [used in Master]
    bool show_adc_dac_window = false;
    bool show_commutator_window = true;
    bool show_packet_tracer_window = true;

    // ADC-DAC's params and states
    bool adc_polling = false;
    bool adc_connection = false;

    // Commutator's params and states
    bool comm_connection = false;
    bool comm_polling = false;
    bool mux_slot_id_1 = false;
    bool mux_slot_id_2 = false;
    bool mux_slot_id_3 = false;
    bool mux_slot_id_4 = false;
    ModbusTcp comm_modbus;

    // Window's style settings
    bool show_style_editor = false;
    ImGuiWindowFlags window_flags = 0;
    bool no_titlebar = false;
    bool no_scrollbar = false;
    bool no_menu = false;
    bool no_move = false;
    bool no_resize = false;
    bool no_collapse = false;
    bool no_close = false;
    bool no_nav = false;
    bool no_background = false;
    bool no_bring_to_front = false;
    bool no_docking = false;
    bool unsaved_document = false;
};