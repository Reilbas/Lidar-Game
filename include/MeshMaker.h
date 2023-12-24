#ifndef MESHMAKER
#define MESHMAKER

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

struct vec3d {
	float x = 0;
    float y = 0;
    float z = 0;
};

struct triangle {
	vec3d p[3];
};

class MeshMaker {
    // Constructeur et Méthodes
    public:
        static float* fromObj(std::string nomfic,float x, float y, float z, float size, unsigned int &nTriangle);
};

#endif
