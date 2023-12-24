#include "ListeGraf.h"

// renvoie une liste vide
liste lEmpty(){
    return nullptr;
}

// renvoie une nouvelle liste ou 'L' est le prédécesseur
liste addFollowing(glm::vec3 val, liste L){
    liste newRight;
    newRight = (liste)malloc(sizeof(struct Liste));
    newRight->value = val;
    newRight->previous = L;
    newRight->following = nullptr;
    if(!isEmpty(L)){
        L->following = newRight;
    }
    return newRight;
}

// renvoie une nouvelle liste ou 'L' est le successeur
liste addPrevious(glm::vec3 val, liste L){
    liste newLeft;
    newLeft = (liste)malloc(sizeof(struct Liste));
    newLeft->value = val;
    newLeft->previous = nullptr;
    newLeft->following = L;
    if(!isEmpty(L)){
        L->previous = newLeft;
    }
    return newLeft;
}

// renvoie le successeur de 'L'
liste getFollowing(liste L){
    return L->following;
}

// renvoie le prédécesseur de 'L'
liste getPrevious(liste L){
    return L->previous;
}

// liberation a partir du maillon 'L'
void freeList(liste L){
    if (!isEmpty(L)){
        freeList(getFollowing(L));
        free(L);
    }
}

// retourne vrai si 'L' est vide
bool isEmpty(liste L){
    return L == nullptr;
}

// renvoie la taille de la liste a partir du maillon 'L'
unsigned int size(liste L){
    if (isEmpty(L)){
        return 0;
    } else {
        return 1 + size(getFollowing(L));
    }
}

// retire le maillon 'L' de la Liste
// renvoie le maillon qui le succéde
liste removePrevious(liste L){
    if (!isEmpty(L)){
        liste nPrevious = L->following;
        free(L);
        return nPrevious;
    }
    return nullptr;
}

// retire le maillon 'L' de la Liste
// renvoie le maillon qui le précéde
liste removeFollowing(liste L){
    if (!isEmpty(L)){
        liste nFollowing = L->previous;
        free(L);
        return nFollowing;
    }
    return nullptr;
}