#define DEBUG_TEST
//#define RELEASE_TEST
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

            if (packets.empty()) {
                ImGui::Text("No Packets logged yet ...");
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

/////////////////////////////////////////// EXAMPLE LOGGING /////////////////////////////////////////////
struct PacketTracer
{
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool                AutoScroll;  // Keep scrolling if already at the bottom.

    PacketTracer()
    {
        AutoScroll = true;
        Clear();
    }

    void    Clear()
    {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }

    void    Draw(const char* title, bool* p_open = NULL)
    {
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Main window
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool clear = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        Filter.Draw("Filter", -100.0f);

        ImGui::Separator();

        if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (clear)
                Clear();
            if (copy)
                ImGui::LogToClipboard();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            const char* buf = Buf.begin();
            const char* buf_end = Buf.end();
            if (Filter.IsActive())
            {
                // In this example we don't use the clipper when Filter is enabled.
                // This is because we don't have random access to the result of our filter.
                // A real application processing logs with ten of thousands of entries may want to store the result of
                // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
                for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    if (Filter.PassFilter(line_start, line_end))
                        ImGui::TextUnformatted(line_start, line_end);
                }
            }
            else
            {
                // The simplest and easy way to display the entire buffer:
                //   ImGui::TextUnformatted(buf_begin, buf_end);
                // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
                // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
                // within the visible area.
                // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
                // on your side is recommended. Using ImGuiListClipper requires
                // - A) random access into your data
                // - B) items all being the  same height,
                // both of which we can handle since we have an array pointing to the beginning of each line of text.
                // When using the filter (in the block of code above) we don't have random access into the data to display
                // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
                // it possible (and would be recommended if you want to search through tens of thousands of entries).
                ImGuiListClipper clipper;
                clipper.Begin(LineOffsets.Size);
                while (clipper.Step())
                {
                    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                    {
                        const char* line_start = buf + LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                        ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                clipper.End();
            }
            ImGui::PopStyleVar();

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
            // Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();
        ImGui::End();
    }
};

void GuiWindows::ShowPacketTracerWindow(bool* p_open)
{
    static PacketTracer log;

    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Packet Tracer: Log", p_open);
    if (ImGui::SmallButton("[Debug] Add 5 entries"))
    {
        static int counter = 0;
        const char* categories[3] = { "info", "warn", "error" };
        const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
        for (int n = 0; n < 5; n++)
        {
            const char* category = categories[counter % IM_ARRAYSIZE(categories)];
            const char* word = words[counter % IM_ARRAYSIZE(words)];
            log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
                ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
            counter++;
        }
    }
    ImGui::End();

    log.Draw("Packet Tracer: Log", p_open);
}

//void GuiWindows::ShowPacketTracerWindow(bool* p_open)
//{
//    static PacketTracer log;
//
//    // Устанавливаем размер окна
//    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
//    ImGui::Begin("Packet Tracer: Log", p_open);
//
//    // Выводим пакеты
//    const auto& packets = log.modbus.getPacketLog(); // Предполагая, что вы можете получить пакеты от log
//    ImGui::Separator();
//    ImGui::Text("Packet Log:");
//
//    if (ImGui::SmallButton("[Debug] Add 5 entries")) {
//        for (const auto& packet : packets) {
//            ImGui::Text("Time: %s, Direction: %s, Function: %s, Details: %s, Data: %s",
//                packet.timestamp.c_str(),
//                packet.direction.c_str(),
//                packet.function.c_str(),
//                packet.details.c_str(),
//                packet.dataHex.c_str());
//        }
//    }
//
//    ImGui::End();
//
//    // Вызываем обычный лог
//    log.Draw("Packet Tracer: Log", p_open);
//}
