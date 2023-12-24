#include "Item.h"

Item::Item(float* d, unsigned int nTri, Shader* s) : Objet(d, nTri, s){
    controleur = nullptr;
}

Item::~Item(){}

glm::mat4 Item::getModel(){
    glm::mat4 model = glm::mat4(1.0f);
    if(controleur != nullptr){
        model = glm::translate(model, glm::vec3(controleur->getX() + controleur->getCameraFront().x * 3, controleur->getY() + controleur->getCameraFront().y * 3, controleur->getZ() + controleur->getCameraFront().z * 3));
        //glm::mat4 model = model * controleur->getView();
    } else {
        model = glm::translate(model, glm::vec3(posX, posY, posZ));
        if(rY != 0){model = glm::rotate(model, glm::radians(rY), glm::vec3(0, 1, 0));}
        if(rX != 0){model = glm::rotate(model, glm::radians(rX), glm::vec3(1, 0, 0));}
        if(rZ != 0){model = glm::rotate(model, glm::radians(rZ), glm::vec3(0, 0, 1));}
    }
    return model;
}

void Item::setControleur(Camera* c){
    controleur = c;
}