#version 330 core

uniform vec4 couleur;

out vec4 FragColor;

void main(){
    FragColor = vec4(couleur.x , couleur.y, couleur.z, 1.0);
};