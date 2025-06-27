// #define DEBUG
#include "GuiWindows.h"

//////////////////////////////////// Multiplexers (Commutator window) ////////////////////////////////////
void GuiWindows::ShowMultiplexerWindow(AppState& state) {

    ModbusTcp comm_modbus("192.168.127.254", 502, 3000);

    ImGui::Begin("Commutator", &state.show_commutator_window, state.window_flags);
    ImGui::Text("Here is Commutator window. That controls Multiplexers in commutator unit."); 

    // Connection and Popup
    if (ImGui::Button("Connect"))
        ImGui::OpenPopup("Connect?"); ImGui::SameLine(); ImGui::SetItemTooltip("Connect to MUX-device...");

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    std::vector<uint16_t> input_reg;
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Connect?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Connect to MUX-device?");
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            try {
                comm_modbus.connect();
                state.comm_connection = true;

                input_reg = comm_modbus.readInputRegisters(2, 1, 1);

                if (comm_modbus.processRegisters(input_reg, 1, 1, false) == ("1") 
                    || comm_modbus.processRegisters(input_reg, 2, 1, false) == ("1") 
                    || comm_modbus.processRegisters(input_reg, 3, 1, false) == ("1")) {
                    state.mux_slot_id_1 = true;
                }
                else {
                    state.mux_slot_id_1 = false;
                }

                if (comm_modbus.processRegisters(input_reg, 1, 1, false) == ("2") 
                    || comm_modbus.processRegisters(input_reg, 2, 1, false) == ("2")
                    || comm_modbus.processRegisters(input_reg, 3, 1, false) == ("2")) {
                    state.mux_slot_id_2 = true;
                }
                else {
                    state.mux_slot_id_2 = false;
                }

                if (comm_modbus.processRegisters(input_reg, 1, 1, false) == ("3")
                    || comm_modbus.processRegisters(input_reg, 2, 1, false) == ("3")
                    || comm_modbus.processRegisters(input_reg, 3, 1, false) == ("3")) {
                    state.mux_slot_id_3 = true;
                }
                else {
                    state.mux_slot_id_3 = false;
                }

                if (comm_modbus.processRegisters(input_reg, 1, 1, false) == ("4")
                    || comm_modbus.processRegisters(input_reg, 2, 1, false) == ("4")
                    || comm_modbus.processRegisters(input_reg, 3, 1, false) == ("4")) {
                    state.mux_slot_id_4 = true;
                }
                else {
                    state.mux_slot_id_4 = false;
                }
            }
            catch (const std::exception &err) {
                state.comm_connection = false;
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus(); ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }

    // Disconnection
    if (ImGui::Button("Disconnect"))
        ImGui::OpenPopup("Disconnect?"); ImGui::SameLine(); ImGui::SetItemTooltip("Disconnect MUX-device");

    if (ImGui::BeginPopupModal("Disconnect?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Disconnect MUX-device?");
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            try {
                comm_modbus.disconnect();
                state.comm_connection = false;
            }
            catch (const std::exception& err) {
                state.comm_connection = true;
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus(); ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }

    // Status
    ImGui::ColorButton("Commutator Status",
        state.comm_connection ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1),
        ImGuiColorEditFlags_NoTooltip, ImVec2(25, 25)); ImGui::SameLine();
    ImGui::Text("Commutator connection status: [%s]", state.comm_connection ? "Connected" : "Disconnected");

    if (!state.comm_connection) {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Device is disconnected!");
        ImGui::End();
        return;
    } ImGui::SameLine();

#ifdef DEBUG
    if (ImGui::Button("[DEBUG] INPUT REGISTER")) {
        auto input_reg = comm_modbus.readInputRegisters(2, 1, 1);
        std::cout << "=======" << std::endl;
        std::cout << comm_modbus.processRegisters(input_reg, 1, 1, false) << std::endl;
        std::cout << comm_modbus.processRegisters(input_reg, 2, 1, false) << std::endl;
        std::cout << comm_modbus.processRegisters(input_reg, 3, 1, false) << std::endl;
        std::cout << "=======" << std::endl;
    }
#endif // DEBUG

    // Polling
    if (ImGui::Checkbox("Polling", &state.comm_polling)) {
        if (state.comm_polling == true) {
            std::cout << "Callback: Turn on poll_command !!!!!" << std::endl;
        } else {
            std::cout << "Callback: Turn off poll_command !!!!!" << std::endl;
        }
    } ImGui::SameLine();

    ImGui::ColorButton("MUX1 Status",
        state.mux_slot_id_1 ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1),
        ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20)); ImGui::SameLine();
    ImGui::Text("MUX_ID = 1: [%s]", state.mux_slot_id_1 ? "Installed" : "Not Installed");

    static char str0[128] = "192.168.127.254";
    if (ImGui::InputText("IP-addr", str0, IM_ARRAYSIZE(str0))) {
        std::cout << "Callback: New IP-adress: " << str0 << std::endl;
    } ImGui::SameLine();

    ImGui::ColorButton("MUX2 Status",
        state.mux_slot_id_2 ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1),
        ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20)); ImGui::SameLine();
    ImGui::Text("MUX_ID = 2: [%s]", state.mux_slot_id_2 ? "Installed" : "Not Installed");

    static int int0 = 502;
    if (ImGui::InputInt("Port       ", &int0)) {
        std::cout << "Callback: New port-number: " << int0 << std::endl;
    } ImGui::SameLine();

    ImGui::ColorButton("MUX3 Status",
        state.mux_slot_id_3 ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1),
        ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20)); ImGui::SameLine();
    ImGui::Text("MUX_ID = 3: [%s]", state.mux_slot_id_3 ? "Installed" : "Not Installed");

    static int int1 = 1;
    if (ImGui::InputInt("Unit ID  ", &int1)) {
        std::cout << "Callback: New Unit_ID: " << int1 << std::endl;
    } ImGui::SameLine();

    ImGui::ColorButton("MUX4 Status",
        state.mux_slot_id_4 ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1),
        ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20)); ImGui::SameLine();
    ImGui::Text("MUX_ID = 4: [%s]", state.mux_slot_id_4 ? "Installed" : "Not Installed");

    // MUXES tabs
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MUXsTabBar", tab_bar_flags))
    {
        if (state.mux_slot_id_1 && ImGui::BeginTabItem("1 x 2 --> 6 pors MUX"))
        {
            ImGui::Text("This is the MUX with ID = 1 tab!");
            /// In1 port
            ImGui::SeparatorText("Setters for In1 port");
            ImGui::BeginGroup(); ImGui::NextColumn();
            if (ImGui::Button("Set In1 x Out1")) {
                comm_modbus.writeSingleRegister(12, 1, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In1 x Out2")) {
                comm_modbus.writeSingleRegister(12, 2, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In1 x Out3")) {
                comm_modbus.writeSingleRegister(12, 3, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In1 x Out4")) {
                comm_modbus.writeSingleRegister(12, 4, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In1 x Out5")) {
                comm_modbus.writeSingleRegister(12, 5, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In1 x Out6")) {
                comm_modbus.writeSingleRegister(12, 6, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::EndGroup();

            /// In2 port
            ImGui::BeginGroup(); ImGui::NextColumn();
            ImGui::SeparatorText("Setters for In2 port");
            if (ImGui::Button("Set In2 x Out1")) {
                comm_modbus.writeSingleRegister(12, 17, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In2 x Out2")) {
                comm_modbus.writeSingleRegister(12, 18, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In2 x Out3")) {
                comm_modbus.writeSingleRegister(12, 19, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In2 x Out4")) {
                comm_modbus.writeSingleRegister(12, 20, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In2 x Out5")) {
                comm_modbus.writeSingleRegister(12, 21, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In2 x Out6")) {
                comm_modbus.writeSingleRegister(12, 22, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::EndGroup();
            ImGui::EndTabItem();
        }

        if (state.mux_slot_id_2 && ImGui::BeginTabItem("2 x 4 --> 1 ports MUX"))
        {
            ImGui::Text("This is the MUX with ID = 2 tab!");
            /// 1.x-2.1 port
            ImGui::SeparatorText("Setters for 1.* x 2.1 ports");
            ImGui::BeginGroup(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.1 x 2.1 port")) {
                comm_modbus.writeSingleRegister(10, 12, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.2 x 2.1 port")) {
                comm_modbus.writeSingleRegister(10, 13, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.3 x 2.1 port")) {
                comm_modbus.writeSingleRegister(10, 14, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.4 x 2.1 port")) {
                comm_modbus.writeSingleRegister(10, 15, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::EndGroup();

            /// 1.x-2.2 port
            ImGui::SeparatorText("Setters for 1.* x 2.2 ports");
            ImGui::BeginGroup(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.1 x 2.2 port")) {
                comm_modbus.writeSingleRegister(10, 16, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.2 x 2.2 port")) {
                comm_modbus.writeSingleRegister(10, 17, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.3 x 2.2 port")) {
                comm_modbus.writeSingleRegister(10, 18, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.4 x 2.2 port")) {
                comm_modbus.writeSingleRegister(10, 19, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::EndGroup();

            /// 1.x-2.3 port
            ImGui::SeparatorText("Setters for 1.* x 2.3 ports");
            ImGui::BeginGroup(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.1 x 2.3 port")) {
                comm_modbus.writeSingleRegister(10, 32, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.2 x 2.3 port")) {
                comm_modbus.writeSingleRegister(10, 33, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.3 x 2.3 port")) {
                comm_modbus.writeSingleRegister(10, 34, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.4 x 2.3 port")) {
                comm_modbus.writeSingleRegister(10, 35, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::EndGroup();

            /// 1.x-2.4 port
            ImGui::SeparatorText("Setters for 1.* x 2.4 ports");
            ImGui::BeginGroup(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.1 x 2.4 port")) {
                comm_modbus.writeSingleRegister(10, 48, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.2 x 2.4 port")) {
                comm_modbus.writeSingleRegister(10, 49, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.3 x 2.4 port")) {
                comm_modbus.writeSingleRegister(10, 50, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.4 x 2.4 port")) {
                comm_modbus.writeSingleRegister(10, 51, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::EndGroup();
            ImGui::EndTabItem();
        }
        if (state.mux_slot_id_3 && ImGui::BeginTabItem("1 x 8 --> 1 ports MUX"))
        {
            ImGui::Text("This is the MUX with ID = 3 tab!");
            /// 1-4 port
            ImGui::SeparatorText("Setters for 1 - 4 ports");
            ImGui::BeginGroup(); ImGui::NextColumn();
            if (ImGui::Button("Set 1 port")) {
                comm_modbus.writeSingleRegister(11, 0, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 2 port")) {
                comm_modbus.writeSingleRegister(11, 1, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 3 port")) {
                comm_modbus.writeSingleRegister(11, 2, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 4 port")) {
                comm_modbus.writeSingleRegister(11, 3, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::EndGroup();

            /// 5-8 port
            ImGui::SeparatorText("Setters for 5 - 8 ports");
            ImGui::BeginGroup(); ImGui::NextColumn();
            if (ImGui::Button("Set 5 port")) {
                comm_modbus.writeSingleRegister(11, 4, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 6 port")) {
                comm_modbus.writeSingleRegister(11, 5, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 7 port")) {
                comm_modbus.writeSingleRegister(11, 6, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 8 port")) {
                comm_modbus.writeSingleRegister(11, 7, 1);
                comm_modbus.getPacketLog();
            }
            ImGui::EndGroup();
            ImGui::EndTabItem();
        }

        if (state.mux_slot_id_4 && ImGui::BeginTabItem("4 x 2 --> 1 pors MUX"))
        {
            ImGui::Text("This is the MUX with ID = 4 tab!");
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::Separator();

    ImGui::End();
}
///////////////////////////////////////////////// END MUX ///////////////////////////////////////////////