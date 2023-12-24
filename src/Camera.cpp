#include "Camera.h"

GLFWwindow* Camera::window = nullptr;

float Camera::lastX = 0;
float Camera::lastY = 0;
float Camera::yaw = -90.0f;
float Camera::pitch = 0.0f;
float Camera::vitesse = PLAYER_SPEED;
float Camera::fov = FOV;

bool Camera::firstMouse = true;

glm::vec3 Camera::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 Camera::projection = glm::perspective(glm::radians(fov), (float)FENETRE_LARGEUR /(float)FENETRE_HAUTEUR, 0.1f, 100.0f);

Camera::Camera(GLFWwindow* w, Shader* s, float x, float y, float z){
	window = w;
	shader = s;
    cameraPos = glm::vec3(x, y, z);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	shader->setMat4("projection", projection);
	money = 0;
};

void Camera::deplacement(float d, char** map, unsigned int nX, unsigned nZ){
    const float cameraSpeed = vitesse * d;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		glm::vec3 newLocation = cameraPos + cameraSpeed * cameraFront;
		checkLocation(newLocation, map, nX, nZ);

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		glm::vec3 newLocation = cameraPos - cameraSpeed * cameraFront;
		checkLocation(newLocation, map, nX, nZ);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		glm::vec3 newLocation = cameraPos - glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		checkLocation(newLocation, map, nX, nZ);
	}
	
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		glm::vec3 newLocation = cameraPos + glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		checkLocation(newLocation, map, nX, nZ);
	}
};

void Camera::checkLocation(glm::vec3 newLocation, char** map, unsigned int nX, unsigned nZ){
	if(0 <= (unsigned int)newLocation.x && (unsigned int)newLocation.x < nX && 0 <= (unsigned int)newLocation.z && (unsigned int)newLocation.z < nZ){
		if(map[(unsigned int)newLocation.x][(unsigned int)newLocation.z]!='X'){
			cameraPos = newLocation;
			cameraPos.y = 1.0f;
		}
	}
}

void Camera::mouse_callback(GLFWwindow* w, double xpos, double ypos){
	if(firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed: y ranges bottom to top
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if(pitch > 89.0f){
		pitch = 89.0f;
	}
	if(pitch < -89.0f){
		pitch = -89.0f;
	}
};

glm::mat4 Camera::getView(){
    glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return view;
};

void Camera::setLocation(float x, float y, float z){
	cameraPos.x = x;
	cameraPos.y = y;
	cameraPos.z = z;
}

void Camera::handleCameraEvent(){
	shader->setMat4("view", this->getView());
}

void Camera::setActiveShader(Shader* s){
	shader = s;
}

float* Camera::getSpeedPointer(){
    return &vitesse;
}

float* Camera::getFOVPointer(){
    return &fov;
}

void Camera::applyChanges(){
	projection = glm::perspective(glm::radians(fov), (float)FENETRE_LARGEUR /(float)FENETRE_HAUTEUR, 0.1f, 100.0f);
	shader->setMat4("projection", projection);
};

void Camera::addMoney(int m){
	money += m;
};

int Camera::getMoney(){
	return money;
}

float Camera::getX(){
	return cameraPos.x;
}

float Camera::getY(){
	return cameraPos.y;
}

float Camera::getZ(){
	return cameraPos.z;
}

glm::vec3 Camera::getCameraFront(){
	return cameraFront;
}

glm::vec3 Camera::getCameraPos(){
	return cameraPos;
}
