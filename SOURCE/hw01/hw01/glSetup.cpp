#include "glSetup.h"

#include <string.h>
#include <iostream>
using namespace std;

bool perspectiveView = true;

float screenScale = 0.5f;
int screenW = 0, screenH = 0;
int windowW, windowH;
float aspect;
float dpiScaling = 0;

int vsync = 1;

float fovy = 45.4f;
//float fovy = 37.8f;
//float fovy = 27.0f;
//float fovy = 16.1f;
//float fovy = 11.4f;
//float fovy = 6.9f;

float nearDist = 1.0f;
float farDist = 20.0f;

void errorCallback(int error, const char* desciption) {
	cerr << "####" << desciption << endl;
}

void setupProjectionMatrix() {
	if (perspectiveView) gluPerspective(fovy, aspect, nearDist, farDist);
	else glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, -nearDist, farDist);
}

void reshape(GLFWwindow* window, int w, int h) {
	aspect = (float)w / h;

	windowW = w;
	windowH = h;
	// 시작점 좌표, 크기
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	setupProjectionMatrix();

	glfwGetWindowSize(window, &screenW, &screenH);
	cerr << "reshape(" << w << ", " << h << ")";
	cerr << " with screen " << screenW << " x " << screenH << endl;
}

GLFWwindow* initializeOpenGL(int argc, char* argv[], GLfloat bgColor[4]) {
	glfwSetErrorCallback(errorCallback);

	//glf 초기화
	if (!glfwInit()) exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	glfwWindowHint(GLFW_SAMPLES, 4);

	// 윈도우 만들기
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	int monitorW, monitorH;
	glfwGetMonitorPhysicalSize(monitor, &monitorW, &monitorH);
	cerr << "Status : Monitor " << monitorW << "mm x " << monitorH << "mm" << endl;

	const GLFWvidmode* vidoeMode = glfwGetVideoMode(monitor);
	if (screenW == 0) screenW = int(vidoeMode->width * screenScale);
	if (screenH == 0) screenH = int(vidoeMode->height * screenScale);

	// 너비, 높이, 제목, 전체화면 할 모니터, 컨텍스트 복사 할 모니터
	GLFWwindow* window = glfwCreateWindow(screenW, screenH, argv[0], NULL, NULL);
	if (!window) {
		glfwTerminate();
		cerr << "Failed in glfwCreateWindow()" << endl;
		return NULL;
	}

	glfwMakeContextCurrent(window);

	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	glfwSwapBuffers(window);	//보고 있지 않는 화면에 그려놓고 바꾸는 더블 버퍼를 스왑 

	// 윈도우 사이즈 체크 
	glfwGetWindowSize(window, &screenW, &screenH);
	cerr << "Status: Screen " << screenW << " x " << screenH << endl;

	// 윈도우 프레임 체크 
	glfwGetFramebufferSize(window, &windowW, &windowH);
	cerr << "Status: Framebuffer " << windowW << " x " << windowH << endl;

	if (dpiScaling == 0) dpiScaling = float(windowW) / screenW;

	// Callback, 윈도우의 크기가 바뀌면 reshape 함수 부르기
	glfwSetFramebufferSizeCallback(window, reshape);

	cout << "Status: Renderer " << glGetString(GL_RENDERER) << endl;
	cout << "Status: Ventor" << glGetString(GL_VENDOR) << endl;
	cout << "Status: OpenGL " << glGetString(GL_VERSION) << endl;

	glfwSwapInterval(vsync);	// 0 은 immediate mode라서 화면이 찢어질 수 있음

	return window;
}

void drawAxes(float l, float w) {
	glLineWidth(w);

	glBegin(GL_LINES);
	glColor3f(l, 0, 0); glVertex3f(0, 0, 0); glVertex3f(l, 0, 0);
	glColor3f(0, l, 0); glVertex3f(0, 0, 0); glVertex3f(0, l, 0);
	glColor3f(0, 0, l); glVertex3f(0, 0, 0); glVertex3f(0, 0, l);
	glEnd();
}