#pragma once
#include "imgui.h"
#include "modbus_tcp.h"

struct AppState {

    // Slave windows main params [used in Master]
    bool show_adc_dac_window = true;
    bool show_multiplexer_window = true;
    bool show_packet_tracer_window = true;

    // ADC-DAC's params and states
    bool adc_polling = false;
    bool adc_connection = false;

    // Commutator's params and states
    bool mux_polling = false;
    bool mux_connection = false;
    ModbusTcp modbus;

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