#include "glSetup.h"

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
#include <fstream>
using namespace std;

void init();
void quit();
void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void generateTorus();

// Camera configuration
vec3 eye(5, 5, 5);
vec3 center(0, 0, 0);
vec3 up(0, 1, 0);

float AXIS_LENGTH = 3;
float AXIS_LINE_WIDTH = 2;

// Colors
GLfloat bgColor[4] = { 1,1,1,1 };

// Selected example

int CIRCLE_POINTS = 18;
int CIRCLE_NUMS = 36;

vec3** points;

bool drawQuads = true;

int angles = 1;


int main(int argc, char* argv[])
{
	vsync = 0;

	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1;

	// Callback
	glfwSetKeyCallback(window, keyboard);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);

	reshape(window, windowW, windowH);

	init();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		render(window);
		glfwSwapBuffers(window);
	}


	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void init() {
	points = new vec3 * [CIRCLE_NUMS];
	for (int i = 0; i < CIRCLE_NUMS; i++) {

		points[i] = new vec3[CIRCLE_POINTS];
	}
	generateTorus();

	cout << endl;
	cout << "Keyboard Input : right arrow for increasing angle" << endl;
	cout << "Keyboard Input : left arrow for decreasing angle" << endl;
	cout << "Keyboard Input : d for draw wireframe only" << endl;
}



void generateTorus() {
	for (int i = 0; i < CIRCLE_NUMS; i++) {
		float theta = 2.0f * M_PI * i / CIRCLE_NUMS;
		glm::vec3 zDirection(0, 0, 1.0f);
		glm::vec3 yDirection(0.0f, 1.0f, 0);

		for (int j = 0; j < CIRCLE_POINTS; j++) {
			float phi = 2.0f * M_PI * j / CIRCLE_POINTS;
			glm::mat4 M(1.0f);

			M = glm::rotate(M, theta, yDirection);
			M = glm::translate(M, glm::vec3(1.0f, 1.0f, 0.0f));
			M = glm::rotate(M, phi, zDirection);
			M = glm::translate(M, glm::vec3(0.5f, 0.0f, 0.0f));

			// points 배열에 저장
			points[i][j] = glm::vec3(M * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		}
		
	}
}
void drawWireframe() {
	glBegin(GL_QUADS);
	for (int i = 0; i < angles && i < CIRCLE_NUMS; i++) {
		int nextI = (i + 1) % CIRCLE_NUMS;
		for (int j = 0; j < CIRCLE_POINTS; j++) {
			int nextJ = (j + 1) % CIRCLE_POINTS;
			glColor3f(0.0f, 0.0f, 0.0f);

			glVertex3fv(value_ptr(points[i][j]));
			glVertex3fv(value_ptr(points[i][nextJ]));
			glVertex3fv(value_ptr(points[nextI][nextJ]));
			glVertex3fv(value_ptr(points[nextI][j]));

		}
	}
	glEnd();
}

void drawTorus() {
	glBegin(GL_QUADS);
	for (int i = 0; i < angles && i < CIRCLE_NUMS; i++) {
		int nextI = (i + 1) % CIRCLE_NUMS;
		for (int j = 0; j < CIRCLE_POINTS; j++) {
			int nextJ = (j + 1) % CIRCLE_POINTS;

			vec3 v1 = points[i][j] - points[i][nextJ];
			vec3 v2 = points[i][j] - points[nextI][j];
			vec3 v = cross(v1, v2);
			float dotProduct = glm::dot(v, eye);

			if (dotProduct > 0) glColor3f(1.0f, 0.0f, 0.0f);
			else glColor3f(0.0f, 0.0f, 1.0f);

			glVertex3fv(value_ptr(points[i][j]));
			glVertex3fv(value_ptr(points[i][nextJ]));
			glVertex3fv(value_ptr(points[nextI][nextJ]));
			glVertex3fv(value_ptr(points[nextI][j]));
			
		}
	}
	glEnd();
}

void render(GLFWwindow* window) {

	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLineWidth(5 * dpiScaling);

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);

	if (drawQuads) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPolygonOffset(1.0, 1.0);
		drawTorus();
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawWireframe();
}
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE); break;

		case GLFW_KEY_RIGHT:  angles = std::min(angles + 1, CIRCLE_NUMS); break;
		case GLFW_KEY_LEFT: angles = std::max(angles - 1, 1);  break;

		case GLFW_KEY_D: drawQuads = !drawQuads; break;
		}
	}
}
