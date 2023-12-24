#include "Objet.h"

Objet::Objet(float* d, unsigned int nTri, Shader* s){
    shader = s;
    nTriangle = nTri;
    data = d;
    posX = 0; posY = 0; posZ = 0;
    rX = 0; rY = 0; rZ = 0; // si rX=rY=rZ=0 alors rien ne s'affichent

    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*nTriangle*3, data, GL_STATIC_DRAW);

    // A adapter en fonction de notre Shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
}

Objet::~Objet(){
    free(data);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Objet::draw(){
    glBindVertexArray(VAO);
    shader->setMat4("model", this->getModel());
    glDrawArrays(GL_TRIANGLES, 0, nTriangle);
}

void Objet::drawAt(std::vector<glm::vec2> lObjet){
    shader->use();
    glBindVertexArray(VAO);
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ));
    glm::mat4 r2 = glm::rotate(glm::mat4(1.0f), glm::radians(rY), glm::vec3(0, 1, 0));
    glm::mat4 r1 = glm::rotate(glm::mat4(1.0f), glm::radians(rX), glm::vec3(1, 0, 0));
    glm::mat4 r3 = glm::rotate(glm::mat4(1.0f), glm::radians(rZ), glm::vec3(0, 0, 1));
    for(glm::vec2 o : lObjet){
        glm::mat4 temp = glm::mat4(1.0f);
        temp = glm::translate(temp, glm::vec3(o.x +0.5f, 0.0f, o.y +0.5f));
        shader->setMat4("model", temp * trans * r2 * r1 * r3);
        glDrawArrays(GL_TRIANGLES, 0, nTriangle);
    }
}

void Objet::drawAt(liste L){
    if (!isEmpty(L)){
        shader->use();
        glBindVertexArray(VAO);
        glm::mat4 model = glm::mat4(1.0f);
        while(!isEmpty(getFollowing(L))){
            glm::mat4 temp = glm::translate(model, glm::vec3(L->value.x, L->value.y, L->value.z));
            shader->setMat4("model", temp);
            glDrawArrays(GL_TRIANGLES, 0, nTriangle);
            L = getFollowing(L);
        }
    }
}

void Objet::setCoordinate(float x, float y, float z){
    posX = x;
    posY = y;
    posZ = z;
}

void Objet::translate(float x, float y, float z){
    posX += x;
    posY += y;
    posZ += z;
}

void Objet::rotate(float x, float y, float z){
    rX = x;
    rY = y;
    rZ = z;
}

glm::mat4 Objet::getModel(){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(posX, posY, posZ));
    if(rY != 0){model = glm::rotate(model, glm::radians(rY), glm::vec3(0, 1, 0));}
    if(rX != 0){model = glm::rotate(model, glm::radians(rX), glm::vec3(1, 0, 0));}
    if(rZ != 0){model = glm::rotate(model, glm::radians(rZ), glm::vec3(0, 0, 1));}
    return model;
}
