#include <inttypes.h>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>

#include "globals.cpp"
#include "fonts.hpp"
#include "imgui_internal.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "imgui_memory_editor.h"

#include "machine.cpp"
/* #include "cpu.cpp" */
/* #include "memory.cpp" */

// Function prototypes
void drawLog();
std::vector<uint8_t> loadExeFile(const std::string& filePath);

bool showDebug = true;
bool showInfo  = true;
bool showWarn  = true;
bool showError = true;

static MemoryEditor mem_edit;

uint32_t addr = 0x00000000;
uint8_t value = 0x00;

int main(void) {
    GLFWwindow *window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1280, 720, "TRV Emu", NULL, NULL);

    if (!window) {
        glfwTerminate();
        LOG_ERROR("Failed to create window");
        return -1;
    }

    LOG_INFO("Window created");

    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();

    LOG_INFO("ImGui version: %s", IMGUI_VERSION);
    LOG_INFO("GLFW version: %s", glfwGetVersionString());
    LOG_INFO("OpenGL version: %s", glGetString(GL_VERSION));

#ifdef __linux__
    LOG_INFO("GLSL version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif

    LOG_INFO("Renderer: %s", glGetString(GL_RENDERER));

    ImGui::CreateContext();
    LOG_INFO("ImGui context created");

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImFontConfig config;
	config.FontDataOwnedByAtlas = false;
	io.Fonts->AddFontFromMemoryTTF(fonts::JetBrainsMonoNerdFont_Regular_ttf, sizeof(fonts::JetBrainsMonoNerdFont_Regular_ttf), 16.0f, &config, io.Fonts->GetGlyphRangesDefault());

    if (io.Fonts->Fonts.empty())
		LOG_ERROR("Failed to load font\n");

    LOG_INFO("Font loaded");

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    LOG_INFO("ImGui initialized");

    Machine machine;

    /*
    // Load `.//Debug/TRV-Emu.exe` as a uint8_t array
    #ifdef _WIN32
    std::vector<uint8_t> program = loadExeFile(".\\TRV-Emu.exe");
    #else
    std::vector<uint8_t> program = loadExeFile("./TRV-Emu");
    #endif

    machine.memory.loadBytes(0x0, (uint8_t *)program.data(), program.size());
    */

    const uint32_t program[] = {
        0x00400293,    // li t0, 4
        0x0012f313,    // li t0, 4
    };

    /* std::vector<uint32_t> program = { */
    /*     0x00500293, // addi t0, zero, 5 */
    /*     0x00300313, // addi t1, zero, 3 */
    /*     0x406283b3, // sub t2, t0, t1 */
    /* }; */

    machine.memory.loadDwords(0x0, (uint32_t*)program, sizeof(program));
    /* machine.memory.loadBytes(0x0, (uint8_t *)program.data(), program.size()); */
    /* machine.memory.write32(0x0, 0b00111110100000000000001100010011); // addi t1, zero, 1000 */

    LOG_INFO("Program loaded");

    /* cpu.memory.write32(0xFFFFFFFF, 0x12345678); */
    /* (void)cpu.memory.read32(0xFFFFFFFF); */

    while (!glfwWindowShouldClose(window) && !globals::shouldExit) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create a dockspace
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Quit")) {
                    globals::shouldExit = true;
                    break;
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        mem_edit.DrawWindow("Memory Editor", (void *)machine.memory.getMemory(), MEMORY_BYTES);

        ImGui::Begin("Control");
        if (ImGui::Button("Step"))
            machine.step();

        ImGui::InputScalar("Address", ImGuiDataType_U32, &addr, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
        ImGui::InputScalar("Value", ImGuiDataType_U8, &value, 0, 0, "%02X", ImGuiInputTextFlags_CharsHexadecimal);

        if (ImGui::Button("Read"))
            LOG_DEBUG("%02X", machine.memory.read8(addr));

        if (ImGui::Button("Write"))
            machine.memory.write8(addr, value);

        if (ImGui::Button("Exit"))
            globals::shouldExit = true;

        ImGui::End();

        ImGui::Begin("Log");
        ImGui::Checkbox("Debug", &showDebug);
        ImGui::SameLine();
        ImGui::Checkbox("Info", &showInfo);
        ImGui::SameLine();
        ImGui::Checkbox("Warn", &showWarn);
        ImGui::SameLine();
        ImGui::Checkbox("Error", &showError);
        ImGui::Separator();

        drawLog();
        ImGui::End();

        ImGui::Begin("Registers");
        CPU *cpu = machine.getCPU();

        // Bi-directional input (show value and allow editing)
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

        ImGui::End();

        ImGui::Render();

        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    glfwTerminate();

    LOG_INFO("Window destroyed");

    // cpu.memory.write32(0xFFFFFFFB, 0x12345678);
    // (void)cpu.memory.read32(0xFFFFFFFB);

    return 0;
}

void drawLog() {
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

std::vector<uint8_t> loadExeFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate); // Open file in binary mode and seek to the end
    if (!file) {
        LOG_ERROR("Failed to open file");
        return std::vector<uint8_t>();
    }

    std::streamsize size = file.tellg(); // Get the size of the file
    file.seekg(0, std::ios::beg); // Seek back to the beginning of the file

    std::vector<uint8_t> buffer(size); // Allocate a buffer of the appropriate size
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) { // Read the file into the buffer
        throw std::runtime_error("Failed to read file");
    }

    return buffer; // Return the buffer containing the file data
}
