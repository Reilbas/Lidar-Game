#ifndef OBJET
#define OBJET

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Shader.h"
#include "ListeGraf.h"

class Objet{
    public:
        // Constructeur(s)
        Objet(float* d, unsigned int nTri, Shader* s);
        virtual ~Objet();

        // Methode(s) 
        void draw();
        void drawAt(std::vector<glm::vec2> lObjet);
        void drawAt(liste L);
        void setCoordinate(float x, float y, float z);
        void translate(float x, float y, float z);
        void rotate(float x, float y, float z);
        void setShader(Shader* s);
        virtual glm::mat4 getModel();

    protected:
        float posX, posY, posZ, rX, rY, rZ;
        unsigned int VAO, VBO, nTriangle;
        float* data;
        Shader* shader;
};
#endif
