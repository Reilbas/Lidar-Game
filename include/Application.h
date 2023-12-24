#ifndef APP
#define APP
#include <iostream>
#include <cmath>
#include <cstdlib>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <fstream>

#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "stb_image.h"

#include "LecteurNiveau.h"
#include "MeshMaker.h"
#include "Shader.h"
#include "Camera.h"
#include "Objet.h"
#include "Map.h"
#include "Item.h"
#include "Wall.h"
#include "Config.h"
#include "ListeGraf.h"

// singleton ?
class Application{
    public:
        Application();
        ~Application();
        void run();

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        void processInput(GLFWwindow *window);
        void UI(GLFWwindow *window, const ImGuiViewport* viewport, ImGuiWindowFlags flags);
        void inventory(GLFWwindow *window, const ImGuiViewport* viewport, ImGuiWindowFlags flags);
        void menu(GLFWwindow *window, const ImGuiViewport* viewport, ImGuiWindowFlags flags);
        void hud(GLFWwindow *window, const ImGuiViewport* viewport, ImGuiWindowFlags flags);
        void victory(GLFWwindow *window, const ImGuiViewport* viewport, ImGuiWindowFlags flags);

        bool loadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
        void addGraf(float x, float y);
        void drawRectangle();
        char* loadFile(char const* path);
        bool initialized();

    private:
        // Attributs
        bool init;
        GLFWwindow* window;
        Shader* shader;
        Camera* camera;

        //map
        Map* map;
        Item* gun;
        Objet* obj;
        Objet* graffiti;
        std::vector<glm::vec2> objets;

        FILE* file;
        char* buffer_map;

        liste lGraf;
        liste premier;
        unsigned int lGrafSize;
        
        // Methodes Privees
        bool loadMap(std::string nomFic);
};
#endif