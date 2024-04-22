#include <inttypes.h>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>

#include "globals.cpp"
#include "fonts.hpp"

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

/*constexpr uint8_t helloWorld[] = {
    0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x57,
    0x6F, 0x72, 0x6C, 0x64, 0x21, 0x00
};*/

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

    // Load `.//Debug/TRV-Emu.exe` as a uint8_t array
    #ifdef _WIN32
    std::vector<uint8_t> program = loadExeFile(".\\TRV-Emu.exe");
    #else
    std::vector<uint8_t> program = loadExeFile("./TRV-Emu");
    #endif

    machine.memory.loadBytes(0x0, (uint8_t *)program.data(), program.size());

    /* cpu.memory.write32(0xFFFFFFFF, 0x12345678); */
    /* (void)cpu.memory.read32(0xFFFFFFFF); */

    while (!glfwWindowShouldClose(window) && !globals::shouldExit) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

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
                ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "%s", to.c_str());
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
