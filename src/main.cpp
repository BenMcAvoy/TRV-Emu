#include <sstream>
#include <string>
#include <cstdio>

#include "printf_hook.hpp"
#include "globals.cpp"

#ifdef _WIN32
#include <windows.h>
#endif

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "cpu.cpp"
#include "memory.cpp"

void drawLog();

bool showDebug = false;
bool showInfo = false;
bool showWarn = true;
bool showError = true;

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

    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    LOG_INFO("ImGui initialized");

    CPU cpu;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        // Draw the log window
        ImGui::Begin("TRV Emu Log");
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
        bool show = showInfo && strstr(to.c_str(), "INFO")
            || showWarn && strstr(to.c_str(), "WARN")
            || showError && strstr(to.c_str(), "ERROR")
            || showDebug && strstr(to.c_str(), "DEBUG");

        if (show)
            ImGui::Text("%s", to.c_str());
    }
}