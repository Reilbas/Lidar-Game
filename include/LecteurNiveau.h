#ifndef LECTEURNIVEAU
#define LECTEURNIVEAU

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Camera.h"
#include "Wall.h"

class LecteurNiveau{
    public:
        static bool isEmpty(std::string nomFic);
        static char** retrievingLevel(std::string nomFic, unsigned int &L, unsigned int &l, Camera* c, std::vector<glm::vec2> &objets);
        static float* dataToLevel(char** data, unsigned int L, unsigned int l, unsigned int &nTriangle, std::vector<Wall> &lM, float h);

    private:
        static unsigned int ajouterMur(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, bool* voisins, float h);

        static unsigned int addCouche(std::vector<float> &lFloat, std::vector<Wall> &lM, float sX, float sZ, float h);

        static unsigned int addMurDG(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h);
        static unsigned int addMurBH(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h);

        static unsigned int addCornBD(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h);
        static unsigned int addCornBG(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h);
        static unsigned int addCornHG(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h);
        static unsigned int addCornDH(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h);

        static unsigned int addTriBDG(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h);
        static unsigned int addTriBHG(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h);
        static unsigned int addTriDHG(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h);
        static unsigned int addTriBDH(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h);

        static unsigned int addCroix(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h);
};

#endif