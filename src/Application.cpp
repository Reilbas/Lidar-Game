#include "Application.h"

// Variables Globales
bool fin = false;
bool flag_true = true;
bool* p_open =& flag_true;
bool drawMap = false;

// Couleurs
float color[4] = {0.2f, 1.0f, 0.0f, 1.0f};
float colorP[4] = {1.0f, 1.0f, 1.0f, 1.0f};

// Delta
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Premier niveau
char strNiveau[128] = "niveaux/niveau0.txt";

// Flag
bool keyFlag = true;
bool mouseFlag = true;

// Scanner
bool scanRectangle = false;
bool scanSimple = false;
int scanX; int scanY;
float scanSize = 0.1f;
int SCAN_DISTANCE = -30;

// IMGUI
bool imguiInit = false;
bool Imgui_window_state = false;
bool start = false;

//ImGuiIO io;

static ImGuiWindowFlags flags;
const ImGuiViewport* viewport;
int my_image_width = 0;
int my_image_height = 0;
unsigned int my_image_texture = 0;



// Prototypes de fonction
float volumeTetra(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
glm::vec3 intersection(glm::vec3 p1, glm::vec3 p2, glm::vec3 t1, glm::vec3 t2, glm::vec3 t3);
bool intersecte(glm::vec3 p1, glm::vec3 p2, glm::vec3 t1, glm::vec3 t2, glm::vec3 t3);
glm::vec3 getPosMoinsLoin(glm::vec3 og, std::vector<glm::vec3> &lPoints);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Application::Application(){
	init = false;
	window = nullptr;
	shader = nullptr;
	camera = nullptr;
	map = nullptr;
	gun = nullptr;
	obj = nullptr;
	graffiti = nullptr;
	//-------------------INITIALISATION-------------------
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//----------------------------------------------
	window = glfwCreateWindow(FENETRE_LARGEUR, FENETRE_HAUTEUR, "Lidar-Game", NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//----------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(2);
	}

	//-------------------PARAMETRES-------------------
	// Zone de rendu graphique
	glViewport(0, 0, FENETRE_LARGEUR, FENETRE_HAUTEUR);
	// Pour que les triangles s'affiche par distance
	glEnable(GL_DEPTH_TEST);
	// Couleur de nettoyage
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// TRIANGLE PLEIN
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// WIREFRAME

    //-------------------SHADERS-------------------
	shader = new Shader("shaders/shader.vert", "shaders/shader.frag");
	shader->use();
	shader->setVec4("couleur", color[0], color[1], color[2], color[3]);

    //--------------Camera(joueur)----------------
	camera = new Camera(window, shader, 1.5f, 1.0f, 1.5f);
	glfwSetCursorPosCallback(window, Camera::mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Liste doublement Chainée de graffiti
	lGraf = lEmpty();
	premier = lEmpty();
	lGrafSize = 0;

    // Creation de la map
    if(loadMap(strNiveau)){
		// Declaration des objets
		unsigned int gunTri, objetTri, grafTri;
		float* gunMesh = MeshMaker::fromObj("assets/lidarGun.obj", 0.0f, 0.0f, 0.0f, 0.05f, gunTri);
		gun = new Item(gunMesh, gunTri, shader);
		gun->setCoordinate(3,0.5,3);
		//gun->setControleur(camera);

		float* objetMesh = MeshMaker::fromObj("assets/item.obj", 0.0f, 0.0f, 0.0f, 0.1f, objetTri);
		obj = new Objet(objetMesh, objetTri, shader);

		float* graffitiMesh = MeshMaker::fromObj("assets/graffiti.obj", 0.0f, 0.0f, 0.0f, 0.01f, grafTri);
		graffiti = new Objet(graffitiMesh, grafTri, shader);

		//----------------IMGUI----------------
		const char* glsl_version = "#version 460";
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		//style
		io.Fonts->AddFontFromFileTTF("assets/PixelOperatorMono.ttf", 23.0f);
		ImGuiStyle &style = ImGui::GetStyle();
		viewport = ImGui::GetMainViewport();
		style.WindowRounding = 12.0f;

		buffer_map = loadFile(strNiveau);
		imguiInit = true;
		init = true;
	}
}

void Application::run(){
	//----------------BOUCLE-DU-JEU----------------
	while (!glfwWindowShouldClose(window) && init){
		//---Temps-"delta"(entre chaque frames)---
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		//---IMGUI---
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		UI(window,viewport,flags);

		//---Gestion-des-Inputs---
		processInput(window);
		if(start){
			
			if(!Imgui_window_state && !fin){
				//camera->deplacement(deltaTime, lvlData, l, L);
				camera->deplacement(deltaTime, map->getLevelData(), map->getLargeur(), map->getLongueur());
				camera->handleCameraEvent();
				//---Scanneurs---
				if(scanSimple){
					for(int i = 0; i < GRAF_PER_FRAME_SPRAY; i++){
						float x = (std::rand() % GRAF_SPRAY_DENSITY_X*2+1 - GRAF_SPRAY_DENSITY_X) / 100;
						float y = (std::rand() % GRAF_SPRAY_DENSITY_Y*2+1 - GRAF_SPRAY_DENSITY_Y) / 100;
						addGraf(x*scanSize, y*scanSize);
					}
				}
				if(scanRectangle){
					for(int i = 0; i < GRAF_PER_FRAME_BOMB; i++){
						if(scanY <= SCAN_Y_MAX){
							addGraf(scanX, scanY);
							if(scanX <= SCAN_X_MAX){
								scanX += GRAF_BOMB_SPACING_X;
							} else {
								scanX = SCAN_X_MIN;
								scanY += GRAF_BOMB_SPACING_Y;
							}
						} else {
							scanRectangle = false;
						}
					}
				}
				// check si sur objet
				for(unsigned int i = 0; i < (unsigned int)objets.size(); i++){
					glm::vec2 o = objets.at(i);
					if(o.x == (int)camera->getX() && o.y == (int)camera->getZ()){
						objets.erase(objets.begin()+i);
						camera->addMoney(10);
						if(objets.size() == 0){
							fin = true;
						}
					}
				}
				//---Operations---
				int rotation = (int)(glfwGetTime()*100)%360;
				float height = sin(glfwGetTime())/4+0.8;

				gun->setCoordinate(0, height, 0);
				gun->rotate(-30, rotation, 0);

				obj->setCoordinate(0, height, 0);
				obj->rotate(-30, rotation, 0);

				//---Rendering---
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// WIREFRAME

				float lum = sin(glfwGetTime()*2)+0.5f;
				//float lum = 1.0f;
				shader->setVec4("couleur", color[0]*lum, color[1]*lum, color[2]*lum, color[3]);
				shader->use();
				if (drawMap){
					map->draw();
				}
				gun->draw();
				// dessine tout les objets ramassable
				obj->drawAt(objets);
				// dessine tout les points
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// TRIANGLE PLEIN
				shader->setVec4("couleur", colorP[0], colorP[1], colorP[2], colorP[3]);
				graffiti->drawAt(premier);
			}
			
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//---Swap-buffers---
		glfwSwapBuffers(window);
	}
}

Application::~Application(){
	//------Liberation-des-ressources-allouées------
	// liberation des objets et struct
	if(obj!=nullptr){delete obj;}
	if(graffiti!=nullptr){delete graffiti;}
	if(map!=nullptr){delete map;}
	if(gun!=nullptr){delete gun;}
	if(shader!=nullptr){delete shader;}
	if(camera!=nullptr){delete camera;}
	free(buffer_map);
	freeList(premier);

	// liberation de IMGUI
	if(imguiInit){
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	// liberation de GLFW
	glfwTerminate();
}

bool Application::loadMap(std::string nomFic){
	if(!LecteurNiveau::isEmpty(nomFic)){
		unsigned int L, l, mapTri;
		char** lvlData = LecteurNiveau::retrievingLevel(nomFic, L, l, camera, objets);
		std::vector<Wall> listeMurs;
    	float* mapData = LecteurNiveau::dataToLevel(lvlData, L, l, mapTri, listeMurs, WALL_HEIGHT);
		map = new Map(lvlData, l, L, mapData, mapTri, listeMurs, shader);
		// on vide la liste de graffiti
		if(!isEmpty(premier)){
			freeList(premier);
			lGraf = lEmpty();
			premier = nullptr;
			lGrafSize = 0;
		}
		return true;
	} else {
		std::cout << "ERREUR: le fichier est vide ou n'existe" << std::endl;
		return false;
	}
}

//------Corps-des-Fonctions------
void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void Application::processInput(GLFWwindow *window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && mouseFlag){
		mouseFlag = false;
		if(scanRectangle){
			scanRectangle = false;
		} else {
			scanRectangle = true;
			scanX = SCAN_X_MIN;
			scanY = SCAN_Y_MIN;
		}
	}
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE){
		mouseFlag = true;
	}

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
		scanSimple = true;
	}
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
		scanSimple = false;
	}
}

void Application::UI(GLFWwindow *window, const ImGuiViewport* viewport, ImGuiWindowFlags flags){
	
	if(!start){
		menu(window,viewport,flags);
	}
	else{
		if(!fin){
			if(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS && keyFlag){
			keyFlag=false;
			Imgui_window_state=!Imgui_window_state;
			if(Imgui_window_state){
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			}
			if(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE){
					keyFlag = true;
			}
			if(Imgui_window_state){
				inventory(window,viewport,flags);
			}
			hud(window,viewport,flags);
		}
		else{
			victory(window,viewport,flags);
		}
	}
	
}

void Application::inventory(GLFWwindow *window, const ImGuiViewport* viewport, ImGuiWindowFlags flags){
	ImGui::SetNextWindowPos(ImVec2(viewport->WorkSize.x/2, viewport->WorkSize.y/2), ImGuiCond_Always, ImVec2(0.5f,0.5f));
	ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x/2, viewport->WorkSize.y/2));
	flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
	ImGui::Begin("Settings",p_open,flags);
	if (ImGui::BeginTabBar(""))
		{
	if (ImGui::BeginTabItem("Settings")){
		ImGui::Checkbox("Draw map",&drawMap);
		ImGui::SliderFloat("Speed", camera->getSpeedPointer(), 1.0f, 30.0f);
		ImGui::SliderFloat("FOV", camera->getFOVPointer(), 1.0f, 180.0f);
		ImGui::ColorEdit4("Color", color);
		ImGui::ColorEdit4("ColorP", colorP);
		ImGui::EndTabItem();
		if( ImGui::Button(  "Apply" , ImVec2( 93.f, 30.f ) )  ){
			camera->applyChanges();
			init = loadMap(strNiveau);
			Imgui_window_state=!Imgui_window_state;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
	if (ImGui::BeginTabItem("Map")){
		ImGui::InputText("niveau", strNiveau, IM_ARRAYSIZE(strNiveau));
		ImGui::SameLine();
		if(ImGui::Button("Load", ImVec2( 93.f, 30.f ))){
			buffer_map = loadFile(strNiveau);
		}
		ImGui::Spacing();
		ImGui::Columns( 2,  "##config-settings" , false );
		ImGui::PushItemWidth(viewport->WorkSize.x*0.45);
		ImGui::InputTextMultiline("##text_map", buffer_map, 10000, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));
		ImGui::PopItemWidth();
		ImGui::EndTabItem();
		if( ImGui::Button(  "Apply" , ImVec2( 93.f, 30.f ) )  ){
			std::ofstream file(strNiveau, std::ios::trunc);
			std::string copy = std::string(buffer_map);
			copy.erase(std::remove(copy.begin(), copy.end(), '\r'), copy.cend());
			file << copy;
			file.close();
			init = loadMap(strNiveau);
			buffer_map = loadFile(strNiveau);
			Imgui_window_state =! Imgui_window_state;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
	if (ImGui::BeginTabItem("Quit")){
		ImGui::Spacing();
		ImGui::Columns( 2,  "##config-settings" , false );
		if(ImGui::Button("Quit")){
				glfwSetWindowShouldClose(window, true);
			}
		ImGui::EndTabItem();
	}
		ImGui::Columns( 1 );
		ImGui::EndTabBar();
		}
	ImGui::End();
}

void Application::menu(GLFWwindow *window, const ImGuiViewport* viewport, ImGuiWindowFlags flags){
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
	ImGui::Begin("Menu",p_open,flags);

		loadTextureFromFile("assets/lidar.jpg",&my_image_texture,&my_image_width,&my_image_height);
		ImGui::SetNextItemAllowOverlap();
		ImGui::Image((void*)(intptr_t)my_image_texture, viewport->WorkSize);
		ImGui::SetWindowFontScale(viewport->WorkSize.y*0.0025);
		ImGui::SetCursorPos(ImVec2((viewport->WorkPos.x)+(viewport->WorkSize.x)*0.01f,(viewport->WorkSize.y)*0.75f));
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0.4f, 0.4f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(0.7f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(0.7f, 0.8f, 0.8f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,5.0f);
		if( ImGui::Button("Start", ImVec2( (viewport->WorkSize.x)*0.2f,(viewport->WorkSize.y)*0.1f ))){
			start=true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		ImGui::Spacing();
		ImGui::SetCursorPos(ImVec2((viewport->WorkPos.x)+(viewport->WorkSize.x)*0.01f,(viewport->WorkSize.y)*0.75f+(viewport->WorkSize.y)*0.115f));
		if(ImGui::Button("Quit", ImVec2( (viewport->WorkSize.x)*0.2f,(viewport->WorkSize.y)*0.1f  ))){
			glfwSetWindowShouldClose(window, true);
		}
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(1);
	ImGui::End();
}

void Application::hud(GLFWwindow *window, const ImGuiViewport* viewport, ImGuiWindowFlags flags){	
	flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMouseInputs;
	ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
	
	ImGui::Begin("HUD",p_open,flags);
		ImGui::SetWindowFontScale(viewport->WorkSize.y*0.0017);
		ImGui::Text("Money : %d", camera->getMoney());
		ImGui::Text("Spread: %.2f", scanSize);
	ImGui::End();
}

void Application::victory(GLFWwindow *window, const ImGuiViewport* viewport, ImGuiWindowFlags flags){	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
	ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
	
	ImGui::Begin("Victory",p_open,flags);
		loadTextureFromFile("assets/victoire.jpg",&my_image_texture,&my_image_width,&my_image_height);
		ImGui::SetNextItemAllowOverlap();
		ImGui::Image((void*)(intptr_t)my_image_texture, viewport->WorkSize);
		ImGui::SetCursorPos(ImVec2((viewport->WorkSize.x)*0.405f,(viewport->WorkSize.y)*0.4f));
		ImGui::SetWindowFontScale(viewport->WorkSize.y*0.00305f);
		ImGui::Text("Victoire");
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0.4f, 0.4f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(0.7f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(0.7f, 0.8f, 0.8f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,5.0f);
		ImGui::SetCursorPos(ImVec2((viewport->WorkSize.x)*0.4f,(viewport->WorkSize.y)*0.7f));
		if(ImGui::Button("Quit", ImVec2( (viewport->WorkSize.x)*0.2f,(viewport->WorkSize.y)*0.1f  ))){
			glfwSetWindowShouldClose(window, true);
		}
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(1);
	ImGui::End();
}

char* Application::loadFile(char const* path){
    char* buffer = 0;
	long bSize;
    FILE * f = fopen (path, "rb");

    if (f){
      fseek (f, 0, SEEK_END);
      bSize = ftell (f);
      fseek (f, 0, SEEK_SET);
      buffer = (char*)malloc ((bSize+1)*sizeof(char));
      if (buffer){
        fread (buffer, sizeof(char), bSize, f);
      }
      fclose (f);
    }
	else{
		f = fopen (path, "w");
		bSize = 0;
		buffer = (char*)malloc ((bSize+1)*sizeof(char));
		fclose (f);
	}
    buffer[bSize] = '\0';

	
    return buffer;
}

bool Application::loadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height){
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned int image_texture;
    glGenTextures(1, &image_texture);
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data==NULL)
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    // Create a OpenGL texture identifier
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
};

bool intersecte(glm::vec3 p1, glm::vec3 p2, glm::vec3 t1, glm::vec3 t2, glm::vec3 t3){
	float v1 = volumeTetra(p1, t1, t2, t3);
	float v2 = volumeTetra(p2, t1, t2, t3);
	if((v1 > 0 && v2 > 0)||(v1 < 0 && v2 < 0)){
		float v3 = volumeTetra(p1, p2, t1, t2);
		float v4 = volumeTetra(p1, p2, t2, t3);
		float v5 = volumeTetra(p1, p2, t3, t1);
		if((v3 > 0 && v4 > 0 && v5 > 0)||(v3 < 0 && v4 < 0 && v5 < 0)){
			return true; // Alors il ya intersection
		}
	}
	return false;
};

glm::vec3 intersection(glm::vec3 p1, glm::vec3 p2, glm::vec3 t1, glm::vec3 t2, glm::vec3 t3){
	glm::vec3 N = glm::cross(t2-t1, t3-t1);
	float t = -(glm::dot(p1-t1,N)/glm::dot(p2-p1,N));
	return glm::vec3(p1 + t*(p2-p1));
};

float volumeTetra(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d){
	return (1.0/6.0) * glm::dot(glm::cross(b-a,c-a),d-a);
};

glm::vec3 getPosMoinsLoin(glm::vec3 og, std::vector<glm::vec3> &lPoints){
	glm::vec3 pointPlusPret = lPoints.at(0);
	float d = glm::distance(og, pointPlusPret);
	for(unsigned int i = 1; i<lPoints.size(); i++){
		float temp = glm::distance(og, lPoints.at(i));
		if(temp < d){
			d = temp;
			pointPlusPret = lPoints.at(i);
		}
	}
	return pointPlusPret;
}

void Application::addGraf(float x, float y){
	std::vector<glm::vec3> inter;
	for(Wall const &wall : map->getMurs()){
		glm::vec3 facing = camera->getCameraFront();

		// matrice de rotation sur l'axe Y
		glm::mat4 temp = glm::mat4(1.0f);
		temp =  glm::rotate(temp, glm::radians(x), glm::vec3(0.0f, 1.0f, 0.0f));
		facing = temp * glm::vec4(facing, 1.0f);

		// hauteur du point
		facing.y = facing.y + (y*0.01f);

		glm::vec3 pointDevantJoueur = glm::vec3(camera->getX() + facing.x * SCAN_DISTANCE, camera->getY() + facing.y * SCAN_DISTANCE, camera->getZ() + facing.z * SCAN_DISTANCE);
		// triangle du haut
		if(intersecte(camera->getCameraPos(), pointDevantJoueur, wall.BL, wall.UL, wall.UR)){
			glm::vec3 pInter = intersection(camera->getCameraPos(), pointDevantJoueur, wall.BL, wall.UL, wall.UR);
			inter.push_back({pInter.x, pInter.y, pInter.z});
		}
		// triangle du bas
		if(intersecte(camera->getCameraPos(), pointDevantJoueur, wall.BL, wall.UR, wall.BR)){
			glm::vec3 pInter = intersection(camera->getCameraPos(), pointDevantJoueur, wall.BL, wall.UR, wall.BR);	
			inter.push_back({pInter.x, pInter.y, pInter.z});
		}
	}

	if(!inter.empty()){
		glm::vec3 pPlusPret = getPosMoinsLoin(camera->getCameraPos(), inter);
		if (isEmpty(premier)){
			premier = addFollowing(pPlusPret, lGraf);
			lGraf = premier;
		} else {
			lGraf = addFollowing(pPlusPret, lGraf);
		}
		lGrafSize++;
		if(lGrafSize > MAX_GRAF){
			premier = removePrevious(premier);
			lGrafSize--;
		}
	}
}

// Controle de la molette
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	scanSize -= yoffset/50;
	if (scanSize < MIN_SPREAD) {scanSize = MIN_SPREAD;}
	if (scanSize > MAX_SPREAD) {scanSize = MAX_SPREAD;}
}

bool Application::initialized(){
	return init;
}