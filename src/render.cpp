#include <sstream>

#include "cpu.hpp"
#include "imgui.h"

void drawRegisters(CPU* cpu) {
    ImGui::InputScalar("PC", ImGuiDataType_U32, &cpu->pc, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("Zero ", ImGuiDataType_U32, &cpu->x0, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_ReadOnly);
    ImGui::InputScalar("RA   ", ImGuiDataType_U32, &cpu->x1, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("SP   ", ImGuiDataType_U32, &cpu->x2, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("GP   ", ImGuiDataType_U32, &cpu->x3, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("TP   ", ImGuiDataType_U32, &cpu->x4, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("RA   ", ImGuiDataType_U32, &cpu->x5, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("T0   ", ImGuiDataType_U32, &cpu->x6, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("T1   ", ImGuiDataType_U32, &cpu->x7, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("T2   ", ImGuiDataType_U32, &cpu->x8, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("S0/FP", ImGuiDataType_U32, &cpu->x9, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("A0   ", ImGuiDataType_U32, &cpu->x10, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("A1   ", ImGuiDataType_U32, &cpu->x11, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("A2   ", ImGuiDataType_U32, &cpu->x12, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("A3   ", ImGuiDataType_U32, &cpu->x13, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("A4   ", ImGuiDataType_U32, &cpu->x14, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("A5   ", ImGuiDataType_U32, &cpu->x15, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("A6   ", ImGuiDataType_U32, &cpu->x16, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("A7   ", ImGuiDataType_U32, &cpu->x17, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("S1   ", ImGuiDataType_U32, &cpu->x18, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("S2   ", ImGuiDataType_U32, &cpu->x19, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("S3   ", ImGuiDataType_U32, &cpu->x20, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("S4   ", ImGuiDataType_U32, &cpu->x21, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("S5   ", ImGuiDataType_U32, &cpu->x22, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("S6   ", ImGuiDataType_U32, &cpu->x23, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("S7   ", ImGuiDataType_U32, &cpu->x24, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("S8   ", ImGuiDataType_U32, &cpu->x25, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("S9   ", ImGuiDataType_U32, &cpu->x26, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("S10  ", ImGuiDataType_U32, &cpu->x27, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("S11  ", ImGuiDataType_U32, &cpu->x28, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("T3   ", ImGuiDataType_U32, &cpu->x29, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("T4   ", ImGuiDataType_U32, &cpu->x30, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("T5   ", ImGuiDataType_U32, &cpu->x31, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("T6   ", ImGuiDataType_U32, &cpu->x31, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
}

void drawLog(bool *showDebug, bool *showInfo, bool *showWarn, bool *showError) {
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
}

