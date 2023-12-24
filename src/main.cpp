#include <iostream>
#include "Application.h"

int main(){
	Application* app = new Application();
	// Verification de l'initialisation
	if(app->initialized()){
		app->run();
	} else{
		delete app;
		std::cout << "Le premier niveau n'a pas pu etre initialise" << std::endl;
		return 1;
	}
	delete app;
	std::cout << "Fin du programme" << std::endl;
	return 0;
}