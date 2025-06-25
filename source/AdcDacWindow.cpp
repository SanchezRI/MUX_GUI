#include "GuiWindows.h"

////////////////////////////////////////////// ADC-DAC Module ///////////////////////////////////////////
void GuiWindows::ShowAdcDacWindow(AppState& state) {

    ImGui::Begin("ADC/DAC-Module", &state.show_adc_dac_window, state.window_flags);

    ImGui::Text("Here is ADC/DAC Module's window. That controls ADC/DAC Module");

    if (ImGui::Button("Connect"))
        ImGui::OpenPopup("Connect?"); ImGui::SameLine(); ImGui::SetItemTooltip("Connect to ADC-device via Tango API");

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Connect?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Connect to ADC-device?");
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            try {
                state.adc_connection = true;
            }
            catch (std::exception &err) {
                state.adc_connection = false;

            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus(); ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }

    if (ImGui::Button("Disconnect"))
        ImGui::OpenPopup("Disconnect?"); ImGui::SameLine(); ImGui::SetItemTooltip("Disconnect ADC-device");

    if (ImGui::BeginPopupModal("Disconnect?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Disconnect ADC-device?");
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            try {
                state.adc_connection = false;
            }
            catch (std::exception& err) {
                state.adc_connection = true;

            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus(); ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }

    ImGui::ColorButton("ADC Status",
        state.adc_connection ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1),
        ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20)); ImGui::SameLine();
    ImGui::Text("Connection status: %s", state.adc_connection ? "Connected" : "Disconnected");

    if (!state.adc_connection) {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Device is disconnected!");
        ImGui::End();
        return;
    } ImGui::SameLine();

    // Polling
    if (ImGui::Checkbox("Polling", &state.adc_polling)) {
        if (state.adc_polling == true) {
            std::cout << "Turn on poll_command !!!!!" << std::endl;
        }
        else {
            std::cout << "Turn off poll_command !!!!!" << std::endl;
        }
    }

    ImGui::NewLine();

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("ADCTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("ADC/DAC Settings"))
        {
            ImGui::Text("Here is setings for Analog Digital Converter");

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("PacketTracer"))
        {
            ImGui::Text("This is packet tracer between client and ADC-server devices.");
            
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::Separator();

    ImGui::End();
}
//////////////////////////////////////////////// END ADC-DAC ////////////////////////////////////////////