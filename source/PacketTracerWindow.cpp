#define DEBUG_TEST
#include "GuiWindows.h"

struct PacketTracer
{
    ImGuiTextBuffer Buf;
    bool ScrollToBottom;

    void Clear() { Buf.clear(); }

    void AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        ScrollToBottom = true;
    }

    void Draw(const char* title, bool* p_opened = NULL)
    {
        ImGui::Begin(title, p_opened);
        ImGui::TextUnformatted(Buf.begin());
        if (ScrollToBottom)
            ImGui::SetScrollHereY(1.0f);
        ScrollToBottom = false;
        ImGui::End();
    }
};

void GuiWindows::ShowPacketTracerWindow(AppState& state) {
    static PacketTracer log;

    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("PacketTracer: Communication traffic", &state.show_packet_tracer_window);

    //std::cout << "[DEBUG] COMMUTATOR CONNECTION STATUS: " << state.comm_connection << std::endl;

    const char* categories[3] = { "INFO", "WARNING", "ERROR" };
    auto packets = state.comm_modbus.getPacketLog();

    if (!state.comm_connection && packets.empty()) {
        log.AddLog("[%.2f sec] {%s}: Please build connection with ModbusTCP server.\n",
            ImGui::GetTime(), categories[2]);
    }
    else if (state.comm_connection) {
        if (!packets.empty()) {

            /*for (const auto& packet : packets) {
                log.AddLog("[%s] [%.2f sec] {%s} | %s | %s | %s | %s\n",
                    packet.timestamp.c_str(), ImGui::GetTime(), categories[0],
                    packet.direction.c_str(), packet.function.c_str(),
                    packet.details.c_str(), packet.dataHex.c_str());
            }*/

            log.AddLog("[%s] [%.2f sec] {%s} | %s | %s | %s | %s | ", packets.data()->timestamp.c_str(),
                ImGui::GetTime(), categories[0], packets.data()->direction.c_str(), 
                packets.data()->function.c_str(), packets.data()->details.c_str(), packets.data()->dataHex.c_str());
        }
        else {
            log.AddLog("[%.2f sec] {%s}: Connection established, but no packets yet.\n",
                ImGui::GetTime(), categories[0]);
        }
    }
    else {
        log.AddLog("[DEBUG]: Unknown state!\n");
    }

    log.Draw("PacketTracer: Communication traffic", &state.show_packet_tracer_window);
    ImGui::End();
}


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

//void GuiWindows::ShowPacketTracerWindow(AppState& state) {
//    static PacketTracer log;
//
//    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
//    ImGui::Begin("PacketTracer: Communication traffic", &state.show_packet_tracer_window);
//
//    std::cout << "[DEBUG] COMMUTATOR CONNECTION STATUS: " << state.mux_connection << std::endl;
//
//    const char* categories[3] = { "INFO", "WARNING", "ERROR"};
//    auto packets = state.modbus.getPacketLog();
//
//    if (state.mux_connection == false && packets.empty()) {
//        log.AddLog("[%.2f sec] {%s}: Please build connection with ModbusTCP server.\n", ImGui::GetTime(), categories[2]);
//    }
//    else if (state.mux_connection == true) {
//        log.AddLog("[%s] [%.2f sec] {%s} | %s | %s | %s | %s | ", packets.data()->timestamp.c_str(), 
//            ImGui::GetTime(), categories[0], packets.data()->direction.c_str(), 
//            packets.data()->function.c_str(), packets.data()->details.c_str(), packets.data()->dataHex.c_str());
//    }
//    else {
//        log.AddLog("[DEBUG]: ZALUPA KAKAYA-TO!!!!!!!!!!!!!\n");
//    }
//    log.Draw("PacketTracer: Communication traffic", &state.show_packet_tracer_window);
//    ImGui::End();
//}

#endif
