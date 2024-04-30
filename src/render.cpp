#include <sstream>

#include "logging.hpp"
#include "cpu.hpp"
#include "imgui.h"

static bool showDebug = true;
static bool showInfo  = true;
static bool showWarn  = true;
static bool showError = true;

static void drawRegister(uint32_t* reg, const char* name, const char* tooltip, bool readOnly = false) {
    if (readOnly) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::InputScalar(name, ImGuiDataType_U32, reg, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_ReadOnly);
        ImGui::PopStyleColor();
    } else {
        ImGui::InputScalar(name, ImGuiDataType_U32, reg, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    }

    if (tooltip) {
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("%s", tooltip);
            ImGui::EndTooltip();
        }
    }
}

void drawRegisters(CPU* cpu) {
    if (ImGui::Button("Reset")) {
        for (int i = 0; i < 33; i++)
            *cpu->registers[i] = 0;
    }

    drawRegister(&cpu->pc, "PC", "Program counter");

    drawRegister(&cpu->x0, "Zero", "Hard-wired zero", true);
    drawRegister(&cpu->x1, "RA", "Return address");
    drawRegister(&cpu->x2, "SP", "Stack pointer");
    drawRegister(&cpu->x3, "GP", "Global pointer");
    drawRegister(&cpu->x4, "TP", "Thread pointer");
    drawRegister(&cpu->x5, "T0", "Temporary/alternate link register");
    drawRegister(&cpu->x6, "T1", "Temporary");
    drawRegister(&cpu->x7, "T2", "Temporary");
    drawRegister(&cpu->x8, "S0/FP", "Saved register/frame pointer");
    drawRegister(&cpu->x9, "A0", "Function argument/return value");
    drawRegister(&cpu->x10, "A1", "Function argument/return value");
    drawRegister(&cpu->x11, "A2", "Function argument/return value");
    drawRegister(&cpu->x12, "A3", "Function argument/return value");
    drawRegister(&cpu->x13, "A4", "Function argument");
    drawRegister(&cpu->x14, "A5", "Function argument");
    drawRegister(&cpu->x15, "A6", "Function argument");
    drawRegister(&cpu->x16, "A7", "Function argument");
    drawRegister(&cpu->x17, "S1", "Saved register");
    drawRegister(&cpu->x18, "S2", "Saved register");
    drawRegister(&cpu->x19, "S3", "Saved register");
    drawRegister(&cpu->x20, "S4", "Saved register");
    drawRegister(&cpu->x21, "S5", "Saved register");
    drawRegister(&cpu->x22, "S6", "Saved register");
    drawRegister(&cpu->x23, "S7", "Saved register");
    drawRegister(&cpu->x24, "S8", "Saved register");
    drawRegister(&cpu->x25, "S9", "Saved register");
    drawRegister(&cpu->x26, "S10", "Saved register");
    drawRegister(&cpu->x27, "S11", "Saved register");
    drawRegister(&cpu->x28, "T3", "Temporary");
    drawRegister(&cpu->x29, "T4", "Temporary");
    drawRegister(&cpu->x30, "T5", "Temporary");
    drawRegister(&cpu->x31, "T6", "Temporary");
}

void drawLog() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Filter")) {
            ImGui::Checkbox("Debug", &showDebug);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Show debug messages");
            ImGui::Checkbox("Info", &showInfo);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Show info messages");
            ImGui::Checkbox("Warn", &showWarn);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Show warning messages");
            ImGui::Checkbox("Error", &showError);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Show error messages");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options")) {
            ImGui::Checkbox("Log to term", &globals::logToTerm);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Log to the terminal, this can be slow on some platforms");
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    std::stringstream ss(globals::logContent);
    std::string to;

    while (std::getline(ss, to, '\n')) {
        bool isInfo = strstr(to.c_str(), "INFO");
        bool isWarn = strstr(to.c_str(), "WARN");
        bool isError = strstr(to.c_str(), "ERROR");
        bool isDebug = strstr(to.c_str(), "DEBUG");

        bool show = (isInfo && showInfo)
            || (isWarn && showWarn)
            || (isError && showError)
            || (isDebug && showDebug);

        if (show) {
            if (isDebug) {
                ImGui::TextColored(ImVec4(0.25f, 0.25f, 1.0f, 1.0f), "%s", to.c_str());
            } else if (isInfo) {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", to.c_str());
            } else if (isError) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", to.c_str());
            } else if (isWarn) {
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", to.c_str());
            } else {
                ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%s", to.c_str());
            }
        }
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
}

