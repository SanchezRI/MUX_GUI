#include "GuiWindows.h"

//////////////////////////////////// Multiplexers (Commutator window) ////////////////////////////////////
void GuiWindows::ShowMultiplexerWindow(AppState& state) {

    ModbusTcp modbus("192.168.127.254", 502, 3000);

    ImGui::Begin("Commutator", &state.show_multiplexer_window, state.window_flags);
    ImGui::Text("Here is Commutator window. That controls Multiplexers in commutator unit");

    // Connection and Popup
    if (ImGui::Button("Connect"))
        ImGui::OpenPopup("Connect?"); ImGui::SameLine(); ImGui::SetItemTooltip("Connect to MUX-device...");

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Connect?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Connect to MUX-device?");
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            try {
                modbus.connect();
                state.mux_connection = true;
            }
            catch (const std::exception &err) {
                state.mux_connection = false;
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
                modbus.disconnect();
                state.mux_connection = false;
            }
            catch (const std::exception& err) {
                state.mux_connection = true;
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus(); ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }

    // Status
    ImGui::ColorButton("MUX Status",
        state.mux_connection ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1),
        ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20)); ImGui::SameLine();
    ImGui::Text("Connection status: %s", state.mux_connection ? "Connected" : "Disconnected");

    if (!state.mux_connection) {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Device is disconnected!");
        ImGui::End();
        return;
    } ImGui::SameLine();

    // Polling
    if (ImGui::Checkbox("Polling", &state.mux_polling)) {
        if (state.mux_polling == true) {
            std::cout << "Callback: Turn on poll_command !!!!!" << std::endl;
        } else {
            std::cout << "Callback: Turn off poll_command !!!!!" << std::endl;
        }
    }

    static char str0[128] = "192.168.127.254";
    if (ImGui::InputText("IP-address", str0, IM_ARRAYSIZE(str0))) {
        std::cout << "Callback: New IP-adress: " << str0 << std::endl;
    }

    static int int0 = 502;
    if (ImGui::InputInt("Port", &int0)) {
        std::cout << "Callback: New port-number: " << int0 << std::endl;
    }

    // MUXES tabs
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MUXsTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("2 x 1 to 4 ports MUX"))
        {
            ImGui::Text("This is the MUX with ID=2 tab!");
            /// 1.x-2.1 port
            ImGui::SeparatorText("Setters for 1.*x2.1 ports");
            ImGui::BeginGroup(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.1 x 2.1 port")) {
                modbus.writeSingleRegister(10, 12, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.2 x 2.1 port")) {
                modbus.writeSingleRegister(10, 13, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.3 x 2.1 port")) {
                modbus.writeSingleRegister(10, 14, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.4 x 2.1 port")) {
                modbus.writeSingleRegister(10, 15, 1);
                modbus.getPacketLog();
            }
            ImGui::EndGroup();

            /// 1.x-2.2 port
            ImGui::SeparatorText("Setters for 1.*x2.2 ports");
            ImGui::BeginGroup(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.1 x 2.2 port")) {
                modbus.writeSingleRegister(10, 16, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.2 x 2.2 port")) {
                modbus.writeSingleRegister(10, 17, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.3 x 2.2 port")) {
                modbus.writeSingleRegister(10, 18, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.4 x 2.2 port")) {
                modbus.writeSingleRegister(10, 19, 1);
                modbus.getPacketLog();
            }
            ImGui::EndGroup();

            /// 1.x-2.3 port
            ImGui::SeparatorText("Setters for 1.*x2.3 ports");
            ImGui::BeginGroup(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.1 x 2.3 port")) {
                modbus.writeSingleRegister(10, 32, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.2 x 2.3 port")) {
                modbus.writeSingleRegister(10, 33, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.3 x 2.3 port")) {
                modbus.writeSingleRegister(10, 34, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.4 x 2.3 port")) {
                modbus.writeSingleRegister(10, 35, 1);
                modbus.getPacketLog();
            }
            ImGui::EndGroup();

            /// 1.x-2.4 port
            ImGui::SeparatorText("Setters for 1.*x2.4 ports");
            ImGui::BeginGroup(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.1 x 2.4 port")) {
                modbus.writeSingleRegister(10, 48, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.2 x 2.4 port")) {
                modbus.writeSingleRegister(10, 49, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.3 x 2.4 port")) {
                modbus.writeSingleRegister(10, 50, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 1.4 x 2.4 port")) {
                modbus.writeSingleRegister(10, 51, 1);
                modbus.getPacketLog();
            }
            ImGui::EndGroup();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("1 x 1 to 8 ports MUX"))
        {
            ImGui::Text("This is the MUX with ID=3 tab!");
            /// 1-4 port
            ImGui::SeparatorText("Setters for 1 - 4 ports");
            ImGui::BeginGroup(); ImGui::NextColumn();
            if (ImGui::Button("Set 1 port")) {
                modbus.writeSingleRegister(11, 0, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 2 port")) {
                modbus.writeSingleRegister(11, 1, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 3 port")) {
                modbus.writeSingleRegister(11, 2, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 4 port")) {
                modbus.writeSingleRegister(11, 3, 1);
                modbus.getPacketLog();
            }
            ImGui::EndGroup();

            /// 5-8 port
            ImGui::SeparatorText("Setters for 5 - 8 ports");
            ImGui::BeginGroup(); ImGui::NextColumn();
            if (ImGui::Button("Set 5 port")) {
                modbus.writeSingleRegister(11, 4, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 6 port")) {
                modbus.writeSingleRegister(11, 5, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 7 port")) {
                modbus.writeSingleRegister(11, 6, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set 8 port")) {
                modbus.writeSingleRegister(11, 7, 1);
                modbus.getPacketLog();
            }
            ImGui::EndGroup();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("4 x 1 to 2 pors MUX"))
        {
            ImGui::Text("This is the MUX with ID=4 tab!");
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("1 x 2 to 6 pors MUX"))
        {
            ImGui::Text("This is the MUX with ID=1 tab!");
            /// In1 port
            ImGui::SeparatorText("Setters for In1 port");
            ImGui::BeginGroup(); ImGui::NextColumn();
            if (ImGui::Button("Set In1 x Out1 port")) {
                modbus.writeSingleRegister(12, 1, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In1 x Out2 port")) {
                modbus.writeSingleRegister(12, 2, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In1 x Out3 port")) {
                modbus.writeSingleRegister(12, 3, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In1 x Out4 port")) {
                modbus.writeSingleRegister(12, 4, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In1 x Out5 port")) {
                modbus.writeSingleRegister(12, 5, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In1 x Out6 port")) {
                modbus.writeSingleRegister(12, 6, 1);
                modbus.getPacketLog();
            }
            ImGui::EndGroup();

            /// In2 port
            ImGui::BeginGroup(); ImGui::NextColumn();
            ImGui::SeparatorText("Setters for In2 port");
            if (ImGui::Button("Set In2 x Out1 port")) {
                modbus.writeSingleRegister(12, 17, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In2 x Out2 port")) {
                modbus.writeSingleRegister(12, 18, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In2 x Out3 port")) {
                modbus.writeSingleRegister(12, 19, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In2 x Out4 port")) {
                modbus.writeSingleRegister(12, 20, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In2 x Out5 port")) {
                modbus.writeSingleRegister(12, 21, 1);
                modbus.getPacketLog();
            }
            ImGui::SameLine(); ImGui::NextColumn();
            if (ImGui::Button("Set In2 x Out6 port")) {
                modbus.writeSingleRegister(12, 22, 1);
                modbus.getPacketLog();
            }
            ImGui::EndGroup();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::Separator();

    ImGui::End();
}
///////////////////////////////////////////////// END MUX ///////////////////////////////////////////////