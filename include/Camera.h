#ifndef CAMERA
#define CAMERA

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Config.h"

class Camera{
    public:
        // Constructeurs
        explicit Camera(GLFWwindow* window, Shader* s, float x, float y, float z);

        // Methodes
        void setActiveShader(Shader* s);
        static void mouse_callback(GLFWwindow* w, double xpos, double ypos);
        void handleCameraEvent();

        void deplacement(float d, char** map, unsigned int nX, unsigned nZ);
        void setLocation(float x, float y, float z);
        
        float* getSpeedPointer();
        float* getFOVPointer();
        void applyChanges();
        
        glm::mat4 getView();
        glm::vec3 getCameraFront();
        glm::vec3 getCameraPos();
        float getX();
        float getY();
        float getZ();
        int getMoney();
        void addMoney(int m);
        

    private:
        // Attributs
        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        Shader* shader;

        static GLFWwindow* window;
        
        static glm::vec3 cameraUp;
        static glm::mat4 projection;
        
        static float lastX, lastY;
        static float yaw, pitch, vitesse, fov;
        static bool firstMouse;

        int money;

        // Methodes privés
        void checkLocation(glm::vec3 newLocation, char** map, unsigned int nX, unsigned nZ);
};
#endif