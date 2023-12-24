#ifndef MAP
#define MAP

//#include <vector>
#include "LecteurNiveau.h"
#include "Objet.h"
#include "Wall.h"

class Map: public Objet{
    public:
        // Constructeur(s)
        Map(char** lvlD, unsigned int largeur, unsigned int Longueur, float* d, unsigned int nTri, std::vector<Wall> lM, Shader* s);
        virtual ~Map();

        // Methode(s)
        std::vector<Wall> getMurs();
        unsigned int getLongueur();
        unsigned int getLargeur();
        char** getLevelData();

    private:
        // Attributs
        std::vector<Wall> lMurs;
        unsigned int L, l, mapTri;
        char** lvlData;
};
#endif
