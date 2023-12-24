#ifndef ITEM
#define ITEM

//#include <vector>
#include "LecteurNiveau.h"
#include "Objet.h"

class Item: public Objet{
    public:
        // Constructeur(s)
        Item(float* d, unsigned int nTri, Shader* s);
        virtual ~Item();

        // Methode(s)
        virtual glm::mat4 getModel() override;
        void setControleur(Camera* c);
        //void estDeposer() <- Controleur == nullptr et deposer sur une case
    private:
        // Attributs
        Camera* controleur;
};
#endif
