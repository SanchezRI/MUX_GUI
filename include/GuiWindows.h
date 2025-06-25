//
// Created by max-tango-virtual on 4/15/25.
//

#pragma once
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include "imgui.h"
#include "implot.h"
#include "AppState.h"
#include "modbus_tcp.h"

namespace GuiWindows {

    //////////////////////////////////////////// Master Window //////////////////////////////////////////
    void ShowMainWindow(AppState& state);

    //////////////////////////////////////////// Slave Windows //////////////////////////////////////////
    // ADC/DAC-module device
    void ShowAdcDacWindow(AppState& state);

    // Multiplexer / Commutators devices
    void ShowMultiplexerWindow(AppState& state);

    // Style Editor devices (ImGui build-in)
    void ShowStyleEditorWindow(AppState& state);

    // Packet Tracer window (only commutator)
    void ShowPacketTracerWindow(AppState& state);
    void ShowPacketTracerWindow(bool* p_open);
}