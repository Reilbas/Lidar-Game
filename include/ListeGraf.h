#ifndef LISTE_GRAF
#define LISTE_GRAF

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* Definitions des types */
typedef struct Liste *liste ; 

struct Liste {
  glm::vec3 value; 
  liste following;
  liste previous;
};

liste lEmpty();
liste addFollowing(glm::vec3 val, liste L);
liste addPrevious(glm::vec3 val, liste L);
liste getFollowing(liste L);
liste getPrevious(liste L);
void freeList(liste L);
bool isEmpty(liste L);
unsigned int size(liste L);
liste removePrevious(liste l);
liste removeFollowing(liste l);

#endif 