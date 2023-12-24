#ifndef WALL_H
#define WALL_H

#include <glm/glm.hpp>

struct Wall{
    glm::vec3 BL;
    glm::vec3 UR;

    glm::vec3 UL;
    glm::vec3 BR;
};

#endif