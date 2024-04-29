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

    std::vector<uint32_t> program = {
        0x00100293,
        0x00200313,
        0x006283b3,
    };

    machine.memory.loadDwords(0x0, program.data(), program.size());
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

        mem_edit.DrawWindow("Memory Editor", (void *)machine.memory.memory, MEMORY_SIZE);

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

        ImGui::Text("PC    : %08X", cpu->pc);
        ImGui::Text("Zero  : %08X", cpu->x0);
        ImGui::Text("RA    : %08X", cpu->x1);
        ImGui::Text("SP    : %08X", cpu->x2);
        ImGui::Text("GP    : %08X", cpu->x3);
        ImGui::Text("TP    : %08X", cpu->x4);
        ImGui::Text("RA    : %08X", cpu->x5);
        ImGui::Text("T0    : %08X", cpu->x6);
        ImGui::Text("T1    : %08X", cpu->x7);
        ImGui::Text("T2    : %08X", cpu->x8);
        ImGui::Text("S0/FP : %08X", cpu->x9);
        ImGui::Text("A0    : %08X", cpu->x10);
        ImGui::Text("A1    : %08X", cpu->x11);
        ImGui::Text("A2    : %08X", cpu->x12);
        ImGui::Text("A3    : %08X", cpu->x13);
        ImGui::Text("A4    : %08X", cpu->x14);
        ImGui::Text("A5    : %08X", cpu->x15);
        ImGui::Text("A6    : %08X", cpu->x16);
        ImGui::Text("A7    : %08X", cpu->x17);
        ImGui::Text("S1    : %08X", cpu->x18);
        ImGui::Text("S2    : %08X", cpu->x19);
        ImGui::Text("S3    : %08X", cpu->x20);
        ImGui::Text("S4    : %08X", cpu->x21);
        ImGui::Text("S5    : %08X", cpu->x22);
        ImGui::Text("S6    : %08X", cpu->x23);
        ImGui::Text("S7    : %08X", cpu->x24);
        ImGui::Text("S8    : %08X", cpu->x25);
        ImGui::Text("S9    : %08X", cpu->x26);
        ImGui::Text("S10   : %08X", cpu->x27);
        ImGui::Text("S11   : %08X", cpu->x28);
        ImGui::Text("T3    : %08X", cpu->x29);
        ImGui::Text("T4    : %08X", cpu->x30);
        ImGui::Text("T5    : %08X", cpu->x31);
        ImGui::Text("T6    : %08X", cpu->x31);

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
