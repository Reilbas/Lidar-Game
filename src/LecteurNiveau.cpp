#include "LecteurNiveau.h"

bool LecteurNiveau::isEmpty(std::string nomFic){
    std::ifstream fic (nomFic);
    return fic.peek() == std::ifstream::traits_type::eof();
}

char** LecteurNiveau::retrievingLevel(std::string nomFic, unsigned int &L, unsigned int &l, Camera* c, std::vector<glm::vec2> &objets){
    std::vector<std::string> lstr;
    std::string line;
    std::ifstream fic (nomFic);
    bool joueurPlacer = false;
    bool caseSecoursTrouver = false;
    unsigned int caseSecoursI = 0;
    unsigned int caseSecoursJ = 0;

    unsigned int taille_max = 0;
    if (fic.is_open()){
        while(getline(fic,line)){
            if(taille_max < (unsigned int)line.size()){
               taille_max = line.size();
            }
            lstr.push_back(line);
        }
        fic.close();
        objets.clear();
        l = lstr.size();
        L = taille_max;
        char** env = (char**) malloc(lstr.size() * sizeof(char*));
        for (unsigned int i = 0 ; i < l ; i++){
            env[i] = (char*) malloc(L * sizeof(char));
            for(unsigned int j=0 ; j < lstr[i].size() ; j++){
                switch (lstr[i][j]){
                    case 'J':
                        c->setLocation(i+0.5f, 1.0f, j+0.5f);
                        joueurPlacer = true;
                        env[i][j] = 'O';
                        break;
                    case 'I':
                        objets.push_back(glm::vec2(i, j));
                        env[i][j] = 'I';
                        break;
                    case 'O':
                        if(!caseSecoursTrouver){
                            caseSecoursTrouver = true;
                            caseSecoursI = i;
                            caseSecoursJ = j;
                        }
                        env[i][j] = 'O';
                        break;
                    default:
                        env[i][j] = lstr[i][j];
                        break;
                }
            }
            for(unsigned int j=lstr[i].size() ; j < L ; j++){
                if(!caseSecoursTrouver){
                    caseSecoursTrouver = true;
                    caseSecoursI = i;
                    caseSecoursJ = j;
                }
                env[i][j] = 'O';
            }
        }
        if(!joueurPlacer){
            if(caseSecoursTrouver){
                c->setLocation(caseSecoursI+0.5f, 1.0f, caseSecoursJ+0.5f);
                std::cout << "Le joueur a ete placer sur une case disponible" << std::endl;
            } else {
                std::cout << "Le joueur n'a pas pu etre placer" << std::endl;
                return nullptr;
            }
        }
        return env;
    } else { 
        std::cout << "Le niveau n'a pas pu etre ouvert" << std::endl;
        return nullptr;
    }
};

float* LecteurNiveau::dataToLevel(char** data, unsigned int L, unsigned int l, unsigned int &nTriangle, std::vector<Wall> &lM, float h){
    std::vector<float> lFloat;
    nTriangle = 0;
    nTriangle += LecteurNiveau::addCouche(lFloat, lM, l, L, 0.0f);  //sol
    nTriangle += LecteurNiveau::addCouche(lFloat, lM, l, L, h);     //plafond
    for (unsigned int i = 0 ; i < l ; i++){
        for(unsigned int j = 0 ; j < L ; j++){
            switch (data[i][j]){
                case 'X':
                    {
                        bool voisins[4] = {false,false,false,false}; // Bas, Droite, Haut, Gauche
                        if(i!=0){
                            if(data[i-1][j]=='X'){
                                voisins[3] = true;
                            }
                        }
                        if(j!=0){
                            if(data[i][j-1]=='X'){
                                voisins[2] = true;
                            }
                        }
                        if(i!=(l-1)){
                            if(data[i+1][j]=='X'){
                                voisins[1] = true;
                            }
                        }
                        if(j!=(L-1)){
                            if(data[i][j+1]=='X'){
                                voisins[0] = true;
                            }
                        }
                        nTriangle += LecteurNiveau::ajouterMur(lFloat, lM, i, j, voisins, h);
                    }
                    break;
                case 'O':
                default:
                    break;
            }
        }
    }
    // conversion en array de float
    float* tabFloat = (float*) malloc(sizeof(float)*lFloat.size());
    for(unsigned int i = 0; i < (unsigned int)lFloat.size(); i++){
        tabFloat[i] = lFloat.at(i);
    }
    return tabFloat;
}

unsigned int LecteurNiveau::ajouterMur(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, bool* voisins, float h){
    // Bas, Droite, Haut, Gauche

    // 0 0 0 0 | Rien (ou Pillone)
    if(!voisins[0] && !voisins[1] && !voisins[2] && !voisins[3]){
        return 0;
    }

    // 0 0 0 1 | Rien (ou Pillone vers Gauche)
    if(!voisins[0] && !voisins[1] && !voisins[2] && voisins[3]){
        return LecteurNiveau::addMurDG(lFloat, lM, x, z, h);
    }

    // 0 0 1 0 | Rien (ou Pillone vers Haut)
    if(!voisins[0] && !voisins[1] && voisins[2] && !voisins[3]){
        return LecteurNiveau::addMurBH(lFloat, lM, x, z, h);
    }

    // 0 0 1 1 | Coin Haut Gauche
    if(!voisins[0] && !voisins[1] && voisins[2] && voisins[3]){
        return LecteurNiveau::addCornHG(lFloat, lM, x, z, h);
    }

    // 0 1 0 0 | Rien (ou Pillone vers Droite)
        if(!voisins[0] && voisins[1] && !voisins[2] && !voisins[3]){
        return LecteurNiveau::addMurDG(lFloat, lM, x, z, h);
    }

    // 0 1 0 1 | Mur Droite Gauche
    if(!voisins[0] && voisins[1] && !voisins[2] && voisins[3]){
        return LecteurNiveau::addMurDG(lFloat, lM, x, z, h);
    }

    // 0 1 1 0 | Coin Droite Haut
    if(!voisins[0] && voisins[1] && voisins[2] && !voisins[3]){
        return LecteurNiveau::addCornDH(lFloat, lM, x, z, h);
    }

    // 0 1 1 1 | Tri Droite Haut Gauche
    if(!voisins[0] && voisins[1] && voisins[2] && voisins[3]){
        return LecteurNiveau::addTriDHG(lFloat, lM, x, z, h);
    }

    // 1 0 0 0 | Rien (ou Pillone vers Bas)
    if(voisins[0] && !voisins[1] && !voisins[2] && !voisins[3]){
        return LecteurNiveau::addMurBH(lFloat, lM, x, z, h);
    }
    
    // 1 0 0 1 | Coin Bas Gauche
    if(voisins[0] && !voisins[1] && !voisins[2] && voisins[3]){
        return LecteurNiveau::addCornBG(lFloat, lM, x, z, h);
    }

    // 1 0 1 0 | Mur Bas Haut
    if(voisins[0] && !voisins[1] && voisins[2] && !voisins[3]){
        return LecteurNiveau::addMurBH(lFloat, lM, x, z, h);
    }

    // 1 0 1 1 | Tri Bas Haut Gauche
    if(voisins[0] && !voisins[1] && voisins[2] && voisins[3]){
        return LecteurNiveau::addTriBHG(lFloat, lM, x, z, h);
    }

    // 1 1 0 0 | Coin Bas Droite
    if(voisins[0] && voisins[1] && !voisins[2] && !voisins[3]){
        return LecteurNiveau::addCornBD(lFloat, lM, x, z, h);
    }

    // 1 1 0 1 | Tri Bas Droite Gauche
    if(voisins[0] && voisins[1] && !voisins[2] && voisins[3]){
        return LecteurNiveau::addTriBDG(lFloat, lM, x, z, h);
    }

    // 1 1 1 0 | Tri Bas Droite Haut
    if(voisins[0] && voisins[1] && voisins[2] && !voisins[3]){
        return LecteurNiveau::addTriBDH(lFloat, lM, x, z, h);
    }

    // 1 1 1 1 | Croix
    if(voisins[0] && voisins[1] && voisins[2] && voisins[3]){
        return LecteurNiveau::addCroix(lFloat, lM, x, z, h);
    }
    return 0;
}

unsigned int LecteurNiveau::addCouche(std::vector<float> &lFloat, std::vector<Wall> &lM, float sX, float sZ, float h){
    lFloat.insert(lFloat.end(),
    {
		0.0f, h, 0.0f,
		1.0f*sX, h, 0.0f,//-
		1.0f*sX, h, 1.0f*sZ,

		0.0f, h, 0.0f,
		1.0f*sX, h, 1.0f*sZ,
		0.0f, h, 1.0f*sZ//-
	});
    lM.insert(lM.end(), {
        {0.0f, h, 0.0f},
        {1.0f*sX, h, 1.0f*sZ},
        {1.0f*sX, h, 0.0f},
        {0.0f, h, 1.0f*sZ},
    });
    return 6;
};

unsigned int LecteurNiveau::addMurDG(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h){
    lFloat.insert(lFloat.end(),
    {
		0.0f+x, 0.0f, 0.5f+z,
		1.0f+x, 0.0f, 0.5f+z,//-
		1.0f+x, h, 0.5f+z,

		0.0f+x, 0.0f, 0.5f+z,
		1.0f+x, h, 0.5f+z,
		0.0f+x, h, 0.5f+z//-
	});
    lM.insert(lM.end(), {
        {0.0f+x, 0.0f, 0.5f+z},
        {1.0f+x, h, 0.5f+z},
        {1.0f+x, 0.0f, 0.5f+z},
        {0.0f+x, h, 0.5f+z},
    });
    return 6;
};

unsigned int LecteurNiveau::addMurBH(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h){
    lFloat.insert(lFloat.end(),
    {
		0.5f+x, 0.0f, 0.0f+z,
		0.5f+x, 0.0f, 1.0f+z,//-
		0.5f+x, h, 1.0f+z,

		0.5f+x, 0.0f, 0.0f+z,
		0.5f+x, h, 1.0f+z,
		0.5f+x, h, 0.0f+z//-
	});
    lM.insert(lM.end(), {
        {0.5f+x, 0.0f, 0.0f+z},
        {0.5f+x, h, 1.0f+z},
        {0.5f+x, 0.0f, 1.0f+z},
        {0.5f+x, h, 0.0f+z},
    });
    return 6;
};

unsigned int LecteurNiveau::addCornBD(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h){
    lFloat.insert(lFloat.end(),
    {
		0.5f+x, h, 0.5f+z,
		1.0f+x, h, 0.5f+z,//-
		1.0f+x, 0.0f, 0.5f+z,

		0.5f+x, h, 0.5f+z,
		1.0f+x, 0.0f, 0.5f+z,
		0.5f+x, 0.0f, 0.5f+z,//-

		0.5f+x, h, 1.0f+z,
		0.5f+x, h, 0.5f+z,//-
		0.5f+x, 0.0f, 0.5f+z,

		0.5f+x, h, 1.0f+z,
		0.5f+x, 0.0f, 0.5f+z,
		0.5f+x, 0.0f, 1.0f+z //-
	});
    lM.insert(lM.end(), {
        {0.5f+x, h, 0.5f+z},
        {1.0f+x, 0.0f, 0.5f+z},
        {1.0f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 0.5f+z},
    });
    lM.insert(lM.end(), {
        {0.5f+x, h, 1.0f+z},
        {0.5f+x, 0.0f, 0.5f+z},
        {0.5f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 1.0f+z},
    });
    return 12;
};

unsigned int LecteurNiveau::addCornBG(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h){
    lFloat.insert(lFloat.end(),
    {
		0.0f+x, h, 0.5f+z,
		0.5f+x, h, 0.5f+z,//-
		0.5f+x, 0.0f, 0.5f+z,

		0.0f+x, h, 0.5f+z,
		0.5f+x, 0.0f, 0.5f+z,
		0.0f+x, 0.0f, 0.5f+z,//-

		0.5f+x, h, 0.5f+z,
		0.5f+x, h, 1.0f+z,//-
		0.5f+x, 0.0f, 1.0f+z,

		0.5f+x, h, 0.5f+z,
		0.5f+x, 0.0f, 1.0f+z,
		0.5f+x, 0.0f, 0.5f+z,//-
	});
    lM.insert(lM.end(), {
        {0.0f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 0.5f+z},
        {0.5f+x, h, 0.5f+z},
        {0.0f+x, 0.0f, 0.5f+z},
    });
    lM.insert(lM.end(), {
        {0.5f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 1.0f+z},
        {0.5f+x, h, 1.0f+z},
        {0.5f+x, 0.0f, 0.5f+z},
    });
    return 12;
};

unsigned int LecteurNiveau::addCornHG(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h){
    lFloat.insert(lFloat.end(),
    {
		0.5f+x, h, 0.0f+z,
		0.5f+x, h, 0.5f+z,//-
		0.5f+x, 0.0f, 0.5f+z,

		0.5f+x, h, 0.0f+z,
		0.5f+x, 0.0f, 0.5f+z,
		0.5f+x, 0.0f, 0.0f+z,//-

		0.5f+x, h, 0.5f+z,
		0.0f+x, h, 0.5f+z,//-
		0.0f+x, 0.0f, 0.5f+z,

		0.5f+x, h, 0.5f+z,
		0.0f+x, 0.0f, 0.5f+z,
		0.5f+x, 0.0f, 0.5f+z,//-
	});
    lM.insert(lM.end(), {
        {0.5f+x, h, 0.0f+z},
        {0.5f+x, 0.0f, 0.5f+z},
        {0.5f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 0.0f+z},
    });
    lM.insert(lM.end(), {
        {0.5f+x, h, 0.5f+z},
        {0.0f+x, 0.0f, 0.5f+z},
        {0.0f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 0.5f+z},
    });
    return 12;
};

unsigned int LecteurNiveau::addCornDH(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h){
    lFloat.insert(lFloat.end(),
    {
		1.0f+x, h, 0.5f+z,
		0.5f+x, h, 0.5f+z,//-
		0.5f+x, 0.0f, 0.5f+z,

		1.0f+x, h, 0.5f+z,
		0.5f+x, 0.0f, 0.5f+z,
		1.0f+x, 0.0f, 0.5f+z,//-

		0.5f+x, h, 0.5f+z,
		0.5f+x, h, 0.0f+z,//-
		0.5f+x, 0.0f, 0.0f+z,

		0.5f+x, h, 0.5f+z,
		0.5f+x, 0.0f, 0.0f+z,
		0.5f+x, 0.0f, 0.5f+z,//-
	});
    lM.insert(lM.end(), {
        {1.0f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 0.5f+z},
        {0.5f+x, h, 0.5f+z},
        {1.0f+x, 0.0f, 0.5f+z},
    });
    lM.insert(lM.end(), {
        {0.5f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 0.0f+z},
        {0.5f+x, h, 0.0f+z},
        {0.5f+x, 0.0f, 0.5f+z},
    });
    return 12;
};

unsigned int LecteurNiveau::addTriBDG(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h){
    lFloat.insert(lFloat.end(),
    {
		0.0f+x, 0.0f, 0.5f+z,
		1.0f+x, 0.0f, 0.5f+z,//-
		1.0f+x, h, 0.5f+z,

		0.0f+x, 0.0f, 0.5f+z,
		1.0f+x, h, 0.5f+z,
		0.0f+x, h, 0.5f+z,//-

		0.5f+x, h, 0.5f+z,
		0.5f+x, h, 1.0f+z,//-
		0.5f+x, 0.0f, 1.0f+z,

		0.5f+x, h, 0.5f+z,
		0.5f+x, 0.0f, 1.0f+z,
		0.5f+x, 0.0f, 0.5f+z,//-
	});
    lM.insert(lM.end(), {
        {0.0f+x, 0.0f, 0.5f+z},
        {1.0f+x, h, 0.5f+z},
        {1.0f+x, 0.0f, 0.5f+z},
        {0.0f+x, h, 0.5f+z},
    });
    lM.insert(lM.end(), {
        {0.5f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 1.0f+z},
        {0.5f+x, h, 1.0f+z},
        {0.5f+x, 0.0f, 0.5f+z},
    });
    return 12;
};

unsigned int LecteurNiveau::addTriBHG(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h){
    lFloat.insert(lFloat.end(),
    {
		0.5f+x, 0.0f, 0.0f+z,
		0.5f+x, 0.0f, 1.0f+z,//-
		0.5f+x, h, 1.0f+z,

		0.5f+x, 0.0f, 0.0f+z,
		0.5f+x, h, 1.0f+z,
		0.5f+x, h, 0.0f+z,//-

		0.0f+x, h, 0.5f+z,
		0.5f+x, h, 0.5f+z,//-
		0.5f+x, 0.0f, 0.5f+z,

		0.0f+x, h, 0.5f+z,
		0.5f+x, 0.0f, 0.5f+z,
		0.0f+x, 0.0f, 0.5f+z,//-
	});
    lM.insert(lM.end(), {
        {0.5f+x, 0.0f, 0.0f+z},
        {0.5f+x, h, 1.0f+z},
        {0.5f+x, 0.0f, 1.0f+z},
        {0.5f+x, h, 0.0f+z},
    });
    lM.insert(lM.end(), {
        {0.0f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 0.5f+z},
        {0.5f+x, h, 0.5f+z},
        {0.0f+x, 0.0f, 0.5f+z},
    });
    return 12;
};

unsigned int LecteurNiveau::addTriDHG(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h){
    lFloat.insert(lFloat.end(),
    {
		0.0f+x, 0.0f, 0.5f+z,
		1.0f+x, 0.0f, 0.5f+z,//-
		1.0f+x, h, 0.5f+z,

		0.0f+x, 0.0f, 0.5f+z,
		1.0f+x, h, 0.5f+z,
		0.0f+x, h, 0.5f+z,//-

		0.5f+x, h, 0.5f+z,
		0.5f+x, h, 0.0f+z,//-
		0.5f+x, 0.0f, 0.0f+z,

		0.5f+x, h, 0.5f+z,
		0.5f+x, 0.0f, 0.0f+z,
		0.5f+x, 0.0f, 0.5f+z,//-
	});
    lM.insert(lM.end(), {
        {0.0f+x, 0.0f, 0.5f+z},
        {1.0f+x, h, 0.5f+z},
        {1.0f+x, 0.0f, 0.5f+z},
        {0.0f+x, h, 0.5f+z},
    });
    lM.insert(lM.end(), {
        {0.5f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 0.0f+z},
        {0.5f+x, h, 0.0f+z},
        {0.5f+x, 0.0f, 0.5f+z},
    });
    return 12;
};

unsigned int LecteurNiveau::addTriBDH(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h){
    lFloat.insert(lFloat.end(),
    {
		0.5f+x, 0.0f, 0.0f+z,
		0.5f+x, 0.0f, 1.0f+z,//-
		0.5f+x, h, 1.0f+z,

		0.5f+x, 0.0f, 0.0f+z,
		0.5f+x, h, 1.0f+z,
		0.5f+x, h, 0.0f+z,//-

		1.0f+x, h, 0.5f+z,
		0.5f+x, h, 0.5f+z,//-
		0.5f+x, 0.0f, 0.5f+z,

		1.0f+x, h, 0.5f+z,
		0.5f+x, 0.0f, 0.5f+z,
		1.0f+x, 0.0f, 0.5f+z,//-
	});
    lM.insert(lM.end(), {
        {0.5f+x, 0.0f, 0.0f+z},
        {0.5f+x, h, 1.0f+z},
        {0.5f+x, 0.0f, 1.0f+z},
        {0.5f+x, h, 0.0f+z},
    });
    lM.insert(lM.end(), {
        {1.0f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 0.5f+z},
        {0.5f+x, h, 0.5f+z},
        {1.0f+x, 0.0f, 0.5f+z},
    });
    return 12;
};

unsigned int LecteurNiveau::addCroix(std::vector<float> &lFloat, std::vector<Wall> &lM, float x, float z, float h){
    lFloat.insert(lFloat.end(),
    {
		0.5f+x, h, 0.5f+z,
		1.0f+x, h, 0.5f+z,//-
		1.0f+x, 0.0f, 0.5f+z,

		0.5f+x, h, 0.5f+z,
		1.0f+x, 0.0f, 0.5f+z,
		0.5f+x, 0.0f, 0.5f+z,//-

		0.5f+x, h, 1.0f+z,
		0.5f+x, h, 0.5f+z,//-
		0.5f+x, 0.0f, 0.5f+z,

		0.5f+x, h, 1.0f+z,
		0.5f+x, 0.0f, 0.5f+z,
		0.5f+x, 0.0f, 1.0f+z,//-

		0.5f+x, h, 0.0f+z,
		0.5f+x, h, 0.5f+z,//-
		0.5f+x, 0.0f, 0.5f+z,

		0.5f+x, h, 0.0f+z,
		0.5f+x, 0.0f, 0.5f+z,
		0.5f+x, 0.0f, 0.0f+z,//-

		0.5f+x, h, 0.5f+z,
		0.0f+x, h, 0.5f+z,//-
		0.0f+x, 0.0f, 0.5f+z,

		0.5f+x, h, 0.5f+z,
		0.0f+x, 0.0f, 0.5f+z,
		0.5f+x, 0.0f, 0.5f+z,//-
	});
    lM.insert(lM.end(), {
        {0.5f+x, h, 0.5f+z},
        {1.0f+x, 0.0f, 0.5f+z},
        {1.0f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 0.5f+z},
    });
    lM.insert(lM.end(), {
        {0.5f+x, h, 1.0f+z},
        {0.5f+x, 0.0f, 0.5f+z},
        {0.5f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 1.0f+z},
    });
    lM.insert(lM.end(), {
        {0.5f+x, h, 0.0f+z},
        {0.5f+x, 0.0f, 0.5f+z},
        {0.5f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 0.0f+z},
    });
    lM.insert(lM.end(), {
        {0.5f+x, h, 0.5f+z},
        {0.0f+x, 0.0f, 0.5f+z},
        {0.0f+x, h, 0.5f+z},
        {0.5f+x, 0.0f, 0.5f+z},
    });
    return 24;
};
