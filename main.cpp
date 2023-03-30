#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include<shader.h>

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include "imnodes.h"
#include <graph.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <functional>
static std::function<void()>            MainLoopForEmscriptenP;
static void MainLoopForEmscripten()     { MainLoopForEmscriptenP(); }
#define EMSCRIPTEN_MAINLOOP_BEGIN       MainLoopForEmscriptenP = [&]()
#define EMSCRIPTEN_MAINLOOP_END         ; emscripten_set_main_loop(MainLoopForEmscripten, 0, true)
#else
#define EMSCRIPTEN_MAINLOOP_BEGIN
#define EMSCRIPTEN_MAINLOOP_END
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;


int main(int, char**)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImNodes::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    Graph graph;

    int id = 0;

    graph.addNode(100, 1);
    graph.addNode(101, 2);
    graph.addNode(102, 1);
    graph.addNode(103, 1);

    graph.addInputAttribute(20, 100, 1);
    graph.addOutputAttribute(21, 100, 1);

    graph.addInputAttribute(22, 101, 1);
    graph.addOutputAttribute(23, 101, 1);

    graph.addInputAttribute(24, 102, 1);
    graph.addOutputAttribute(25, 102, 1);

    graph.addInputAttribute(26, 103, 1);
    graph.addOutputAttribute(27, 103, 1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float vertices[] = {
         1.0f,  1.0f,  // top right
         1.0f, -1.0f,  // bottom right
        -1.0f, -1.0f,  // bottom left
        -1.0f,  1.0f,  // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader metaballShader("metaball_shader.vs", "metaball_shader.fs");
    Shader metaballShader3d("metaball_shader_3d.vs", "metaball_shader_3d.fs");

    float speed = 1.0f;
    float s1 = 0.2f;
    float s2 = 0.2f;
    float s3 = 0.2f;
    float s4 = 0.2f;
    float distortion = 1.0f;
    bool use3d = false;
    bool showEditor = false;

    #ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = NULL;
    EMSCRIPTEN_MAINLOOP_BEGIN
    #else
        while (!glfwWindowShouldClose(window))
    #endif
    {
        processInput(window);

        if(!use3d){
            metaballShader.use();
            metaballShader.setFloat("time", glfwGetTime());
            metaballShader.setFloat("speed", speed);
            metaballShader.setFloat("s1", s1);
            metaballShader.setFloat("s2", s2);
            metaballShader.setFloat("s3", s3);
            metaballShader.setFloat("s4", s4);
            metaballShader.setFloat("distortion", distortion);
            metaballShader.setFloat("aspectRatio", (float)SCR_HEIGHT/(float)SCR_WIDTH);
        }
        else{
            metaballShader3d.use();
            metaballShader3d.setFloat("aspectRatio", (float)SCR_HEIGHT/(float)SCR_WIDTH);
            metaballShader3d.setFloat("time", glfwGetTime());
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings");
        //ImGui::SliderFloat("speed", &speed, -10.0f, 10.0f);
        ImGui::Checkbox("3d", &use3d);
        ImGui::Checkbox("Editor", &showEditor);
        ImGui::SliderFloat("s1", &s1, 0.0f, 1.0f);
        ImGui::SliderFloat("s2", &s2, 0.0f, 1.0f);
        ImGui::SliderFloat("s3", &s3, 0.0f, 1.0f);
        ImGui::SliderFloat("s4", &s4, 0.0f, 1.0f);
        ImGui::SliderFloat("distortion", &distortion, 0.0, 1000.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        ImGui::End();

        ImGui::Begin("Editor");
            ImNodes::BeginNodeEditor();
                graph.draw();
            ImNodes::EndNodeEditor();

            int start_attr, end_attr;
            if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
            {
                std::cout << start_attr << " " << end_attr << "\n";
                graph.connect(start_attr, end_attr, start_attr+end_attr);
            }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    #ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
    #endif

    ImNodes::DestroyContext();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_HEIGHT = height;
    SCR_WIDTH = width;
}