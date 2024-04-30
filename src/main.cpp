#include <inttypes.h>
#include <cstdio>

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
#include "render.cpp"

bool clockEnabled = false;
float clockInterval = 1.0f;
float timeSinceLastStep = 0.0f;

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

    // Initialise the VM
    Machine machine;

    const uint32_t program[] = {
        0x00400293,    // li t0, 4
        0x0012f313,    // li t0, 4
    };

    machine.memory.loadDwords(0x0, (uint32_t*)program, sizeof(program));
    LOG_INFO("Program loaded");

    while (!glfwWindowShouldClose(window) && !globals::shouldExit) {
        glfwPollEvents();

        timeSinceLastStep += io.DeltaTime;

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
        ImGui::Checkbox("Clock enabled", &clockEnabled);

        if (clockEnabled) {
            ImGui::InputFloat("Interval", &clockInterval, 0.1f, 1.0f, "%.1f");
            if (timeSinceLastStep >= clockInterval) {
                machine.step();
                timeSinceLastStep = 0.0f;
            }
        }

        if (ImGui::Button("Step"))
            machine.step();

        ImGui::InputScalar("Address", ImGuiDataType_U32, &addr, 0, 0, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
        ImGui::InputScalar("Value", ImGuiDataType_U8, &value, 0, 0, "%02X", ImGuiInputTextFlags_CharsHexadecimal);

        if (ImGui::Button("Read"))
            LOG_DEBUG("%02X", machine.memory.read8(addr));

        if (ImGui::Button("Write"))
            machine.memory.write8(addr, value);

        ImGui::End();

        ImGui::Begin("Log", nullptr, ImGuiWindowFlags_MenuBar);
        drawLog();
        ImGui::End();

        ImGui::Begin("Registers");
        CPU *cpu = machine.getCPU();
        drawRegisters(cpu);
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

