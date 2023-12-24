#include "Map.h"

Map::Map(char** lvlD, unsigned int largeur, unsigned int Longueur, float* d, unsigned int nTri, std::vector<Wall> lM, Shader* s) : Objet(d, nTri, s){
	lvlData = lvlD;
    l = largeur;
    L = Longueur;
    lMurs = lM;
}

Map::~Map(){
    free(lvlData);
}

std::vector<Wall> Map::getMurs(){
    return lMurs;
}

unsigned int Map::getLongueur(){
    return L;
}

unsigned int Map::getLargeur(){
    return l;
}

char** Map::getLevelData(){
    return lvlData;
}