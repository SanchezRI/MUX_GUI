//#define DEBUG_TEST
#include "GuiWindows.h"

#ifndef DEBUG_TEST
void GuiWindows::ShowPacketTracerWindow(AppState& state) {
    ImGui::Begin("Packet Tracer", &state.show_packet_tracer_window);
    ImGui::Text("ModbusTCP Communication Tracer");
    ImGui::Separator();

    // Log controlling
    if (ImGui::Button("Clear Log")) {
        state.modbus.clearPacketLog();
    } ImGui::SameLine();

    static bool autoScroll = true;
    ImGui::Checkbox("Auto-scroll", &autoScroll); ImGui::SameLine();
    static bool showErrorsOnly = false;
    ImGui::Checkbox("Show Errors Only", &showErrorsOnly); ImGui::SameLine();

    // Filters
    static bool showSent = true;
    static bool showReceived = true;
    ImGui::Checkbox("Show Sent", &showSent); ImGui::SameLine();
    ImGui::Checkbox("Show Received", &showReceived); ImGui::SameLine();

    ImGui::Separator();

    // Table with packets
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
    ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 15);

    if (ImGui::BeginTable("PacketTable", 5,
        ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable, outer_size))
    {
        ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, 120.0f);
        ImGui::TableSetupColumn("Direction", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Function", ImGuiTableColumnFlags_WidthFixed, 150.0f);
        ImGui::TableSetupColumn("Details", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Data (HEX)", ImGuiTableColumnFlags_WidthFixed, 200.0f);
        ImGui::TableHeadersRow();

        auto packets = state.modbus.getPacketLog();
        for (const auto& packet : packets)
        {
            // Filtering
            if ((!showSent && packet.direction == "Sent") ||
                (!showReceived && packet.direction == "Received") ||
                (showErrorsOnly && !packet.isError))
                continue;

            ImGui::TableNextRow();

            // Time
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", packet.timestamp.c_str());

            // Direction
            ImGui::TableSetColumnIndex(1);
            if (packet.direction == "Sent") {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(100, 200, 255, 255));
            }
            else {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(100, 255, 100, 255));
            }
            ImGui::Text("%s", packet.direction.c_str());
            ImGui::PopStyleColor();

            // Function
            ImGui::TableSetColumnIndex(2);
            if (packet.isError) {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 100, 100, 255));
            }
            ImGui::Text("%s", packet.function.c_str());
            if (packet.isError) {
                ImGui::PopStyleColor();
            }

            // Details
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s", packet.details.c_str());

            // Data
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%s", packet.dataHex.c_str());

            // Tooltip with ASCII view
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("ASCII: %s", packet.dataAscii.c_str());
                ImGui::EndTooltip();
            }
        }

        // Autoscroll
        if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndTable();
    }
    ImGui::End();
}

#endif
